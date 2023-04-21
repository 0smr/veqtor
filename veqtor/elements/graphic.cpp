#include "graphic.h"
#include "../utils/svgtools.h"

namespace veqtor::elements {

graphic::graphic(const shape_sptr &sh, QObject *parent, const QMap<QString, QString> &attrs)
    : element{parent, tools::filter(attrs, mainAttrs())}, mShape(sh) {
    mShape->setTransform(&mTransformBuff);
    mShape->setPen(core::nanoPen{
        svgTools::normColor(attrs["fill"]).rgba(),
        svgTools::normColor(attrs["stroke"]).rgba(),
        svgTools::normSW(attrs["stroke-width"])
    });
}

graphic::graphic(const shape_sptr &sh, QObject *parent): graphic{sh, parent, {}} {}

graphic::graphic(QObject *parent): graphic{nullptr, parent, {}} {}

graphic::operator shapes::shape *() { return mShape.get(); }

graphic::operator shape_sptr() { return mShape; }

const graphic::shape_sptr &graphic::shape() const { return mShape; }

graphic::shape_sptr &graphic::shape() { return mShape; }

core::nanoPen graphic::pen() const { return mShape->pen(); }

element::Type graphic::type() const { return Type(mShape->type()); }

bool graphic::contains(const QPointF &point) const {
    return mShape->contains(point);
}

QColor graphic::fill() const { return mShape->pen().mFill; }

QColor graphic::stroke() const { return mShape->pen().mStroke; }

float graphic::strokeWidth() const { return mShape->pen().mWidth; }

void graphic::setFill(const QColor &color) {
    QRgb &fill = mShape->pen().mFill;
    if(fill == color.rgba()) return;
    fill = color.rgba();

    emit fillChanged();
    emit updated();
}

void graphic::setStroke(const QColor &color) {
    QRgb &stroke = mShape->pen().mStroke;
    if(stroke == color.rgba()) return;
    stroke = color.rgba();

    emit strokeChanged();
    emit updated();
}

void graphic::setStrokeWidth(float width) {
    float &w = mShape->pen().mWidth;
    if(qFuzzyCompare(w , width)) return;
    w = width;

    emit strokeWidthChanged();
    emit updated();
}

void graphic::setAttributes(const QVariantMap &attrs) {
    if(attrs.isEmpty()) return;
    if(attrs.contains("fill")) setFill(attrs["fill"].value<QColor>());
    if(attrs.contains("stroke")) setStroke(attrs["stroke"].value<QColor>());
    if(attrs.contains("stroke-width")) {
        setStrokeWidth(svgTools::normSW(attrs["stroke-width"].toString()));
    }

    element::setAttributes(tools::filter(attrs, mainAttrs()));
}
} // namespace veqtor::elements
