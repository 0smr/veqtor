#pragma once

#include <QLine>
#include <QPen>
#include <QPoint>
#include <QRect>

#include <array>
#include <cmath>
#include <vector>
#include <memory>

#include "nanopen.h"
#include "shapes/shapes.h"

namespace veqtor::canvas {
class paintHelper {
public:
    paintHelper();

    /**
     * @param painter
     * @param shape
     * @param pen
     * @brief drawShape
     * Draw shapes based on their types.
     */
    static void drawShape(QNanoPainter *painter,
                          const std::shared_ptr<shapes::shape> &shape,
                          const core::nanoPen &pen,
                          const QTransform &rootTransform = QTransform());

    /**
     * @param painter
     * @param path
     * @brief drawPath
     * Draw path shape.
     */
    static void drawPath(QNanoPainter *painter, const std::shared_ptr<shapes::path> &path);

    /**
     * @param painter
     * @param rect
     * @brief drawRect
     * Draw a rectangle.
     */
    static void drawRect(QNanoPainter *painter, const std::shared_ptr<shapes::rect> &rect);

    /**
     * @param painter
     * @param rect
     * @brief drawRect
     * Draw given QRectF.
     */
    static void drawRect(QNanoPainter *painter, const QRectF &rect);

    /**
     * @param painter
     * @param ellipse
     * @brief drawEllipse
     * Draw given shapes::ellipse pointer.
     */
    static void drawEllipse(QNanoPainter *painter, const std::shared_ptr<shapes::ellipse> &ellipse);

    /**
     * @param painter
     * @param line
     * @brief drawLine
     * Draw given shapes::line pointer.
     */
    static void drawLine(QNanoPainter *painter, const std::shared_ptr<shapes::line> &line);

    /**
     * @param painter
     * @param line
     * @brief drawLine
     * Draw given shapes::line pointer.
     */
    static void drawLine(QNanoPainter *painter, const QLineF &line);

    /**
     * @param painter
     * @param point
     * @brief drawPoint
     * Draw given QPointF.
     * Two crossing lines for single point.
     */
    static void drawPoint(QNanoPainter *painter, const QPointF &point);
};
}
