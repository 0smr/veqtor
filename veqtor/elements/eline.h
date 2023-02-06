#pragma once

#include <QObject>

#include <memory>

#include "../shapes/shapes.h"
#include "graphic.h"

namespace veqtor::elements {
class eline final: public graphic {
    Q_OBJECT
public:
    explicit eline(QObject *parent = nullptr): graphic{parent} {
        mShape = std::make_shared<shapes::line>();
    }

    std::shared_ptr<shapes::line> lineShape() { return std::dynamic_pointer_cast<shapes::line>(mShape); }
    Type type() const override { return Type::Line; }
    void setAttributes(const QVariantMap &attrs) override { Q_UNUSED(attrs); }
signals:
private:
};
}
