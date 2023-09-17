#include "epath.h"
#include "../utils/svgtools.h"

namespace veqtor::elements {
using utils::tools;

epath::epath(const QMap<QString, QString> &attrs, QObject *parent, const shapes::path &p)
    : graphic{std::make_shared<shapes::path>(p), parent, tools::filter(attrs, mainAttrs())},
      mData{attrs["d"]} {
    pathShape()->setPathData(svgTools::svgPathParser(mData));
}

epath::epath(const QString &d, QObject *parent)
    : epath{{{"d", d}}, parent, shapes::path()} {}

epath::epath(const shapes::path &pathObject, QObject *parent)
    : epath{{}, parent, pathObject} {}

std::shared_ptr<shapes::path> epath::pathShape() {
    return std::dynamic_pointer_cast<shapes::path>(mShape);
}

std::shared_ptr<shapes::path> epath::pathShape() const {
    return std::dynamic_pointer_cast<shapes::path>(mShape);
}

void epath::setData(const QString &d) {
    if(mData == d) return;
    mData = d;
    pathShape()->setPathData(svgTools::svgPathParser(mData));

    emit dataChanged();
    emit updated();
}

void epath::setAttributes(const QVariantMap &attrs) {
    if(attrs.isEmpty()) return;
    if(attrs.contains("d")) setData(attrs["d"].toString());

    graphic::setAttributes(tools::filter(attrs, mainAttrs()));
}

QVariantMap epath::at(long long index) const {
    auto shape = pathShape();
    return index < shape->size() ? shape->at(index).map() : QVariantMap();
}

QVariantMap epath::shift() {
    auto pd = at(0);
    pathShape()->leftShift();
    emit updated();
    return pd;
}

void epath::close() {
	pathShape()->close();
	emit updated();
}

void epath::vTo(qreal y, bool rel) {
    pathShape()->vTo(y, rel);
    emit updated();
}

void epath::hTo(qreal x, bool rel) {
    pathShape()->hTo(x, rel);
    emit updated();
}

void epath::moveTo(QPointF to, bool rel) {
    pathShape()->moveTo(to, rel);
    emit updated();
}

void epath::lineTo(QPointF to, bool rel) {
    pathShape()->lineTo(to, rel);
    emit updated();
}

void epath::quadTo(const QPointF &c, const QPointF &to, bool rel) {
    pathShape()->quadTo(c, to, rel);
    emit updated();
}

void epath::cubicTo(const QPointF &c1, const QPointF &c2, const QPointF &to, bool rel) {
    pathShape()->cubicTo(c1, c2, to, rel);
    emit updated();
}

void epath::arcTo(QPointF to, QSizeF radius, qreal xrot, bool larc, bool sweep, bool rel) {
    pathShape()->arcTo(to, radius, xrot, larc, sweep, rel);
    emit updated();
}

void epath::pop() {
    pathShape()->pop();
    emit pointsChanged(size() -1);
    emit updated();
}

void epath::push(char type, const QPointF &to, const QVariantMap &data, bool relative) {
    pathShape()->push(type, to, data, relative);
    emit updated();
}
}
