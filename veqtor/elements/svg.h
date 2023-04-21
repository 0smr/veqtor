#pragma once

#include <QObject>

#include "../shapes/shapes.h"
#include "container.h"
#include "../utils/svgtools.h"
#include "../utils/tools.h"

namespace veqtor::elements {
/// TODO: Inherit the <svg> element from the <view> element.
///  https://developer.mozilla.org/en-US/docs/Web/SVG/Element/view
class svg: public container {
    Q_OBJECT
    Q_PROPERTY(QRectF viewBox READ viewBox WRITE setViewBox NOTIFY viewBoxChanged)
public:
    explicit svg(QMap<QString, QString> attrs = {}, QObject *parent = nullptr)
        : container{parent, utils::tools::filter(attrs, {"viewBox"})},
          mViewBox(utils::svgTools::parseViewBox(attrs["viewBox"])) {}

    Type type() const override { return Type::SVG; }
    const QRectF &viewBox() const { return mViewBox; }
    void setViewBox(const QRectF &viewBox) {
        if(viewBox == mViewBox) return;
        mViewBox = viewBox;
        emit viewBoxChanged();
        emit updated();
    }

signals:
    void viewBoxChanged();

private:
    QRectF mViewBox;
};
}
