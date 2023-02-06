#pragma once

#include <QObject>
#include <QUrl>

#include "../shapes/shapes.h"
#include "container.h"

namespace veqtor::elements {
class link final: public container {
    Q_OBJECT
public:
    enum Target {Blank};
    Q_ENUM(Target)
    explicit link(QObject *parent = nullptr): container{parent} {}

    Type type() const override { return Type::Link; }
signals:
private:
    QUrl mHref;
    Target mTarget;
};
}
