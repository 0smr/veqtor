#pragma once

#include <QHash>
#include <QObject>
#include <QString>
#include <QVariantMap>
#include <QQmlPropertyMap>
#include <QQuickTransform>

#include <memory>
#include <vector>
#include <functional>

#include "../nanopen.h"
#include "../shapes/shapes.h"

namespace veqtor::elements {
class element : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QList<QVariantMap> transform READ transform WRITE setTransform NOTIFY transformChanged)
    Q_PROPERTY(QQmlPropertyMap *attributes READ attributes NOTIFY attributesChanged)
    Q_PROPERTY(QPointF origin READ origin WRITE setOrigin NOTIFY originChanged)
public:
    enum Type {
        Unknown = 0x0,
        /// Graphic
        Graphic = 0x001,
        Line,    Path,    Rect,
        Circle,  Ellipse, Polyline,
        Polygon, Text,    TextPath,
        /// Container
        Container = 0x100,
        SVG, Group, Link,
    };
    Q_ENUM(Type)

    element(QObject *parent = nullptr, QMap<QString, QString> attrs = {});

    /// virtual members
    virtual Type type() const = 0;
    virtual bool contains(const QPointF &) const = 0;

    virtual bool isGraphic() const { return Type::Graphic < type() && type() < Type::Container; }
    virtual bool isContainer() const { return Type::Container < type(); }
    virtual core::nanoPen pen() const { return core::nanoPen(); }

    friend QDebug &operator << (QDebug &debug, const element &el);
    friend QDebug &operator << (QDebug &debug, const QPointer<element> &el);

    QString id() const;
    QQmlPropertyMap *attributes() { return &mAttributes; }

    void setId(const QString &idValue);
    void setAttribute(const QString &key, const QString &value);
    /**
     * @brief setAttributes
     * @param attrs, A map of attributes
     * @abstract
     */
    virtual void setAttributes(const QVariantMap &attrs);

    QList<QVariantMap> transform() const { return mTransform; }
    void setTransform(const QList<QVariantMap> &transforms) {
        if (mTransform == transforms) return;

        mTransform = transforms;
        mTransformBuff.reset();
        mTransformBuff.translate(mOrigin.x(), mOrigin.y());

        for(const auto &t: qAsConst(mTransform)) {
            if(t["t"] == "rotate") {
                Qt::Axis axis = t["axis"].isNull() ? Qt::ZAxis : Qt::Axis(t["axis"].toInt());
                mTransformBuff.rotate(t["angle"].toDouble(), axis);
            } else if(t["t"] == "scale") {
                mTransformBuff.scale(t["x"].toDouble(), t["y"].toDouble());
            } else if(t["t"] == "shear") {
                mTransformBuff.shear(t["h"].toDouble(), t["v"].toDouble());
            } else if(t["t"] == "translate") {
                mTransformBuff.translate(t["x"].toDouble(), t["y"].toDouble());
            }
        }
        mTransformBuff.translate(-mOrigin.x(), -mOrigin.y());

        emit transformChanged();
        emit updated();
    }

    QPointF origin() const { return mOrigin; }
    void setOrigin(QPointF o) {
        if(mOrigin == o) return;
        mOrigin = o;

        emit originChanged();
        emit updated();
    }

private:
    static QStringList mainAttrs() { return {"id","class","style","tab-index"}; }

signals:
    void updated();
    void idChanged();
    void attributesChanged();
    void transformChanged();
    void originChanged();

protected:
    QString mId;
    QStringList mClass;
    QHash<QString, QString> mStyle;

    QList<QVariantMap> mTransform;
    QTransform mTransformBuff;
    QPointF mOrigin;

    long long mTabIndex;

    QQmlPropertyMap mAttributes;
};
}
