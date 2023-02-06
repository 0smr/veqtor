#pragma once

#include "shape.h"

namespace veqtor::shapes {
class path;

/**
 * @brief The line struct
 */
class line : public QLineF, public shape {
public:
    enum LineType { None, Point, Line, Qubic, Curve, Arc };

    line(const QLineF& l = QLineF(), const core::nanoPen& pen = core::nanoPen())
        : QLineF(l), shape(l, pen) {}
    line(const apoint& p1, const apoint& p2, const core::nanoPen& pen = core::nanoPen())
        : line(QLineF(p1, p2), pen) {}

    operator path() const;

    void setBoundingBox(const QRectF &boundingBox) override {
        shape::setBoundingBox(boundingBox);
        setP1(boundingBox.topLeft());
        setP2(boundingBox.bottomRight());
    }

    bool isNull() const override { return QLineF::isNull(); }
    ShapeType type() const override { return ShapeType::Line; }

    /**
     * @brief contains
     * check if point is over line or not.
     * @abstract
     * for p1, p2 and p3, there is a p3 between p1 and p2
     * if distance p1 and p3 plus to distance p3 and p2 are equal to
     * distance p1 and p2.
     *  p1<---->p3<----------->p2
     *   ^---------------------^
     * @param point
     * @return PointState::Edge or PointState::None
     */
    PointState contains(const apoint &point) const override {
        return apoint(point).isBetween(p1(), p2()) ? PointState::Edge : PointState::None;
    }

    /// getters
    apoint p1() const { return apoint(QLineF::p1()); }
    apoint p2() const { return apoint(QLineF::p2()); }

    /// setters
    void setP1(const apoint &p) {
        QLineF::setP1(p);
        mBoundingBox.setTopLeft(p);
    }
    void setP2(const apoint &p) {
        QLineF::setP2(p);
        mBoundingBox.setBottomRight(p);
    }
};
}
