#pragma once

#include <QObject>
#include <QHash>

#include <memory>

#include "../shapes/shapes.h"
#include "graphic.h"

namespace veqtor::elements {
class epath final: public graphic {
    Q_OBJECT
    Q_PROPERTY(QString d READ data WRITE setData NOTIFY dataChanged)
    Q_PROPERTY(qreal PathLength READ PathLength WRITE setPathLength NOTIFY PathLengthChanged)
public:
    epath(const QMap<QString, QString>& attrs, QObject* parent = nullptr, const shapes::path& p = shapes::path());
    epath(const QString& d, QObject* parent = nullptr);
    epath(const shapes::path& pathObject, QObject* parent = nullptr);
    epath(QObject* parent = nullptr) : epath{"", parent} {}

    std::shared_ptr<shapes::path> pathShape();
    QString data() const { return mData; }
    Type type() const override { return Type::Path; }

    void setData(const QString& d);
    void setAttributes(const QVariantMap &attrs) override;

    qreal PathLength() const { return mPathLength; }
    void setPathLength(qreal newPathLength);

private:
    static QStringList mainAttrs() { return {"d","path-length"}; }

signals:
    void dataChanged();

    void PathLengthChanged();

private:
    QString mData;

    qreal mPathLength;
};
}
