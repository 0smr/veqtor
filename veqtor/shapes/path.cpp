#include "path.h"

namespace veqtor::shapes {
PointState path::contains(const apoint &point) const {
    apoint ipoint = invertTransformer().map(point);
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
    mPathData.push_back({invertTransformer().map(apoint{0.0f, y}), pd::vr{}, relative});
    expandBoundigBox(apoint{0.0f, y});
}

void path::hTo(qreal x, bool relative) {
    mPathData.push_back({invertTransformer().map(apoint{x, 0.0f}), pd::hr{}, relative});
    expandBoundigBox(apoint{x, 0.0f});
}

void path::moveTo(std::vector<double> points, bool relative) {
    // `points.size() &~ 1` will round the vector size to a multiple of 2.
    points.resize(std::max(2ull, points.size() &~ 1), 0);
    moveTo(apoint{points[0], points[1]}, relative);
    for(size_t i{2}; i < points.size() - 1; i+=2) {
        lineTo(apoint{points[i], points[i+1]}, relative);
    }
}

void path::moveTo(apoint to, bool relative) {
    mPathData.push_back({invertTransformer().map(to), pd::move{}, relative});
    expandBoundigBox(to);
}

void path::lineTo(const std::vector<double> &points, bool relative) {
    // `points.size() &~ 1` will round the vector size to a multiple of 2.
    size_t size =  points.size() &~ 1;
    for(size_t i{}; i < size; i+=2) {
        lineTo(apoint{points[i], points[i+1]}, relative);
    }
}

void path::lineTo(apoint to, bool relative) {
    mPathData.push_back({invertTransformer().map(to), pd::line{}, relative});
    expandBoundigBox(to);
}

void path::quadTo(const apoint &control, const apoint &to, bool relative) {
    mPathData.push_back({invertTransformer().map(to),
                         pd::quad{invertTransformer().map(control)},
                         relative});
    expandBoundigBox(to);
}

void path::cubicTo(const apoint &c1, const apoint &c2, const apoint &to, bool relative) {
    mPathData.push_back({
        invertTransformer().map(to),
        pd::cubic{invertTransformer().map(c1), invertTransformer().map(c2)},
        relative
    });
    expandBoundigBox(to);
}

void path::cubicTo(const std::vector<double> &v, bool relative) {
    // Round the vector size to a multiple of 6.
    size_t size =  (v.size() / 6) * 6;
    for(size_t i{}; i < size; i+=6) {
        cubicTo({v[0+i], v[1+i]}, {v[2+i], v[3+i]}, {v[4+i], v[5+i]}, relative);
    }
}

void path::arcTo(apoint to, QSizeF radius, qreal xrot, bool larc, bool sweep, bool relative) {
    mPathData.push_back({
        invertTransformer().map(to), pd::arc{radius, xrot, larc, sweep},
        relative
    });
    expandBoundigBox(to);
}

void path::arcTo(const std::vector<double> &v, bool relative) {
    // Round the vector size to a multiple of 7.
    size_t size =  (v.size() / 7) * 7;
    for(size_t i{}; i < size; i+=7) {
        arcTo({v[5+i], v[6+i]}, {v[0+i], v[1+i]}, v[2+i], v[3+i], v[4+i], relative);
    }
}

void path::close() {
    mPathData.push_back(pd::close{});
}
}
