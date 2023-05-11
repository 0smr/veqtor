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
     * @abstract This function converts an SVG arc curve to a list of cubic curves.
     * @param arc
     * @return list of cubic curves
     */
    static std::vector<shapes::pathdata> arcToCubic(const shapes::pd::arc &arc, const QPointF from, const QPointF &to);

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

    /**
     * @abstract Convert an SVG hex color that contains an alpha value to a hex color that is compatible with Qt.
     * #abcd => #ddaabbcc
     * #aabbccdd => #ddaabbcc
     * @param color, SVG color hex.
     * @return color, Qt color hex.
     */
    static std::string svgToQtColor(const std::string &color) {
        std::string c = color.substr(1);
        if(c.size() == 8) { return "#" + c.substr(6) + c.substr(0, 6); }
        if(c.size() == 4) {
            return {'#', c[3], c[3], c[0], c[0], c[1], c[1], c[2], c[2]};
        }
        return color;
    }
    static QString svgToQtColor(const QString &color) {
        return QString::fromStdString(svgToQtColor(color.toStdString()));
    }

    static QColor normColor(const QString &color) {
        QString _color = color.startsWith('#') ? svgToQtColor(color) : "";
        return _color.length() && _color.toLower() != "none" ? QColor(_color) : QColor(Qt::transparent);
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

    /**
     * @abstract This function parses a viewBox string in SVG format to a QRectF object.
     * @param viewBox, viewBox string in SVG format
     * @return viewBox as QRectF
     */
    static QRectF parseViewBox(const QString &viewBox);
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
