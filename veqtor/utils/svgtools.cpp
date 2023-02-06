#include "svgtools.h"
#include "tools.h"

#include "../shapes/shapes.h"

#include "../elements/element.h"
#include "../elements/graphic.h"
#include "../elements/epath.h"
#include "../elements/link.h"
#include "../elements/svg.h"
#include "../elements/unknown.h"

namespace veqtor::utils {
QMap<QString, QString> svgTools::getAttrs(const QDomNode &node) {
    QMap<QString, QString> attrMap;
    QDomNamedNodeMap attrs = node.attributes();
    for(int i = 0; i < attrs.count(); ++i) {
        const auto &attr = attrs.item(i).toAttr();
        attrMap[attr.name()] = attr.value();
    }
    return attrMap;
}

std::vector<shapes::pathdata> svgTools::svgPathParser(const QString &svgPath) {
    return svgPathParser(svgPath.toStdString());
}

std::vector<shapes::pathdata> svgTools::svgPathParser(const std::string &svgPath) {
    static const std::regex reg(R"([a-zA-Z][^a-zA-Z]*)");
    static const std::regex subReg(R"(-?\d*\.?\d*)");

    shapes::path pathShape;

    for(const auto &m: tools::globalMatch(svgPath, reg)) {
        auto a = tools::stodVec(tools::globalMatch(m, subReg));
        bool relative = std::islower(m[0]);
        char type = std::tolower(m[0]);

        switch (type) {
            case 'h': pathShape.hTo(a[0], relative); break;
            case 'v': pathShape.vTo(a[0], relative); break;
            case 'm': pathShape.moveTo(a, relative); break;
            case 'l': pathShape.lineTo(a, relative); break;
            case 'q': pathShape.qubicTo({a[0], a[1]}, {a[2], a[3]}, relative); break;
            case 't': qWarning("Veqtor presently does not support the `t` path line type."); break;
            case 'c': pathShape.curveTo({a[0], a[1]}, {a[2], a[3]}, {a[4], a[5]}, relative); break;
            case 's': qWarning("Veqtor presently does not support the `s` path line type."); break;
            case 'a': qWarning("Veqtor presently does not support the `a` path line type."); break;
            case 'z': pathShape.close(); break;
        }
    }

    return pathShape.pathData();
}

QPointer<element> svgTools::elementGenerator(element::Type type, const QMap<QString, QString> &attrs, QObject *parent) {
    using namespace elements;

    switch(type) {
        case element::Link: return new link(parent);
        case element::Path: return new epath(attrs, parent);
        case element::Rect:
        case element::Circle:
        case element::Ellipse:
        case element::Polyline:
        case element::Polygon:
        case element::Text:
        case element::TextPath: return new graphic(parent);
        case element::SVG: return new svg(attrs, parent);
        case element::Group:
        default: return new unknown(parent);
    };
    return nullptr;
}

QPointer<element> svgTools::domToElement(const QDomNode &node, QObject *parent) {
    QString tagname = node.toElement().tagName();
    element::Type type = mElementTypeMap[tagname];
    QPointer<element> veqtorElement = elementGenerator(type, getAttrs(node), parent);

    if(veqtorElement && type && type > element::Container) {
        auto cont = dynamic_cast<elements::container*>(veqtorElement.data());
        for(auto n = node.firstChild(); cont && !n.isNull(); n = n.nextSibling()) {
            cont->push_back(domToElement(n, veqtorElement));
        }
    }

    if(type) {
        /// TODO: There are elements such as <title> that may be placed into graphical elements too.
        ///  https://developer.mozilla.org/en-US/docs/Web/SVG/Element/title
    }

    return veqtorElement;
}

QPointer<element> svgTools::svgParser(const QString &svgString, QObject *parent) {
    if(svgString.isEmpty()) return nullptr;

    QDomDocument document;
    document.setContent(svgString);
    return domToElement(document.firstChild(), parent);
}

QRectF svgTools::parseViewBox(const QString &viewBox) {
    if(viewBox.isNull()) return QRectF();
    static const std::regex reg(R"(-?\d*\.?\d*(px)?)");
    auto m = tools::stodVec(tools::globalMatch(viewBox.toStdString(), reg));
    m.resize(4, 0.0f);
    // viewbox = "x, y, width, height"
    return QRectF(m[0], m[1], m[2], m[3]);
}
}
