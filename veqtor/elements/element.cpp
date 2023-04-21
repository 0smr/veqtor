#include <qDebug>

#include "element.h"
#include "container.h"
#include "graphic.h"
#include "../utils/csstools.h"

namespace veqtor::elements {
using utils::tools;
using utils::cssTools;

element::element(QObject *parent, QMap<QString, QString> attrs)
    : QObject{parent}, mId(attrs["id"]), mClass{attrs["class"].split(" ")},
    mStyle{cssTools::cssStyleParser(attrs["style"])}, mTabIndex{attrs["tab-index"].toLongLong()} {

    auto map = tools::filter(attrs, mainAttrs());
    for(const auto &key: map) {
        mAttributes.insert(key, attrs[key]);
    }
}

QString element::id() const { return mId; }

void element::setId(const QString &idValue) {
    if(mId == idValue) return;
    mId = idValue;

    emit idChanged();
    emit updated();
}

void element::setAttribute(const QString &key, const QString &value) {
    setAttributes({{key, value}});
}

void element::setAttributes(const QVariantMap &attrs) {
    /// TODO: Use a better method.
    if(attrs.contains("class")) mClass = attrs["class"].toStringList();
    if(attrs.contains("class")) mTabIndex = attrs["class"].toLongLong();
    if(attrs.contains("style")) {
        mStyle = utils::cssTools::cssStyleParser(attrs["style"].toString());
    }

    auto filtred = tools::filter(attrs, mainAttrs());

    for(auto i = filtred.keyValueBegin(); i != filtred.keyValueEnd(); i++) {
        if(i->second.isNull()) mAttributes.clear(i->first);
        else mAttributes.insert(i->first, i->second);
    }

    emit attributesChanged();
    emit updated();
}

QDebug &operator << (QDebug &debug, const element &el) {
    int index = std::clamp(el.type() % 0x100, 0, 11);
    QString names[2][11]{
        {"none", "graphic", "line", "path", "rect", "circle", "ellipse",
         "polyline", "polygon", "text", "textpath"},
        {"containter", "svg", "group", "link"}
    };
    return debug << "el::" << names[el.isContainer()][index] << "(" << el.mId << ")";
}

QDebug &operator<<(QDebug &debug, const QPointer<element> &el) {
    debug << *el;
    if(el->isContainer()) {
        const auto &cont = std::make_shared<container>(el);
        for(const auto &e: *cont) debug << e;
    } else if(el->isGraphic()) {
        const auto &g = std::make_shared<graphic>(el);
        debug << g->shape();
    }
    return debug;
}
}
