// Copyright (C) 2022 smr.
// SPDX-License-Identifier: MIT
// https://smr.best
// A C++/Qt implementation of the SVG arc to cubic curve converter, based on the svgpath repository.
// svgpath: https://github.com/fontello/svgpath/blob/master/lib/a2c.js.

#include <QPointF>
#include <QSizeF>
#include <QVector>
#include <cmath>

namespace veqtor::utils {
struct arcTool {
    struct cubicCurve { QPointF to, c1, c2; };
    static int sign(qreal v) { return std::signbit(v) ? -1 : 1; }

    static QPointF mapToEllipse(const QPointF &point, const QPointF &radius,
                         qreal cosphi, qreal sinphi, const QPointF &center) {
        qreal x = point.x() * radius.x(), y = point.y() * radius.y();
        return QPointF{cosphi * x - sinphi * y + center.x(),
                       sinphi * x + cosphi * y + center.y()};
    }

    static QVector<QPointF> approxUnitArc(qreal ang1, qreal ang2) {
        // If 90 degree circular arc, use a constant
        // as derived from http://spencermortensen.com/articles/bezier-circle
        qreal a = qFuzzyCompare(std::abs(ang2), 1.5707963267948966)
                      ? arcTool::sign(ang2) * 0.551915024494
                      : 4 / 3 * tan(ang2 / 4);
        qreal x1 = cos(ang1), y1 = sin(ang1);
        qreal x2 = cos(ang1 + ang2), y2 = sin(ang1 + ang2);
        return {
            {x1 - y1 * a, y1 + x1 * a},
            {x2 + y2 * a, y2 - x2 * a},
            {x2,          y2         }
        };
    }

    static qreal vectorAngle(QPointF u, QPointF v) {
        qreal sign = (u.x() * v.y() - u.y() * v.x() < 0) ? -1.0 : 1.0;
        qreal dot = std::clamp(QPointF::dotProduct(u, v), -1.0, 1.0);
        return sign * acos(dot);
    }

    static QVector<double> getArcCenter(QPointF from, QPointF to, QPointF radius,
                                 bool largeArcFlag, bool sweepFlag,
                                 double sinphi, double cosphi, QPointF pp) {
        const double TAU = M_PI * 2;
        const double rxsq = radius.x() * radius.x();
        const double rysq = radius.y() * radius.y();
        const double pxpsq = pp.x() * pp.x();
        const double pypsq = pp.y() * pp.y();

        double radicant = (rxsq * rysq) - (rxsq * pypsq) - (rysq * pxpsq);

        if(radicant < 0) { radicant = 0; }

        radicant /= (rxsq * pypsq) + (rysq * pxpsq);
        radicant = std::sqrt(radicant) * (largeArcFlag == sweepFlag ? -1 : 1);

        const QPointF ctrp{radicant * radius.x() / radius.y() * pp.y(),
                           radicant * -radius.y() / radius.x() * pp.x()};

        const QPointF center{cosphi * ctrp.x() - sinphi * ctrp.y() +
                                 (from.x() + to.x()) / 2,
                             sinphi * ctrp.x() + cosphi * ctrp.y() +
                                 (from.y() + to.y()) / 2};

        QPointF v1{(pp.x() - ctrp.x()) / radius.x(),
                   (pp.y() - ctrp.y()) / radius.y()},
            v2{(-pp.x() - ctrp.x()) / radius.x(),
               (-pp.y() - ctrp.y()) / radius.y()};

        double ang1 = vectorAngle({1, 0}, v1);
        double ang2 = vectorAngle(v1, v2);

        if(sweepFlag == 0 && ang2 > 0) { ang2 -= TAU; }
        if(sweepFlag == 1 && ang2 < 0) { ang2 += TAU; }

        return {center.x(), center.y(), ang1, ang2};
    }

    static QVector<cubicCurve> arcToCubic(QPointF from, QPointF to,
                                          QSizeF radius, double rotation,
                                          bool largeArcFlag, bool sweepFlag) {
        QPointF rad{radius.width(), radius.height()};
        // 2 * PI or 2π is colloquially referred to tau or τ
        constexpr double tau = M_PI * 2;
        double phi = rotation * tau / 360;

        if(rad.x() == 0 || rad.y() == 0) { return {}; }

        const double sinphi = sin(phi);
        const double cosphi = cos(phi);

        const double pxp = cosphi * (from.x() - to.x()) / 2 + sinphi * (from.y() - to.y()) / 2;
        const double pyp = -sinphi * (from.x() - to.x()) / 2 +
                           cosphi * (from.y() - to.y()) / 2;

        if(pxp == 0 && pyp == 0) { return {}; }

        rad = QPointF{abs(rad.x()), abs(rad.y())};

        double lambda = (pxp * pxp) / (rad.x() * rad.x()) +
                        (pyp * pyp) / (rad.y() * rad.y());

        if(lambda > 1) rad *= std::sqrt(lambda);

        QVector<double> centerAngles =
            getArcCenter(from, to, rad, largeArcFlag, sweepFlag, sinphi,
                         cosphi, {pxp, pyp});

        QPointF center{centerAngles[0], centerAngles[1]};
        double ang1 = centerAngles[2];
        double ang2 = centerAngles[3];

        // If 'ang2' == 90.0000000001, then `ratio` will evaluate to 1.0000000001.
        // This causes `segments` to be greater than one, which is an unecessary split,
        // and adds extra points to the bezier curve. To alleviate this issue,
        // we round to 1.0 when the ratio is close to 1.0.
        double ratio = abs(ang2) / (tau / 4);
        if(std::abs(1.0 - ratio) < 0.0000001) { ratio = 1.0; }

        int segments = std::max(int(ceil(ratio)), 1);

        ang2 /= segments;

        QVector<QVector<QPointF>> curves;
        QVector<cubicCurve> results;

        for(int i = 0; i < segments; i++) {
            curves.push_back(approxUnitArc(ang1, ang2));
            ang1 += ang2;
        }

        QPointF last = from;

        for(auto curve: curves) {
            QPointF c1 = mapToEllipse(curve[0], rad, cosphi, sinphi, center),
                c2 = mapToEllipse(curve[1], rad, cosphi, sinphi, center),
                to = mapToEllipse(curve[2], rad, cosphi, sinphi, center);
            results.push_back({to, c1, c2});
            last = to;
        }

        return results;
    }
};
} // namespace veqtor::utils
