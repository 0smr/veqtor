#include <QTimer>
#include <QPointF>
#include <QLine>
#include <QFileInfo>

#include "veqtor.h"
#include "nanopainter.h"

#include "nanopen.h"
#include "elements/element.h"
#include "elements/graphic.h"
#include "utils/svgtools.h"

#include "painthelper.h"

namespace veqtor::canvas {
veqtor::veqtor(QQuickItem *parent) : QNanoQuickItem(parent) {
    /// This line sets timer values.
    /// The timer is used to limit the update and paint framerate to 60 FPS.
    mUpdateTimer.setInterval(1000/60 - 1.5);
    mUpdateTimer.setSingleShot(true);
    mUpdateTimer.callOnTimeout(this, &QQuickItem::update);

    connect(this, &veqtor::widthChanged, this, veqtor::adjustResponsive);
    connect(this, &veqtor::heightChanged, this, veqtor::adjustResponsive);
}

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
        /// The `any` function acts as a pre-order traversal, going through all the elements.
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
    using elements::element;

    if(!mRoot) return;

    /// Walk through all of the shape element nodes.
    mRoot->walk([&painter, this](const QPointer<element> &el) {
        if(el->isGraphic()) {
            auto graphic = dynamic_cast<elements::graphic*>(el.data());
            paintHelper::drawShape(painter, *graphic, graphic->pen(), mAdjustment);
        }
    });
}

void veqtor::setSrc(const QString &src) {
    if(mSrc == src) return;
    mSrc = src;
    emit srcChanged();

    QString data = src;

    /// If src is file address extract it's data.
    if(src.size() < 256 && !src.trimmed().startsWith("<svg")) {
        QString path = utils::tools::toValidFilePath(src);
        QFile file(path);

        data.clear();

        if(file.open(QFile::ReadOnly)) {
            data = file.readAll();
            file.close();
        }
    }

    /// Delete old tree
    /// There is a chance that svgParser return nullptr value, and this would cause
    if(mRoot) mRoot->deleteLater();
    mRoot = nullptr;

    /// Generate new tree
    const auto root = utils::svgTools::svgParser(data, this);

    if(root) {
        mRoot = dynamic_cast<elements::svg*>(root.data());
        mDocument.clear();
        mRoot->walk([this](const QPointer<elements::element>& el) {
            connect(el, &elements::element::updated, this, &veqtor::update);

            if(!el->id().isEmpty()) {
                mDocument[el->id()] = QVariant::fromValue(el.data());
            }
        });

        connect(mRoot, &elements::svg::viewBoxChanged, this, &veqtor::adjustSize);

        adjustSize();
        setElementsToProperties();

        emit documentChanged();
        emit rootChanged();
        emit svgLoaded();
    }

    update();
}

void veqtor::setElementsToProperties() {
    /**
     * @brief Set elements to the names of their associated properties depending on their Ids.
     *  Put the element pointer into @a evar, and if the element exists,
     *  assign data from the property with the same name as the element ID to it.
     */

    int num = metaObject()->propertyCount();
    for(int i = metaObject()->propertyOffset(); i < num; ++i) {
        QMetaProperty mp = metaObject()->property(i);
        QVariant evar = mDocument[mp.name()];
        auto name = mp.name();

        if(evar.isValid()) {
            auto eptr = evar.value<elements::element*>();
            if(eptr) eptr->setAttributes(property(name).toMap());
        }
    }
}

void veqtor::adjustSize() {
    QRectF viewBox = mRoot->viewBox();

    setImplicitWidth(viewBox.width());
    setImplicitHeight(viewBox.height());
    adjustResponsive();
}

void veqtor::adjustResponsive() {
    QSizeF size  = mRoot->viewBox().size();
    float scale  = std::min({width()/size.width(), height()/size.height()});
    QPointF offset{width() - scale * size.width(), height() - scale * size.height()};

    mAdjustment.reset();
    /// Move the SVG shape to center of Item.
    mAdjustment.translate(offset.x()/2, offset.y()/2);
    /// Scale the SVG shape to fit
    mAdjustment.scale(scale, scale);
}

void veqtor::update() {
    if(!mUpdateTimer.isActive() && isEnabled()) {
        mUpdateTimer.start();
    }
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
