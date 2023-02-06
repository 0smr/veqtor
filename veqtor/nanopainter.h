#pragma once

#include "qnanoquickitempainter.h"
#include "qnanopainter.h"

#include "veqtor.h"

namespace veqtor::core {
class veqtor;

class nanoPainter : public QNanoQuickItemPainter {
public:
    nanoPainter();
    void paint(QNanoPainter *p) override;
    void setCanvas(const canvas::veqtor* canvas);

private:
    const canvas::veqtor* mCanvas;
};
}
