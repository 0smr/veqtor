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

    Q_INVOKABLE QVariantMap at(unsigned long long i) const;
    Q_INVOKABLE QVariantMap shift();

public slots:
    void close();
    void vTo(qreal y, bool rel = false);
    void hTo(qreal x, bool rel = false);
    void moveTo(QPointF to, bool rel = false);
    void lineTo(QPointF to, bool rel = false);
    void qubicTo(const QPointF &c, const QPointF &to, bool rel = false);
    void curveTo(const QPointF &c1, const QPointF &c2, const QPointF &to, bool rel = false);
    void arcTo(QPointF to, QSizeF radius, qreal xrot, bool larc, bool sweep, bool rel = false);

    void pop();
    void push(char type, const QPointF &to, const QVariantMap &data, bool relative = false);

private:
    static QStringList mainAttrs() { return {"d","path-length"}; }

signals:
    void dataChanged();

private:
    QString mData;
};
}
