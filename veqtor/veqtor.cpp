#include "veqtor.h"
#include "nanopainter.h"

#include "nanopen.h"
#include "elements/element.h"
#include "elements/graphic.h"
#include "utils/svgtools.h"

#include "painthelper.h"

namespace veqtor::canvas {
veqtor::veqtor(QQuickItem *parent) : QNanoQuickItem(parent) {}

QNanoQuickItemPainter *veqtor::createItemPainter() const {
    core::nanoPainter *qquickItem = new core::nanoPainter();
    qquickItem->setCanvas(this);
    return qquickItem;
}

void veqtor::hoverMoveEvent(QHoverEvent* event) {
    using elements::element;

    if(mRoot) {
        QPointF mousePosition = event->posF();

        /// Walk through all of the shape element nodes.
        /// TODO: Change the traversal type to reversed post-order.
        mRoot->any([&mousePosition, this](const QPointer<element> &el) {
            bool contains = el->contains(mousePosition);
            if(contains) emit hovered(el);
            return contains;
        });
    }
    QQuickItem::hoverMoveEvent(event);
}

void veqtor::componentComplete() {
    /**
     * @abstract In this section, we connect all QML-side declared properties to the "propertyChanged()" slot.
     *  The signal names are a mix of the property name and the "Changed()" string.
     */
    int num = metaObject()->propertyCount();
    for(int i = metaObject()->propertyOffset(); i < num; ++i) {
        QMetaProperty mp = metaObject()->property(i);
        if(mp.isWritable() && !mp.isConstant()) {
            QByteArray signalName = QT_STRINGIFY(QSIGNAL_CODE) + QByteArray(mp.name()) + "Changed()";
            connect(this, signalName.constData(), this, SLOT(propertyChanged()));
        }
    }

    setElementsToProperties();

    QQuickItem::componentComplete();
}

void veqtor::painter(QNanoPainter *painter) const {
    if(!mRoot) return;

    using elements::element;
    paintHelper::globalTransform.reset();

    /// TODO: move this lines to onSourceChanged & onViewBoxChanged, this lines can be calculated ones.
    if(mRoot->type() == element::SVG && !mRoot->viewBox().isNull()) {
        /// TODO: Add more scale types.
        QSizeF size = mRoot->viewBox().size();
        float scale = std::min({width()/size.width(), height()/size.height()});
        paintHelper::globalTransform.scale(scale, scale);
    }

    /// Walk through all of the shape element nodes.
    mRoot->walk([&painter](const QPointer<element> &el) {
        if(el->isGraphic()) {
            auto graphic = dynamic_cast<elements::graphic*>(el.data());
            paintHelper::drawShape(painter, *graphic, graphic->pen());
        }
    });
}

void veqtor::setSrc(const QString &src) {
    if(mSrc == src) return;
    mSrc = src;
    emit srcChanged();

    QString data = src;

    /// If src is file address extract it's data.
    if(src.size() < 256 && (src.startsWith("qrc") || src.startsWith("file"))) {
        QString path = src.startsWith("file") ? QUrl{src}.toLocalFile() : QUrl{src}.path();
        QFile file(path);

        if(file.open(QFile::ReadOnly)) {
            data = file.readAll();
            file.close();
        }
    }

    /// Delete old tree
    if(mRoot) mRoot->deleteLater();
    mRoot = nullptr;

    /// Generate new tree
    const auto root = utils::svgTools::svgParser(data, this);

    if(root) {
        mRoot = dynamic_cast<elements::svg*>(root.data());
        mRoot->walk([this](const QPointer<elements::element>& el) {
            connect(el, &elements::element::updated, this, &veqtor::update);

            if(!el->id().isEmpty()) {
                mDocument[el->id()] = QVariant::fromValue(el.data());
            }
        });

        setElementsToProperties();

        emit documentChanged();
        emit rootChanged();
        emit svgLoaded();
    }

    update();
}

void veqtor::setElementsToProperties() {
    /**
     * @abstract Set elements to the names of their associated properties depending on their Ids.
     */
    int num = metaObject()->propertyCount();
    for(int i = metaObject()->propertyOffset(); i < num; ++i) {
        QMetaProperty mp = metaObject()->property(i);
        QVariant evar = mDocument[mp.name()];
        auto name = mp.name();

        if((!mp.isWritable() || mp.isConstant()) && evar.isValid()) {
            auto eptr = evar.value<elements::element*>();
            if(eptr) eptr->setAttributes(property(name).toMap());
            setProperty(name, evar);

            qDebug() << name << property(name) << property(name).toMap();
        }
    }
}

void veqtor::update() {
    if(isEnabled() == true) { QQuickItem::update(); }
}

void veqtor::propertyChanged() {
    /// FIXME: There is a chance that this slot will be called before mRoot initialization.

    QMetaMethod metaMethod = sender()->metaObject()->method(senderSignalIndex());
    QByteArray name = metaMethod.name().chopped(7);
    QVariant prop = property(name.constData());

    if(prop.type() == QVariant::UserType && mDocument.contains(name) && !mDocument[name].isNull()) {
        auto elm = mDocument[name].value<elements::element*>();
        if(elm) elm->setAttributes(prop.toMap());
    }
}
} // namespace veqtor::canvas
