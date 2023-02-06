#pragma once

#include "qnanopainter.h"
#include "qnanoquickitem.h"
#include "qnanoquickitempainter.h"

#include <qnamespace.h>

#include <QQmlApplicationEngine>
#include <QLine>
#include <QPen>
#include <QPoint>
#include <QRect>
#include <QQmlParserStatus>

#include <array>
#include <cmath>
#include <vector>
#include <memory>
#include <algorithm>

#include "shapes/shapes.h"
#include "elements/svg.h"

namespace veqtor::canvas {
class veqtor : public QNanoQuickItem {
    Q_OBJECT
    /// TODO: Use the new type registration method.
    // QML_NAMED_ELEMENT(Veqtor)
    // QML_ADDED_IN_MINOR_VERSION(1)

    Q_PROPERTY(QString src READ src WRITE setSrc NOTIFY srcChanged)
    Q_PROPERTY(QVariantMap document READ document NOTIFY documentChanged)
public:
    /** @brief The Tools enum */
    veqtor(QQuickItem *parent = nullptr);

    /**
     * @brief createItemPainter
     * @return QNanoQuickItemPainter
     */
    QNanoQuickItemPainter *createItemPainter() const override;

    /**
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

    QString src() const { return mSrc; }
    void setSrc(const QString& src);

    QVariantMap document() const { return mDocument; }

    Q_INVOKABLE QVariant getElementById(const QString &id) {
        return mDocument[id];
    }

private:
    void setElementsToProperties();

public slots:
    void update();
    void propertyChanged();

signals:
    void srcChanged();
    void rootChanged();
    void documentChanged();
    void svgLoaded();
    void hovered(QPointer<elements::element> el);

private:
    QPointer<elements::svg> mRoot;
    QVariantMap mDocument;
    QString mSrc;
};

static void registerVeqtorType() {
    qmlRegisterType<veqtor>("veqtor", 0, 1, "Veqtor");
}
Q_COREAPP_STARTUP_FUNCTION(registerVeqtorType)
}
