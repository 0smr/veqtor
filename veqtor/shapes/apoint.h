#pragma once

#include <QSurfaceFormat>
#include <QTransform>
#include <QLineF>
#include <QRectF>
#include <QCache>

#include <cmath>

namespace veqtor {
enum PointProxy {
    Center,
    TopLeft,
    Top,
    TopRight,
    Right,
    BottomRight,
    Bottom,
    BottomLeft,
    Left,
};

enum PointState {
    Inside,
    Edge,
    None
};

/**
 * @brief The apoint class
 * represent float precision anchor point.
 */
class apoint : public QPointF {
public:
    constexpr apoint(const QPointF& p = QPointF()): QPointF(p) {}
    constexpr apoint(qreal x, qreal y): QPointF(x, y) {}

    apoint &operator =(const QPointF &in) {
        setX(in.x());
        setY(in.y());
        return *this;
    }

    apoint plus(qreal xp, qreal yp) const { return apoint(x() + xp, y() + yp); }
    apoint sub(qreal xp, qreal yp) const { return apoint(x() - xp, y() - yp); }

    bool isBetween(const apoint &p1, const apoint &p2, qreal trolance = 0.5f) {
        const qreal distToPoint = QLineF(p1, *this).length() + QLineF(*this, p2).length();
        return std::abs(distToPoint - QLineF(p1, p2).length()) <= trolance;
    }

    apoint transformed(const QTransform &transform) const {
        return transform.map(*this);
    }

    const apoint &transform(const QTransform &transform) {
        return (*this = transformed(transform));
    }

    apoint &rotate(qreal angle, const apoint &origin = {0, 0}) {
        apoint::rotate(*this, angle, origin);
        return *this;
    }

    static apoint rotate(const apoint &p, qreal angle, const apoint &origin = {0, 0}, Qt::Axis axis = Qt::ZAxis) {
        if(axis == Qt::ZAxis) {
            const QTransform transform =
                QTransform::fromTranslate(-origin.x(), -origin.y())
                    .rotate(angle)
                    .translate(origin.x(), origin.y());
            return transform.map(p);
        } else {
            return QTransform().rotate(angle, axis).rotate(angle).map(p);
        }
    }
};

inline QTransform operator +(const QTransform &a, const QTransform &b) {
    return {
        a.m11() * b.m11(), a.m12() + b.m12(), a.m13() + b.m13(),
        a.m21() + b.m21(), a.m22() * b.m22(), a.m23() + b.m23(),
        a.m31() + b.m31(), a.m32() + b.m32(), a.m33() * b.m33(),
    };
}
}
