#pragma once

#include "qnanopainter.h"
#include "qnanoquickitem.h"
#include "qnanoquickitempainter.h"

#include <qnamespace.h>

#include <QQmlApplicationEngine>
#include <QTimer>

#include <array>
#include <cmath>
#include <vector>
#include <memory>
#include <algorithm>

#include "shapes/shapes.h"
#include "elements/svg.h"
#include "elements/epath.h"

namespace veqtor::canvas {
class veqtor : public QNanoQuickItem {
    Q_OBJECT
    /// TODO: Use the new type registration method.
    // QML_NAMED_ELEMENT(Veqtor)
    // QML_ADDED_IN_MINOR_VERSION(1)

    Q_PROPERTY(QString src READ src WRITE setSrc NOTIFY srcChanged)
    Q_PROPERTY(QVariantMap document READ document NOTIFY documentChanged)
    Q_PROPERTY(QObject* root READ root NOTIFY rootChanged)
    Q_PROPERTY(QSizeF sourceSize READ sourceSize CONSTANT)
public:
    /** @brief The Tools enum */
    veqtor(QQuickItem *parent = nullptr);

    /**
     * @brief createItemPainter
     * @return QNanoQuickItemPainter
     */
    QNanoQuickItemPainter *createItemPainter() const override final;

    /**
     * @abstract This function is called whenever the mouse hovers over the component
     *  and will emit a `hovered` event.
     * @brief hoverMoveEvent
     * @param event
     */
    void hoverMoveEvent(QHoverEvent* event) override;

    /**
     * @brief componentComplete
     * @abstract Calls after component completion.
     */
    void componentComplete() override;

    /**
     * @brief paintHelper
     * @param painter
     */
    void painter(QNanoPainter *painter) const;

    QPointer<elements::svg> root() { return mRoot; }

    QString src() const { return mSrc; }
    void setSrc(const QString& src);

    QVariantMap document() const { return mDocument; }

    Q_INVOKABLE QVariant getElementById(const QString &id) {
        return mDocument[id];
    }

    QSizeF sourceSize() const { return mSourceSize; }

private slots:
    void setElementsToProperties();
    void adjustSize();
    void adjustResponsive();
    void updateDocument(const QString &newId, const QString &oldId) {
        /// This slot modifies the main document in the event that the ID of any element is altered.
        auto sender = qobject_cast<elements::element *>(QObject::sender());

        mDocument.remove(oldId);
        mDocument.insert(newId, QVariant::fromValue(sender));

        emit documentChanged();
    }
    void updateElementAttributes();

public slots:
    void update();

signals:
    void srcChanged();
    void rootChanged();
    void documentChanged();
    void svgLoaded();
    void hovered(QPointer<elements::element> target);

private:
    QPointer<elements::svg> mRoot;
    QVariantMap mDocument;
    QString mSrc;
    QSizeF mSourceSize;

    QTimer mUpdateTimer;
    QTransform mAdjustment;
};

static void registerVeqtorType() {
    qmlRegisterType<veqtor>("veqtor", 0, 1, "Veqtor");
    qmlRegisterType<elements::svg>("veqtor", 0, 1, "Svg");
    qmlRegisterType<elements::epath>("veqtor", 0, 1, "Path");
}
Q_COREAPP_STARTUP_FUNCTION(registerVeqtorType)
}
