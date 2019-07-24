#ifndef VOBJECTINFO_H
#define VOBJECTINFO_H

#include "../qscadaconfig.h"

#include <QObject>
#include <QColor>
#include <QRect>


typedef enum {
    QObjectActionNone,
    QObjectActionMove,
    QObjectActionResize
} QScadaObjectAction;

typedef enum {
    QScadaObjectTypeWidget,
    QScadaObjectTypeQML
} QScadaObjectType;

class QScadaObjectInfo : public QObject
{
    Q_OBJECT
public:
    explicit QScadaObjectInfo(QObject *parent = nullptr);
    QScadaObjectInfo(QScadaObjectInfo*);

    int id() const;
    void setId(int id);

    QRect geometry() const;
    void setGeometry(const QRect &geometry);

    bool showBackground() const;
    void setShowBackground(bool showBackground);

    int orderLevel() const;
    void setOrderLevel(int value);
    void urderUp();
    void orderDown();

    QScadaObjectType type() const;
    void setType(const QScadaObjectType &type);

    QString uiResourcePath() const;
    void setUIResourcePath(const QString &uIResourcePath);

    QMultiMap<QString, QVariant> UIProperties() const;
    void setUIProperties(const QMultiMap<QString, QVariant> &qMLProperties);

signals:
    void infoChanged(QScadaObjectInfo *info);
    void geometryChanged(QScadaObjectInfo *info);
    void dynamicStatusChanged(QScadaObjectInfo *info);

private:
    QRect mGeometry;
    int mId;
    int mOrderLevel;

    bool mShowBackground;

    QMultiMap<QString, QVariant> mUIProperties;

    QScadaObjectType mType;
    QString mUIResourcePath;
};

#endif // VOBJECTINFO_H
