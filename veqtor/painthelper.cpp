#include "painthelper.h"

namespace veqtor::canvas {
paintHelper::paintHelper() {}

void paintHelper::drawShape(QNanoPainter *painter, const std::shared_ptr<shapes::shape> &shape, const core::nanoPen &pen) {
    if(shape && !shape->isNull() && pen.visible()) {
        if(shape->type()) {
            pen.setToPainter(painter);
            painter->resetTransform();
            painter->transform(globalTransform);
            painter->transform(shape->transform());
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
    apoint head{0, 0}, tail{0, 0};

    for(const path_data &p: *path) {
        apoint add = p.relative ? tail : apoint{};
        apoint to = p.to + add;

        switch(p.type()) {
            case path_data::Close: painter->closePath(); break;
            case path_data::Move: painter->moveTo(to); head = to; break;
            case path_data::Line: painter->lineTo(to); break;
            case path_data::Hr: to.setY(tail.y()); painter->lineTo(to); break;
            case path_data::Vr: to.setX(tail.x()); painter->lineTo(to); break;
            case path_data::Qubic: {
                apoint c = p.qubic().control;
                painter->quadTo(c, to);
                break;
            }
            case path_data::Curve: {
                apoint c1 = p.curve().c1, c2 = p.curve().c2;
                painter->bezierTo(c1 + add, c2 + add, to);
                break;
            }
            case path_data::Arc: {
                shapes::pd::arc a = p.arc();
                painter->arcTo(to.x(), to.y(), a.radius.width(), a.radius.height(), a.rotation);
                painter->ellipse(to.x(), to.y(), a.radius.width(), a.radius.height());
                break;
            }
        }
            tail = (p.type() != path_data::Close ? to : head);
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
