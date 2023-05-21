#include "painthelper.h"
#include "utils/svgtools.h"

namespace veqtor::canvas {
paintHelper::paintHelper() {}

void paintHelper::drawShape(QNanoPainter *painter,
                            const std::shared_ptr<shapes::shape> &shape,
                            const core::nanoPen &pen, const QTransform &rootTransform) {
    if(shape && !shape->isNull() && pen.visible()) {
        if(shape->type()) {
            pen.setToPainter(painter);
            painter->resetTransform();
            painter->transform(shape->transformer() * rootTransform);
            painter->beginPath();
        }

        switch(shape->type()) {
        case shapes::Path:
            drawPath(painter, std::dynamic_pointer_cast<shapes::path>(shape));
            break;
        case shapes::Line:
            drawLine(painter, std::dynamic_pointer_cast<shapes::line>(shape));
            break;
        case shapes::Ellipse:
            drawEllipse(painter, std::dynamic_pointer_cast<shapes::ellipse>(shape));
            break;
        case shapes::Rect:
            drawRect(painter, std::dynamic_pointer_cast<shapes::rect>(shape));
            break;
        case shapes::Polygon:
        case shapes::Shape:
        default:
            break;
        }

        if(shape->type()) {
            pen.mFill ? painter->fill() : void();
            pen.mStroke ? painter->stroke() : void();
        }
    }
}

void paintHelper::drawPath(QNanoPainter *painter, const std::shared_ptr<shapes::path> &path) {
    using path_data = shapes::pathdata;
    /// @brief "current from", "last to" and "last cubic control" points
    apoint from{}, lto{}, alcc{};

    /// Add a moveTo at the beginning of the path if it doesn't start with one.
    if(!path->empty() && !path->front().isMove()) {
        painter->moveTo(path->front().to);
    }

    for(const path_data &p: *path) {
        /// @brief Convert "p.to" point to "absolute to"
        apoint add = p.relative ? from : apoint{};
        apoint ato = p.to + add;

        switch(p.type()) {
            case path_data::Close: painter->closePath(); break;
            case path_data::Move: painter->moveTo(ato); lto = ato; break;
            case path_data::Line: painter->lineTo(ato); break;
            case path_data::Hr: ato.setY(from.y()); painter->lineTo(ato); break;
            case path_data::Vr: ato.setX(from.x()); painter->lineTo(ato); break;
            case path_data::Quad: {
                apoint c = p.quad().control;
                painter->quadTo(c + add, ato);
                break;
            }
            case shapes::pathdata::ShortQuad: {
                break;
            }
            case path_data::Cubic: {
                apoint c1 = p.cubic().c1, c2 = p.cubic().c2;
                painter->bezierTo(c1 + add, c2 + add, ato);
                alcc = c2 + add;
                break;
            }
            case path_data::Arc: {
                auto cubics = utils::svgTools::arcToCubic(p.arc(), from, ato);
                for(const auto &cubic: cubics) {
                    auto _cubic = cubic.cubic();
                    painter->bezierTo(_cubic.c1, _cubic.c2, cubic.to);
                }
                break;
            }
            case path_data::ShortCubic: {
                /// Reflection of last point to "from" point.
                apoint ac1 = alcc.isNull() ? from : 2 * from - alcc,
                       c2 = p.scubic().control;
                painter->bezierTo(ac1, c2 + add, ato);
                alcc = c2 + add;
                break;
            }
        }
        from = (!p.isClose() ? ato : lto);
        alcc = p.isCubic() || p.isShortCubic() ? alcc : apoint{0,0};
    }
}

void paintHelper::drawRect(QNanoPainter *painter, const std::shared_ptr<shapes::rect> &rect) {
    drawRect(painter, *rect);
}

void paintHelper::drawRect(QNanoPainter *painter, const QRectF &rect) {
    painter->beginPath();
    painter->rect(rect);
}

void paintHelper::drawEllipse(QNanoPainter *painter, const std::shared_ptr<shapes::ellipse> &ellipse) {
    painter->ellipse(*ellipse);
}

void paintHelper::drawLine(QNanoPainter *painter, const std::shared_ptr<shapes::line> &line) {
    drawLine(painter, *line);
}

void paintHelper::drawLine(QNanoPainter *painter, const QLineF &line) {
    painter->moveTo(line.p1());
    painter->lineTo(line.p2());
}

void paintHelper::drawPoint(QNanoPainter *painter, const QPointF &point) {
    /// @brief draw two crossing lines for single point.
    QLineF vLine(point - QPointF(2, 2), point + QPointF(2, 2));
    QLineF hLine(point - QPointF(2,-2), point + QPointF(2,-2));
    drawLine(painter, vLine);
    drawLine(painter, hLine);
}
}
