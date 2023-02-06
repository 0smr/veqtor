#pragma once

#include <limits>

#include "apoint.h"
#include "shape.h"

namespace veqtor::shapes {
namespace pd {
    struct move  { };
    struct line  { };
    struct hr    { };
    struct vr    { };
    struct qubic { apoint control; };
    struct curve { apoint c1, c2; };
    struct arc   { QSizeF radius; qreal rotation; bool largeArc, sweepFlag; };
    struct close { };
}  // namespace pd

struct pathdata {
    using path_data = std::variant<pd::close, pd::move, pd::line, pd::vr, pd::hr, pd::arc, pd::qubic, pd::curve>;
    using real_limit = std::numeric_limits<qreal>;
    enum Type {Close, Move, Line, Vr, Hr, Arc, Qubic, Curve};

    pathdata(const pd::close& data): relative{}, to{}, data{data} {}
    pathdata(const apoint& point, const path_data& data, bool relative = false)
        : relative{relative}, to{point}, data{data} {}

    Type type() const { return static_cast<Type>(data.index()); }

    bool isVr() const { return data.index() == Vr; }
    bool isHr() const { return data.index() == Hr; }
    bool isMove() const { return data.index() == Move; }
    bool isLine() const { return data.index() == Line; }
    bool isArc()  const { return data.index() == Arc; }
    bool isQubic() const { return data.index() == Qubic; }
    bool isCurve() const { return data.index() == Curve; }
    bool isClose() const { return data.index() == Close; }

    pd::curve curve() const { return std::get<pd::curve>(data); }
    pd::qubic qubic() const { return std::get<pd::qubic>(data); }
    pd::arc arc() const { return std::get<pd::arc>(data); }

    bool relative;
    apoint to;
    path_data data;
};

/** @brief The path shape class */
class path : public shape {
public:
    path(const core::nanoPen &pen = core::nanoPen())
        : shape(pen), mPathData({}) {}
    path(const std::vector<pathdata>& pdata, const core::nanoPen &pen = core::nanoPen())
        : shape(pen), mPathData(pdata) {}

    pathdata &operator[](size_t index) { return mPathData.at(index); }
    const pathdata &operator[](size_t index) const { return mPathData.at(index); }

    pathdata &at(size_t index) { return mPathData.at(index); }
    const pathdata &at(size_t index) const { return mPathData.at(index); }

    std::vector<pathdata>::iterator begin() { return mPathData.begin(); }
    std::vector<pathdata>::const_iterator cbegin() const { return mPathData.cbegin(); }
    std::vector<pathdata>::reverse_iterator rbegin() { return mPathData.rbegin(); }
    std::vector<pathdata>::iterator end() { return mPathData.end(); }
    std::vector<pathdata>::const_iterator cend() const { return mPathData.cend(); }
    std::vector<pathdata>::reverse_iterator rend() { return mPathData.rend(); }
    std::vector<pathdata>::const_reference front() const { return mPathData.front();}
    std::vector<pathdata>::const_reference back() const { return mPathData.back();}

    const QRectF& updateBoundingBox() override {
        mBoundingBox = empty() ? QRectF{} : QRectF{front().to, QSizeF{}};
        for(pathdata& p : mPathData) {
            if(p.type() > pathdata::Move){
                expandBoundigBox(p.to);
            }
        }

        return mBoundingBox;
    }

    /**
     * @brief applyTransform
     * Sets angle to zero, and changes points and thier controls depend on shape rotation.
     * NOTE: only path can reset it's bounding box (other shapes can't).
     */
    void applyTransform() {
        if(mAngle != 0) {
            for(pathdata& pdata : mPathData) {
                pdata.to.transform(transform());
                if(pdata.type() == pathdata::Curve) {
                    std::get<pd::curve>(pdata.data).c1.transform(transform());
                    std::get<pd::curve>(pdata.data).c2.transform(transform());
                } else if(pdata.type() == pathdata::Qubic) {
                    std::get<pd::qubic>(pdata.data).control.transform(transform());
                }
            }
        }
        mAngle = 0;
        updateBoundingBox();
    }

    /** @brief isNull, return shape type */
    ShapeType type() const override { return ShapeType::Path; }
    /** @brief isNull, smae as mLineSeries.empty() */
    bool isNull() const override { return mPathData.empty(); }

    /**
     * TODO: Add following function:
     * void setBoundingBox(const QRectF &boundingBox) override {}
     */

    /**
     * @brief contains
     * check wheater if a point is inside a path or not.
     * if shape is not filled function only check the edges and anchors.
     * @param point
     * @return true if path contain point, otherwise return false.
     */
    PointState contains(const apoint &point) const override;

    void clear() { mPathData.clear(); }
    void push(const pathdata &l) { mPathData.push_back(l); }

    void vTo(qreal y, bool relative = false);
    void hTo(qreal x, bool relative = false);
    void moveTo(std::vector<double> points, bool relative = false);
    void moveTo(apoint to, bool relative = false);
    void lineTo(std::vector<double> points, bool relative = false);
    void lineTo(apoint to, bool relative = false);
    void qubicTo(const apoint &control, const apoint &to, bool relative = false);
    void curveTo(const apoint &c1, const apoint &c2, const apoint &to, bool relative = false);
    void arcTo(apoint to, QSizeF radius, qreal xrot, bool larc, bool sweep, bool relative = false);
    void close();

    /// setters
    void setPathData(const std::vector<pathdata> &pathData) {
        mPathData = pathData;
        updateBoundingBox();
    }

    /// getters
    bool singlePoint() const { return size() == 1; }
    bool empty() const { return mPathData.empty(); }
    const std::vector<pathdata>& pathData() const { return mPathData; }
    size_t size() const { return mPathData.size(); }

private:
    std::vector<pathdata> mPathData;
};
}
