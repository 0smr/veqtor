#pragma once

#include <QDomDocument>
#include <QPointer>
#include <QString>
#include <QMap>

#include <memory>
#include <regex>

#include "../shapes/path.h"
#include "../elements/element.h"

namespace veqtor::utils {
using elements::element;

class svgTools {
public:
    explicit svgTools() {}

    /**
     * @list
     * @li h|H {x}
     * @li v|V {y}
     * @li m|M {x  y}
     * @li l|L {x  y}
     * @li q|Q {x1 y1, x y}
     * @li c|C {x1 y1, x2 y2, x y}
     * @li a|A {rx ry x-axis-rotation large-arc-flag sweep-flag x y}
     * @brief svgPathParser
     * @abstract Converts an svg path string to a `pathdata` vector.
     */
    static std::vector<shapes::pathdata> svgPathParser(const QString &svgPath);
    static std::vector<shapes::pathdata> svgPathParser(const std::string &svgPath);

    /**
     * @brief getAttrs
     * @param node
     * @return all element attributes as a QMap object.
     */
    static QMap<QString, QString> getAttrs(const QDomNode &node);

    /**
     * @brief elementGenerator
     * @abstract This method generates and returns a shared pointer element of the specified type.
     * @param type
     * @param attrs
     * @return created element as QPointer<element>
     */
    static QPointer<element> elementGenerator(element::Type type, const QMap<QString, QString> &attrs, QObject *parent = nullptr);

    /**
     * @brief domToElement
     * @param node
     * @return A shared pointer to the element tree's root.
     */
    static QPointer<element> domToElement(const QDomNode &node, QObject *parent = nullptr);

    /**
     * @brief svgParser
     * @param svgString
     * @return
     */
    static QPointer<element> svgParser(const QString &svgString, QObject *parent = nullptr);

    static QColor normColor(const QString &color) {
        return color.length() && color.toLower() != "none" ? QColor(color) : QColor(Qt::transparent);
    }

    /**
     * @brief normSW
     * @abstract Takes stroke-width as input and returns it as a float integer.
     *  if no input is provided, the default value is 1.0f.
     * @param width
     * @return return normal stromElementTypeMap     */
    static float normSW(const QString &width) {
        return width.isEmpty() ? 1.0f : width.toFloat();
    }

    static QRectF parseViewBox(const QString &viewBox);

    // {rx ry x-axis-rotation large-arc-flag sweep-flag x y}
    // void QNanoPainter::arcTo(float c1x, float c1y, float c2x, float c2y, float radius)
    // a c1x c1y r .0 .0 c2x c2y
    static std::tuple<QPointF, QPointF, qreal> toNanoArc(const shapes::pd::arc &arc) {
        Q_UNUSED(arc)
        return {};
    }

private:
    inline const static QMap<QString, element::Type> mElementTypeMap{
        {"circle",   element::Circle  },
        {"ellipse",  element::Ellipse },
        {"line",     element::Line    },
        {"path",     element::Path    },
        {"polygon",  element::Polygon },
        {"polyline", element::Polyline},
        {"rect",     element::Rect    },
        {"text",     element::Text    },
        {"textPath", element::TextPath},
        {"svg",      element::SVG     },
        {"a",        element::Link    },
        {"g",        element::Group   },
    };
};
}
