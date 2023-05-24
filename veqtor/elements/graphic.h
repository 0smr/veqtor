#pragma once

#include <QObject>

#include <map>

#include "element.h"
#include "../shapes/shapes.h"
#include "../utils/svgtools.h"
#include "../utils/tools.h"

namespace veqtor::elements {
using utils::svgTools;
using utils::tools;

class graphic: public element {
    Q_OBJECT
    Q_PROPERTY(QColor fill READ fill WRITE setFill NOTIFY fillChanged)
    Q_PROPERTY(QColor stroke READ stroke WRITE setStroke NOTIFY strokeChanged)
    Q_PROPERTY(float strokeWidth READ strokeWidth WRITE setStrokeWidth NOTIFY strokeWidthChanged)
public:
    using shape_sptr = std::shared_ptr<shapes::shape>;

    graphic(const shape_sptr& sh, QObject* parent, const QMap<QString, QString>& attrs);
    graphic(const shape_sptr& sh, QObject* parent = nullptr);
    graphic(QObject* parent = nullptr);

    graphic(const graphic&) = delete;
    graphic &operator = (const graphic&) = delete;

    /// operators
    operator shapes::shape*();
    operator shape_sptr();

    /// getters
    const shape_sptr &shape() const;
    shape_sptr &shape();

    core::nanoPen pen() const override;

    virtual Type type() const override;
    virtual bool contains(const QPointF& point) const override;
    void setOpacity(qreal _opacity) override {
        mShape->pen().mOpacity = _opacity;
        element::setOpacity(_opacity);
    }

    QColor fill() const;
    QColor stroke() const;
    float strokeWidth() const;

    void setFill(const QColor& color);
    void setStroke(const QColor& color);
    void setStrokeWidth(float width);

    virtual void setAttributes(const QVariantMap &attrs) override;

private:
    static QStringList mainAttrs() { return {"fill", "stroke", "stroke-width"}; }

signals:
    void fillChanged();
    void strokeChanged();
    void strokeWidthChanged();

protected:
    shape_sptr mShape;
};
}
