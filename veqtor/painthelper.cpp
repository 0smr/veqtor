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
            painter->transform(rootTransform * shape->transformer());
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

using path_data = shapes::pathdata;
void paintHelper::drawPath(QNanoPainter *painter, const std::shared_ptr<shapes::path> &path) {
    /// @brief "current from" and "last to" points
    apoint from{0, 0}, lto{0, 0};

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
                painter->quadTo(c, ato);
                break;
            }
            case path_data::Cubic: {
                apoint c1 = p.cubic().c1, c2 = p.cubic().c2;
                painter->bezierTo(c1 + add, c2 + add, ato);
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
        }
        from = (p.type() != path_data::Close ? ato : lto);
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
