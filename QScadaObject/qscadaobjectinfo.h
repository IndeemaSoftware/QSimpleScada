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

    QString title() const;
    void setTitle(const QString &title);

    QRect geometry() const;
    void setGeometry(const QRect &geometry);

    bool isDynamic() const;
    void setIsDynamic(bool isDynamic);

    bool showBackground() const;
    void setShowBackground(bool showBackground);

    QString backGroundImage() const;
    void setBackGroundImage(const QString &backGroundImage);

    bool showBackgroundImage() const;
    void setShowBackgroundImage(bool showBackgroundImage);

    int orderLevel() const;
    void setOrderLevel(int value);
    void urderUp();
    void orderDown();

    QScadaObjectType type() const;
    void setType(const QScadaObjectType &type);

    QString uiResourcePath() const;
    void setUIResourcePath(const QString &uIResourcePath);

    qreal from() const;
    void setFrom(const qreal &from);

    qreal to() const;
    void setTo(const qreal &to);

    QMLInfo qMLInfo() const;
    void setQMLInfo(const QMLInfo &qMLInfo);

signals:
    void infoChanged(QScadaObjectInfo *info);
    void geometryChanged(QScadaObjectInfo *info);
    void dynamicStatusChanged(QScadaObjectInfo *info);

private:
    bool mIsDynamic; //by default false
    QRect mGeometry;
    int mId;
    QString mTitle;
    int mOrderLevel;

    QString mBackGroundImage;
    bool mShowBackgroundImage;
    bool mShowBackground;

    qreal mFrom;
    qreal mTo;

    QMLInfo mQMLInfo;

    QScadaObjectType mType;
    QString mUIResourcePath;
};

#endif // VOBJECTINFO_H
