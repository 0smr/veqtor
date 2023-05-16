#pragma once

#include <limits>
#include <algorithm>

#include "apoint.h"
#include "shape.h"

namespace veqtor::shapes {
namespace pd {
    struct move  { };
    struct line  { };
    struct hr    { };
    struct vr    { };
    struct quad  { apoint control; };
    struct tquad { apoint control; };
    struct cubic { apoint c1, c2; };
    struct scubic{ apoint control; };
    struct arc   { QSizeF radius; qreal rotation; bool largeArc, sweepFlag; };
    struct close { };
}  // namespace pd

struct pathdata {
    using path_data =
        std::variant<pd::close, pd::move, pd::line, pd::vr, pd::hr, pd::arc,
                     pd::quad, pd::tquad, pd::cubic, pd::scubic>;
    using real_limit = std::numeric_limits<qreal>;
    enum Type {Close, Move, Line, Vr, Hr, Arc, Quad, ShortQuad, Cubic, ShortCubic};

    pathdata(const pd::close& data): relative{}, to{}, data{data} {}
    pathdata(const apoint& point, const path_data& data, bool relative = false)
        : relative{relative}, to{point}, data{data} {}
    pathdata(char type, const apoint& point, const QVariantMap& vdata, bool relative = false)
        : relative{relative}, to{point} {
        switch(std::tolower(type)) {
        case 'm': data = pd::move{}; break;
        case 'l': data = pd::line{}; break;
        case 'v': data = pd::vr{}; break;
        case 'h': data = pd::hr{}; break;
        case 'q': data = pd::quad{vdata["control"].toPointF()}; break;
        case 't': data = pd::tquad{}; break;
        case 'c': data = pd::cubic{vdata["control1"].toPointF(), vdata["control2"].toPointF()}; break;
        case 's': data = pd::scubic{vdata["control"].toPointF()}; break;
        case 'a': data = pd::arc{vdata["radius"].toSizeF(), vdata["rotation"].toDouble(),
                                 vdata["largeArc"].toBool(), vdata["sweepFlag"].toBool()}; break;
        case 'z': default: data = pd::close{};
        }
    }
    pathdata(const QVariantMap &map)
        : pathdata(map["type"].toChar().toLatin1(), map["to"].toPointF(),
                   map["data"].toMap(), map["relative"].toBool()) {}

    Type type() const { return static_cast<Type>(data.index()); }
    char typeChar() const { return "zmlvhaqc"[data.index()]; }

    bool isVr() const { return data.index() == Vr; }
    bool isHr() const { return data.index() == Hr; }
    bool isMove() const { return data.index() == Move; }
    bool isLine() const { return data.index() == Line; }
    bool isArc()  const { return data.index() == Arc; }
    bool isQuad() const { return data.index() == Quad; }
    bool isCubic() const { return data.index() == Cubic; }
    bool isShortQuad() const { return data.index() == ShortQuad; }
    bool isShortCubic() const { return data.index() == ShortCubic; }
    bool isClose() const { return data.index() == Close; }

    pd::scubic scubic() const { return std::get<pd::scubic>(data); }
    pd::scubic &scubic() { return std::get<pd::scubic>(data); }
    pd::cubic cubic() const { return std::get<pd::cubic>(data); }
    pd::cubic &cubic() { return std::get<pd::cubic>(data); }
    pd::tquad tquad() const { return std::get<pd::tquad>(data); }
    pd::tquad &tquad() { return std::get<pd::tquad>(data); }
    pd::quad quad() const { return std::get<pd::quad>(data); }
    pd::quad &quad() { return std::get<pd::quad>(data); }
    pd::arc arc() const { return std::get<pd::arc>(data); }
    pd::arc &arc() { return std::get<pd::arc>(data); }

    QVariantMap map() const {
        QVariantMap value{{"relative", relative},{"to", to}};
        value["type"] = QChar("vhmlaqcz"[data.index()]);
        if(isCubic()) { value["control"] = QVariantList{{cubic().c1, cubic().c2}}; }
        else if(isQuad()) { value["control"] = quad().control; }
        else if(isArc()) {
            value.insert({
                {"radius",   arc().radius   },
                {"rotation", arc().rotation },
                {"sweep",    arc().sweepFlag},
                {"largeArc", arc().largeArc },
            });
        }
        return value;
    }

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

    void pop() { mPathData.pop_back(); }
    void leftShift() {
        std::rotate(mPathData.begin(), mPathData.begin() + 1, mPathData.end());
        mPathData.resize(mPathData.size() - 1, pd::close{});
    }

    /**
     * @brief applyTransform
     * Sets angle to zero, and changes points and thier controls depend on shape rotation.
     * NOTE: only path can reset it's bounding box (other shapes can't).
     */
    void applyTransform() {
        if(!transformer().isIdentity()) {
            for(pathdata& pdata : mPathData) {
                pdata.to.transform(transformer());
                if(pdata.type() == pathdata::Cubic) {
                    std::get<pd::cubic>(pdata.data).c1.transform(transformer());
                    std::get<pd::cubic>(pdata.data).c2.transform(transformer());
                } else if(pdata.type() == pathdata::Quad) {
                    std::get<pd::quad>(pdata.data).control.transform(transformer());
                }
            }
        }
        setTransform(QTransform());
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
    void push(char type, QPointF to, const QVariantMap &data, bool relative = false) {
        mPathData.push_back({type, to, data, relative});
    }

    void vTo(qreal y, bool relative = false);
    void hTo(qreal x, bool relative = false);
    void moveTo(std::vector<double> points, bool relative = false);
    void moveTo(apoint to, bool relative = false);
    void lineTo(const std::vector<double> &points, bool relative = false);
    void lineTo(apoint to, bool relative = false);
    void quadTo(const apoint &control, const apoint &to, bool relative = false);
    void shortQuadTo(const apoint &to, bool relative = false) {

    }
    void cubicTo(const apoint &c1, const apoint &c2, const apoint &to, bool relative = false);
    void cubicTo(const std::vector<double> &v, bool relative = false);
    void shortCubicTo(const apoint &control, const apoint &to, bool relative = false) {
        mPathData.push_back({invertTransformer().map(to), pd::scubic{control}, relative});
        expandBoundigBox(to);
    }
    void arcTo(apoint to, QSizeF radius, qreal xrot, bool larc, bool sweep, bool relative = false);
    void arcTo(const std::vector<double> &v, bool relative = false);
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
