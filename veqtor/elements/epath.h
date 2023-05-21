#pragma once

#include <QObject>
#include <QHash>
#include <QQmlListProperty>

#include <memory>

#include "../shapes/shapes.h"
#include "graphic.h"

namespace veqtor::elements {
class epath: public graphic {
    Q_OBJECT
    Q_PROPERTY(QString d READ data WRITE setData NOTIFY dataChanged)
public:
    epath(const QMap<QString, QString>& attrs, QObject* parent = nullptr, const shapes::path& p = shapes::path());
    epath(const QString& d, QObject* parent = nullptr);
    epath(const shapes::path& pathObject, QObject* parent = nullptr);
    epath(QObject* parent = nullptr): epath{"", parent} {}

    std::shared_ptr<shapes::path> pathShape();
    std::shared_ptr<shapes::path> pathShape() const;
    QString data() const { return mData; }
    Type type() const override { return Type::Path; }

    void setData(const QString &d);
    void setAttributes(const QVariantMap &attrs) override;

    Q_INVOKABLE long long size() const { return pathShape()->size(); }
    Q_INVOKABLE QVariantMap at(long long index) const;
    Q_INVOKABLE QVariantMap shift();

public slots:
    void close();
    void vTo(qreal y, bool rel = false);
    void hTo(qreal x, bool rel = false);
    void moveTo(QPointF to, bool rel = false);
    void lineTo(QPointF to, bool rel = false);
    void quadTo(const QPointF &c, const QPointF &to, bool rel = false);
    void cubicTo(const QPointF &c1, const QPointF &c2, const QPointF &to, bool rel = false);
    void arcTo(QPointF to, QSizeF radius, qreal xrot, bool larc, bool sweep, bool rel = false);

    void pop();
    void clear() {
        pathShape()->clear();
        emit updated();
    }

    /**
     * @abstract push new node to path.
     * @param type: Node type, which can have possible values such as h, v, m, l, q, t, c, s, a, or z.
     * @param to: The target point.
     * @param relative: A boolean value.
     * @param data: Extra data for the path node.
     * @list
     * @li q: point{x1 y1},
     * @li c: point{x1 y1}, point{x2 y2}
     * @li a: size{rx ry}, x-axis-rotation: `real`, large-arc-flag: `bool`, sweep-flag: `bool`
     * @endlist
     */
    void push(char type, const QPointF &to, const QVariantMap &data, bool relative = false);

    /**
     * @abstract Set a new value for the path data at the given index.
     * @param index, Index of the path data in the vector. If the given value is out of range, nothing will change.
     * @param data, Path data value in the form of a structure {type: `char`, to: `point`, data: `Object`, relative: `bool` }.
     * @list
     * @li to: point{x: `real`, y: `real` }
     * @li relative: `true`|`false`
     * @li type: h|v|m|l|q|t|c|s|a|z
     * @li data:
     *      - q: point{x1 y1},
     *      - c: point{x1 y1}, point{x2 y2}
     *      - a: size{rx ry}, x-axis-rotation: `real`, large-arc-flag: `bool`, sweep-flag: `bool`
     * @endlist
     */
    void set(long long index, const QVariantMap &data) {
        if(index < size()) {
            pathShape()->at(index) = shapes::pathdata(data);
            emit updated();
        }
    }
    void set(long long index, const QPointF &to) {
        if(index < size()) {
            pathShape()->at(index).to = to;
            emit updated();
        }
    }
    void set(const QList<QPointF> &pointSeries, long long start = 0,
             long long end = std::numeric_limits<long long>::max()) {
        for(size_t i = start; i < std::min<long long>({size(), end, pointSeries.size()}); ++i) {
            pathShape()->at(i).to = pointSeries[i];
        }
        if(start < size() && start < end) emit updated();
    }

private:
    static QStringList mainAttrs() { return {"d","path-length"}; }

signals:
    void dataChanged();
    void pointsChanged(size_t index);

private:
    QString mData;
};
}
