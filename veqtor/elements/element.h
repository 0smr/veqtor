#pragma once

#include <QHash>
#include <QObject>
#include <QString>
#include <QVariantMap>
#include <QQmlPropertyMap>

#include <memory>
#include <vector>
#include <functional>

#include "../nanopen.h"
#include "../shapes/shapes.h"

namespace veqtor::elements {
class element : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QQmlPropertyMap *attributes READ attributes NOTIFY attributesChanged)
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
    virtual void setAttributes(const QVariantMap &attrs);

private:
    static QStringList mainAttrs() { return {"id","class","style","tab-index"}; }
signals:
    void updated();
    void idChanged();
    void attributesChanged();

protected:
    QString mId;
    QString mClass;
    QString mStyle;

    qint64 mTabIndex;

    QQmlPropertyMap mAttributes;
};
}

