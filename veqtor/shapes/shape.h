#pragma once

#include <memory>
#include <array>

#include <QPointer>

#include "apoint.h"
#include "../nanopen.h"

namespace veqtor::shapes {
enum ShapeType {
    Shape = 0x0,
    Line,
    Path,
    Rect,
    Circle,
    Ellipse,
    Polyline,
    Polygon,
    Text,
    TextPath,
};

class shape {
public:
    virtual ~shape() = 0;

    shape() = default;
    shape(const QRectF &rect, const core::nanoPen &pen = core::nanoPen())
        : mBoundingBox(rect), mPen(pen), mTransform{nullptr} {}
    shape(const apoint p1, const apoint p2, const core::nanoPen &pen = core::nanoPen())
        : shape(QRectF(p1, p2), pen) {}
    shape(const QLineF line, const core::nanoPen &pen = core::nanoPen())
        : shape(QRectF(line.p1(), line.p2()), pen) {}
    shape(const core::nanoPen &pen)
        : shape(QRectF(), pen) {}

    /**
     * @brief boundingPoints
     * @return This function returns all 8 points clockwise around the bounding box,
     *  beginning at the top left.
     *  0────1────2
     *  │         │
     *  7    ∙    3
     *  │         │
     *  6────5────4
     */
    std::array<apoint, 8> boundingBoxHandles() const {
        const QRectF &bb = mBoundingBox;
        std::array<apoint, 8> boundPoints {
            bb.topLeft(),    {bb.center().x(), bb.top()},
            bb.topRight(),   {bb.right()     , bb.center().y()},
            bb.bottomRight(),{bb.center().x(), bb.bottom()},
            bb.bottomLeft(), {bb.left()      , bb.center().y()},
        };

        QTransform t = transformer();
        for(auto &point : boundPoints) point  = t.map(point);

        return boundPoints;
    }

    void expandBoundigBox(const apoint& p) {
        mBoundingBox = expandRectTo(mBoundingBox, p);
    }

    /// getters
    const QRectF &boundingBox() const { return mBoundingBox; }
    const core::nanoPen &pen() const { return mPen; }
    core::nanoPen &pen() { return mPen; }
    QTransform invertTransformer() const { return transformer().inverted(); }
    QTransform transformer() const {
        return mTransform ? *mTransform : QTransform();
    }

    virtual float width() const { return mBoundingBox.width(); }
    virtual float height() const { return mBoundingBox.height(); }
    virtual apoint center() const { return mBoundingBox.center(); }
    virtual PointState contains(const apoint &point) const { Q_UNUSED(point) return PointState::None; }
    virtual ShapeType type() const { return ShapeType::Shape; }

    virtual bool isNull() const { return mBoundingBox.isNull(); }
    virtual const QRectF &updateBoundingBox() { return mBoundingBox; }
    virtual void setBoundingBox(const QRectF &boundingBox) { mBoundingBox = boundingBox; }

    /// setters
    void setTransform(const QTransform &transform) { *mTransform = transform; }
    void setTransform(QTransform *transform) { mTransform = transform; }
    void setPen(const core::nanoPen &newPen) { mPen = newPen; }

    /// static members
    /**
     * @brief expandRectTo
     * This function expand given rectangle (@a rect) based on input point (@a p).
     * @param rect
     * @param p
     * @return
     */
    static QRectF expandRectTo(const QRectF &rect, const apoint &p) {
        return QRectF{std::min(rect.left(), p.x()),  std::min(rect.top(), p.y()),
                      std::max(rect.right(), p.x()), std::max(rect.bottom(), p.y())};
    }

    /// friends
    /**
     * @brief operator <<
     *  This overload provides the ability to print shapes using QDebug's "<<" operator.
     * @param debug
     * @param shape
     * @return QDebug object
     */
    friend QDebug operator << (QDebug& debug, const shape& shape) {
        int index = std::clamp(shape.type() * 1, 0, 5);
        QString names[] = {"shape", "path", "line", "polygon", "ellipse", "rectangle"};
        return debug << "sh::" << names[index] << "(" << shape.boundingBox() << ")";
    }

    friend QDebug operator << (QDebug &debug, const std::shared_ptr<shape> shapePtr) {
        return (debug << *shapePtr);
    }

protected:
    QRectF mBoundingBox;
    core::nanoPen mPen;
    QTransform *mTransform;
};
}  // namespace veqtor::shape
