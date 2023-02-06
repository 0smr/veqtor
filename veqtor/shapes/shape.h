#pragma once

#include <memory>
#include <array>

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
    shape(const QRectF &rect, const core::nanoPen &pen = core::nanoPen(), float angle = 0.0f)
        : mBoundingBox(rect), mPen(pen), mAngle(angle) {}
    shape(const apoint p1, const apoint p2, const core::nanoPen &pen = core::nanoPen(), float angle = 0.0f)
        : shape(QRectF(p1, p2), pen, angle) {}
    shape(const QLineF line, const core::nanoPen &pen = core::nanoPen(), float angle = 0.0f)
        : shape(QRectF(line.p1(), line.p2()), pen, angle) {}
    shape(const core::nanoPen &pen, float angle = 0.0f)
        : shape(QRectF(), pen, angle) {}

    QTransform transform() const {
        apoint c = mBoundingBox.center();
        return QTransform()
            .translate(c.x(), c.y())
            .rotate(mAngle)
            .translate(-c.x(), -c.y());
    }

    QTransform rTransform() const {
        apoint c = mBoundingBox.center();
        return QTransform()
            .translate(c.x(), c.y())
            .rotate(-mAngle)
            .translate(-c.x(), -c.y());
    }

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
        QTransform t = transform();
        for(auto &point : boundPoints) point  = t.map(point);
        return boundPoints;
    }

    void expandBoundigBox(const apoint& p) {
        mBoundingBox = expandRectTo(mBoundingBox, p);
    }

    /// getters
    float angle() const { return mAngle; }

    const QRectF &boundingBox() const { return mBoundingBox; }
    const core::nanoPen &pen() const { return mPen; }
    core::nanoPen &pen() { return mPen; }

    virtual float width() const { return mBoundingBox.width(); }
    virtual float height() const { return mBoundingBox.height(); }
    virtual apoint center() const { return mBoundingBox.center(); }
    virtual PointState contains(const apoint &point) const { Q_UNUSED(point) return PointState::None; }
    virtual ShapeType type() const { return ShapeType::Shape; }

    virtual bool isNull() const { return mBoundingBox.isNull(); }
    virtual const QRectF &updateBoundingBox() { return mBoundingBox; }
    virtual void setBoundingBox(const QRectF &boundingBox) { mBoundingBox = boundingBox; }

    /// setters
    void setAngle(float newAngle) { mAngle = newAngle; }
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
    float mAngle;
};
}  // namespace veqtor::shape
