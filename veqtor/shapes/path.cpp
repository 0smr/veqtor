#include "path.h"

namespace veqtor::shapes {
PointState path::contains(const apoint &point) const {
    apoint ipoint = rTransform().map(point);
    if(mBoundingBox.contains(ipoint) == false || mPathData.size() < 2) {
        return PointState::None;
    } else {
        apoint last = mPathData.front().to;
        for(const pathdata& d : mPathData) {
            if(!d.isMove() && !d.isClose() && ipoint.isBetween(last, d.to)) {
                return PointState::Edge;
            }
            last = d.to;
        }

        if(mPathData.size() > 2) {
            QLineF hLine(ipoint.x(), ipoint.y(), ipoint.x(), boundingBox().right() + 1);
            apoint last = mPathData.front().to;
            size_t intersects = 0;

            for(const pathdata &d: mPathData) {
                if(hLine.intersects(QLineF(last, d.to), nullptr) == QLineF::BoundedIntersection) {
                    intersects++;
                }
                last = d.to;
            }
            return intersects % 2 == 1 ? PointState::Inside : PointState::None;
        }
        return PointState::None;
    }
}

void path::vTo(qreal y, bool relative) {
    mPathData.push_back({rTransform().map(apoint{0.0f, y}), pd::vr{}, relative});
    expandBoundigBox(apoint{0.0f, y});
}

void path::hTo(qreal x, bool relative) {
    mPathData.push_back({rTransform().map(apoint{x, 0.0f}), pd::hr{}, relative});
    expandBoundigBox(apoint{x, 0.0f});
}

void path::moveTo(std::vector<double> points, bool relative) {
    moveTo(apoint{points[0], points[1]}, relative);
    for(size_t i{2}; i< points.size(); i+=2) {
        lineTo(apoint{points[i], points[i+1]}, relative);
    }
}

void path::moveTo(apoint to, bool relative) {
    mPathData.push_back({rTransform().map(to), pd::move{}, relative});
    expandBoundigBox(to);
}

void path::lineTo(std::vector<double> points, bool relative) {
    for(size_t i{}; i< points.size(); i+=2) {
        lineTo(apoint{points[i], points[i+1]}, relative);
    }
}

void path::lineTo(apoint to, bool relative) {
    mPathData.push_back({rTransform().map(to), pd::line{}, relative});
    expandBoundigBox(to);
}

void path::qubicTo(const apoint &control, const apoint &to, bool relative) {
    mPathData.push_back({
                         rTransform().map(to),
                         pd::qubic{rTransform().map(control)},
        relative
    });
    expandBoundigBox(to);
}

void path::curveTo(const apoint &c1, const apoint &c2, const apoint &to, bool relative) {
    mPathData.push_back({
        rTransform().map(to),
        pd::curve{rTransform().map(c1), rTransform().map(c2)},
        relative
    });

    expandBoundigBox(to);
}

void path::arcTo(apoint to, QSizeF radius, qreal xrot, bool larc, bool sweep, bool relative) {
    mPathData.push_back({rTransform().map(to), pd::arc{ radius, xrot, larc, sweep}, relative});
    expandBoundigBox(to);
}

void path::close() {
    mPathData.push_back(pd::close{});
}
}
