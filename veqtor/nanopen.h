#pragma once

#include <QGradient>
#include <QPen>

#include <memory>

#include "qnanopainter.h"

namespace veqtor::core {
/**
 * @brief The nanoPen struct
 */
struct nanoPen {
    nanoPen(QRgb fill = 0x0, QRgb stroke = 0x0, float width = 1.0,
            QNanoPainter::LineCap cap = QNanoPainter::CAP_ROUND,
            QNanoPainter::LineJoin join = QNanoPainter::JOIN_ROUND)
        : mWinding(QNanoPainter::WINDING_CW), mCap(cap), mJoin(join),
          mWidth(width), mFill(fill), mStroke(stroke) {}
    nanoPen(QColor fill, QColor stroke, float width)
        : nanoPen(fill.rgba(), stroke.rgba(), width) {}

    void  setToPainter(QNanoPainter *painter) const {
        /**
         * TODO: add pattern as style.
         */
        painter->setFillStyle(mFill);
        painter->setStrokeStyle(mStroke);
        painter->setMiterLimit(mMiter);
        painter->setLineWidth(mWidth);
        painter->setLineCap(mCap);
        painter->setLineJoin(mJoin);
        painter->setPathWinding(mWinding);
    }
    void setJoin(const Qt::PenJoinStyle &join) { mJoin = fromQtJoin(join); }
    void setJoin(const QNanoPainter::LineJoin &joinStyle) { mJoin = joinStyle; }
    void setCap(const Qt::PenCapStyle &capStyle) { mCap = fromQtCap(capStyle); }
    void setCap(const QNanoPainter::LineCap &capStyle) { mCap = capStyle; }

    bool filled() const { return mFill; }
    bool stroke() const { return mStroke; }
    bool visible() const { return mFill || mStroke; }

    operator Qt::PenCapStyle() const { return nanoPen::toQtCap(mCap); }
    operator Qt::PenJoinStyle() const { return nanoPen::toQtJoin(mJoin); }

    static QNanoPainter::LineCap fromQtCap(Qt::PenCapStyle capStyle) {
        using LineCap = QNanoPainter::LineCap;
        switch(capStyle) {
            case Qt::FlatCap:   return LineCap::CAP_BUTT;
            case Qt::SquareCap: return LineCap::CAP_SQUARE;
            case Qt::RoundCap:  return LineCap::CAP_ROUND;
            default: return LineCap::CAP_BUTT;
        }
    }

    static Qt::PenCapStyle toQtCap(QNanoPainter::LineCap capStyle) {
        using LineCap = QNanoPainter::LineCap;
        switch(capStyle) {
            case LineCap::CAP_BUTT: return Qt::FlatCap;
            case LineCap::CAP_ROUND: return Qt::RoundCap;
            case LineCap::CAP_SQUARE: return Qt::SquareCap;
        }
    }

    static QNanoPainter::LineJoin fromQtJoin(Qt::PenJoinStyle joinStyle) {
        using LineJoin = QNanoPainter::LineJoin;
        switch(joinStyle) {
            case Qt::RoundJoin: return LineJoin::JOIN_ROUND;
            case Qt::BevelJoin: return LineJoin::JOIN_BEVEL;
            case Qt::MiterJoin: return LineJoin::JOIN_MITER;
            default: return LineJoin::JOIN_BEVEL;
        }
    }

    static Qt::PenJoinStyle toQtJoin(QNanoPainter::LineJoin joinStyle) {
        using LineJoin = QNanoPainter::LineJoin;
        switch(joinStyle) {
            case LineJoin::JOIN_ROUND: return Qt::RoundJoin;
            case LineJoin::JOIN_BEVEL: return Qt::BevelJoin;
            case LineJoin::JOIN_MITER: return Qt::MiterJoin;
        }
    }

    QNanoPainter::PathWinding mWinding : 2;
    QNanoPainter::LineCap mCap : 2;
    QNanoPainter::LineJoin mJoin : 3;
    QNanoPainter::TextAlign mTextAlign : 3;
    QNanoPainter::TextBaseline mTextBaseLine : 7;
    QNanoPainter::PixelAlign mPixelAlign : 2;
    QNanoPainter::CompositeOperation mComposite : 4;
    QNanoPainter::BlendFactor mBlend : 11;

    float mWidth = 1.0f;
    float mMiter = 10.0f;
    QRgb mFill = 0x00000000; /// #AARRGGBB (Qt::transparent)
    QRgb mStroke = 0x00000000; /// #AARRGGBB (Qt::transparent)

    std::shared_ptr<QGradient> mGradient;
};
}
