#include "line.h"
#include "path.h"

namespace veqtor::shapes {
line::operator path() const {
    path path({{p1(), pd::move{}}, {p2(), pd::line{}}}, mPen);
    return path;
}
}
