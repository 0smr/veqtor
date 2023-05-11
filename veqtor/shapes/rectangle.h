#pragma once

#include <cmath>

#include "shape.h"
#include "path.h"

namespace veqtor::shapes {
/**
 * @brief The rectangular Shapes
 */
class rect: public QRectF, public shape {
public:
    rect(const QRectF &rect = QRectF(), const core::nanoPen &pen = core::nanoPen())
        : QRectF(rect), shape(pen), mCornerRadius({}) {}
    rect(const apoint &topLeft, const apoint &bottomRight, const core::nanoPen &pen = core::nanoPen())
        : rect(QRectF(topLeft, bottomRight), pen) {}

    operator path() const {
        path ps(mPen);

        const auto &rads = mCornerRadius;
        const apoint bb[] = {
            mBoundingBox.topLeft(),
            mBoundingBox.topRight(),
            mBoundingBox.bottomRight(),
            mBoundingBox.bottomLeft(),
        };
        /**
         *  Position anchros depend on corner radiuses.
         *    ╭─p1───p2─╮
         *   p0 r0   r1 p3
         *    │    ∙    │
         *   p7 r3   r2 p4
         *    ╰─p6───p5─╯
         */
        ps.setPathData({
            {bb[0].plus(0, rads[0]), pd::move{}},
            {bb[0].plus(rads[0], 0), pd::quad{bb[0]}},
            {bb[1].sub (rads[1], 0), pd::line{}},
            {bb[1].plus(0, rads[1]), pd::quad{bb[1]}},
            {bb[2].sub (0, rads[2]), pd::line{}},
            {bb[2].plus(rads[2], 0), pd::quad{bb[2]}},
            {bb[3].sub (rads[3], 0), pd::line{}},
            {bb[3].plus(0, rads[3]), pd::quad{bb[3]}},
            {bb[0], pd::line{}},
            pd::close{},
        });
        ps.setTransform(transformer());
        ps.applyTransform();

        return ps;
    }

    const QRectF &updateBoundingBox() override {
        mBoundingBox.setTopLeft(topLeft());
        mBoundingBox.setBottomRight(bottomRight());
        return mBoundingBox;
    }

    /// getters
    ShapeType type() const override { return ShapeType::Rect; }
    bool isNull() const override { return QRectF::isNull(); }

    PointState contains(const apoint &point) const override {
        apoint rp = invertTransformer().map(point);
        if(radius() == 0.0f) {
            const float off = 2.0f; ///> 2x offset for edges area.
            bool inside = adjusted(-off, -off, off, off).contains(rp) && mPen.mFill;
            bool overEdges = rp.isBetween(topLeft(), topRight()) ||
                             rp.isBetween(topRight(), bottomRight()) ||
                             rp.isBetween(bottomRight(), bottomLeft()) ||
                             rp.isBetween(bottomLeft(), topLeft());

            return overEdges ? PointState::Edge :
                     inside  ? PointState::Inside :
                               PointState::None;
        } else {
            /**
             * FIXME: detect if rounded rect contains point.
             * https://stackoverflow.com/questions/401847/circle-rectangle-collision-detection-intersection
             */
            qErrnoWarning("method rect::contains() can't handle rounded rectangles.");
            return PointState::None;
        }
    }

    /// setters
    void setBoundingBox(const QRectF &boundingBox) override {
        shape::setBoundingBox(boundingBox);
        setTopLeft(boundingBox.topLeft());
        setBottomRight(boundingBox.bottomRight());
    }
    void setTopLeft(const apoint &p) {
        QRectF::setTopLeft(p);
        updateBoundingBox();
    }
    void setBottomRight(const apoint &p) {
        QRectF::setBottomRight(p);
        updateBoundingBox();
    }
    void setRectFromCenter(const apoint &center, const apoint &bottomRight) {
        /// FIXME
        setTopLeft(center);
        setBottomRight(bottomRight);
    }
    void setRect(const QRectF &rect = QRectF()) {
        setTopLeft(rect.topLeft());
        setBottomRight(rect.bottomRight());
    }
    void setCornerRadius(const std::array<float, 4>& newCornerRadius) {
        mCornerRadius = newCornerRadius;
    }

    /// getters
    apoint center() const override { return QRectF::center(); }
    float radius() const {
        for(const auto rad : mCornerRadius)
            if(rad != mCornerRadius[0])
                return NAN;
        return mCornerRadius[0];
    }
    const std::array<float, 4> &radiuses() const { return mCornerRadius; }

private:
    std::array<float, 4> mCornerRadius;
};
}

