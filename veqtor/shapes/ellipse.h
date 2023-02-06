#pragma once

#include <array>

#include "shape.h"
#include "path.h"

namespace veqtor::shapes {
/**
 * @brief The ellipse struct
 */
class ellipse : public shape {
public:
    ellipse(const apoint &center = apoint(0.0, 0.0), const QSizeF &radius = QSizeF(0.0, 0.0),
            const core::nanoPen &pen = core::nanoPen())
        : shape(QRectF(center - apoint(radius.width(), radius.height()), radius * 2), pen),
          mCenter(center), mRadius(radius) {}
    ellipse(const QRectF &rect, const core::nanoPen &pen = core::nanoPen())
        : shape(rect, pen), mCenter(rect.center()), mRadius(rect.size() / 2) {}
    ellipse(const apoint &center, qreal radius = 1,
            const core::nanoPen &pen = core::nanoPen())
        : ellipse(center, QSizeF(radius, radius), pen) {}
    ellipse(const apoint &topLeft, const apoint &bottomRight,
            const core::nanoPen &pen = core::nanoPen())
        : ellipse(QRectF(topLeft, bottomRight), pen) {}

    operator path() const {
        path ps(mPen);
        const auto &bb = mBoundingBox;
        apoint c = center();

        ps.setPathData({
            {{bb.left(),   c.y()}, pd::move{}},
            {{c.y(),    bb.top()}, pd::qubic{bb.topLeft()}},
            {{bb.right(),  c.y()}, pd::qubic{bb.topRight()}},
            {{c.y(), bb.bottom()}, pd::qubic{bb.bottomRight()}},
            {{bb.left(),   c.y()}, pd::qubic{bb.bottomLeft()}},
            pd::close{}
        });

        ps.setAngle(angle());
        ps.applyTransform();
        return ps;
    }

    operator QRectF() const {
        return mBoundingBox;
    }

    /**
     * @brief setBoundingBox
     * set bounding box and update ellipse center and radius.
     * @param boundingBox
     */
    void setBoundingBox(const QRectF &boundingBox) override {
        shape::setBoundingBox(boundingBox);
        mCenter = boundingBox.center();
        mRadius.setWidth(boundingBox.width()/2);
        mRadius.setHeight(boundingBox.height()/2);
    }

    const QRectF &updateBoundingBox() override {
        mBoundingBox.setTopLeft(apoint(mCenter.x() - mRadius.width(), mCenter.y() - mRadius.height()));
        mBoundingBox.setSize(mRadius * 2);
        return mBoundingBox;
    }

    ShapeType type() const override { return ShapeType::Ellipse; }
    bool isNull() const override { return mRadius.isNull(); }

    /**
     * @brief contains
     * @param point
     * @return
     * PointState::Inside If the ellipse is filled and the point is within the ellipse.
     * PointState::Edge If the point is over the ellipse's outline.
     * PointState::None If none of the preceding statements are true.
     */
    PointState contains(const apoint &point) const override {
        apoint rp = rTransform().map(point);
        const qreal eeq = std::pow(rp.x() - mCenter.x(), 2) / std::pow(mRadius.width(), 2) +
                          std::pow(rp.y() - mCenter.y(), 2) / std::pow(mRadius.height(), 2);
        return mPen.mFill && eeq <= 0.9 ? PointState::Inside :
               0.9 < eeq  && eeq <= 1.1 ? PointState::Edge : PointState::None;
    }

    /**
     * @brief intersects
     * @param otherEllipse
     * @return true if two ellipse has intersect.
     */
    bool intersects(const ellipse &otherEllipse) const {
        if(isCircle() == true) {
            return apoint::dotProduct(center(), otherEllipse.center()) <=
                   mRadius.width() + otherEllipse.mRadius.width();
        } else {
            /**
             *  FIXME: Add ellipse-ellipse collision.
             *  https://en.wikipedia.org/wiki/Ellipse
             */
            throw "not implemented";
            return false;
        }
    }

    /// TODO: constexpr.
    bool isCircle() const { return mRadius.width() == mRadius.height(); }
    qreal maxRadius() const { return std::max(mRadius.width(), mRadius.height()); }

    /// setters
    void setTopLeft(const apoint &newPoint) {
        mBoundingBox.setTopLeft(newPoint);
        mCenter = mBoundingBox.center();
        mRadius = mBoundingBox.size()/2;
    }
    void setBottomRight(const apoint &newPoint) {
        mBoundingBox.setBottomRight(newPoint);
        mCenter = mBoundingBox.center();
        mRadius = mBoundingBox.size()/2;
    }
    void setCenter(const apoint &newCenter) {
        mCenter = newCenter;
        updateBoundingBox();
    }
    void setRadius(const QSizeF &newRadius) {
        mRadius = newRadius;
        updateBoundingBox();
    }
    void setRadius(qreal xrad, qreal yrad) {
        mRadius = QSizeF(xrad, yrad);
        updateBoundingBox();
    }
    void setRadius(qreal rad) {
        mRadius = QSize(rad, rad);
        updateBoundingBox();
    }

    /// getters
    apoint center() const override { return mCenter; }
    apoint topLeft() const { return mBoundingBox.topLeft(); }
    apoint bottomRight() const { return mBoundingBox.bottomRight(); }
    QSizeF radius() const { return mRadius; }

private:
    apoint mCenter;
    QSizeF mRadius;
};
}
