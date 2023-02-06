#pragma once

#include <QObject>

#include "../shapes/shapes.h"
#include "container.h"
#include "../utils/svgtools.h"
#include "../utils/tools.h"

namespace veqtor::elements {
/// TODO: Inherit the <svg> element from the <view> element.
///  https://developer.mozilla.org/en-US/docs/Web/SVG/Element/view
class svg final: public container {
    Q_OBJECT
public:
    explicit svg(QMap<QString, QString> attrs, QObject *parent = nullptr)
        : container{parent, utils::tools::filter(attrs, {"viewBox"})},
          mViewBox(utils::svgTools::parseViewBox(attrs["viewBox"])) {}

    Type type() const override { return Type::SVG; }
    const QRectF &viewBox() const { return mViewBox; }
    void setViewBox(const QRectF &newViewBox) { mViewBox = newViewBox; }

signals:
private:
    QRectF mViewBox;
};
}
