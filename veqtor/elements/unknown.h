#pragma once

#include <QObject>
#include <QUrl>

#include "../shapes/shapes.h"
#include "element.h"

namespace veqtor::elements {
class unknown final: public element {
    Q_OBJECT
public:
    explicit unknown(QObject *parent = nullptr): element{parent} {}
    Type type() const override { return Type::Unknown; }
    bool contains(const QPointF &) const override { return false; }
signals:
private:
};
}
