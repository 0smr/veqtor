#include "nanopainter.h"

namespace veqtor::core {
nanoPainter::nanoPainter() {}

void nanoPainter::paint(QNanoPainter *p) {
    mCanvas->painter(p);
}

void nanoPainter::setCanvas(const canvas::veqtor *canvas) {
    mCanvas = canvas;
}
}
