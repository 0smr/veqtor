#include "epath.h"
#include "../utils/svgtools.h"

namespace veqtor::elements {
using utils::tools;

epath::epath(const QMap<QString, QString> &attrs, QObject *parent, const shapes::path &p)
    : graphic{std::make_shared<shapes::path>(p), parent, tools::filter(attrs, mainAttrs())},
      mData{attrs["d"]}, mPathLength{attrs["path-length"].toDouble()} {
    pathShape()->setPathData(svgTools::svgPathParser(mData));
}

epath::epath(const QString &d, QObject *parent)
    : epath{{{"d", d}}, parent, shapes::path()} {}

epath::epath(const shapes::path &pathObject, QObject *parent)
    : epath{{}, parent, pathObject} {}

std::shared_ptr<shapes::path> epath::pathShape() {
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
    if(attrs.contains("path-length")) {
        setPathLength(attrs["path-length"].toDouble());
    }

    graphic::setAttributes(tools::filter(attrs, mainAttrs()));
}

void epath::setPathLength(qreal newPathLength) {
    if(qFuzzyCompare(mPathLength, newPathLength)) return;
    mPathLength = newPathLength;
    emit PathLengthChanged();
    emit updated();
}
}
