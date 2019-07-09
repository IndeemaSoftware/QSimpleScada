#ifndef VOBJECTINFO_H
#define VOBJECTINFO_H

#include "qscadaobjectinfoaxis.h"

#include <QObject>
#include <QColor>
#include <QRect>


typedef enum {
    QObjectActionNone,
    QObjectActionMove,
    QObjectActionResize
} QScadaObjectAction;

typedef enum {
    QObjectStatusNone,
    QObjectStatusRed,
    QObjectStatusYellow,
    QObjectStatusGreen
} QScadaObjectStatus;


typedef enum {
    QObjectAxisPositionLeft,
    QObjectAxisPositionRight
} QScadaObjectAxisPosition;

struct QScadaObjectInfoImage {
    QString normal = ":/com/indeema/QSimpleScada/resources/green_marker.png";
    QString green = ":/com/indeema/QSimpleScada/resources/green_marker.png";
    QString yellow = ":/com/indeema/QSimpleScada/resources/yellow_marker.png";
    QString red = ":/com/indeema/QSimpleScada/resources/red_marker.png";

    QString getImageNameForStatus(QScadaObjectStatus);
    void setImageNameForState(QString, QScadaObjectStatus);
};

class QScadaObjectInfo : public QObject
{
    Q_OBJECT
public:
    explicit QScadaObjectInfo(QObject *parent = 0);
    QScadaObjectInfo(QScadaObjectInfo*);

    int id() const;
    void setId(int id);

    QString title() const;
    void setTitle(const QString &title);

    QScadaObjectInfoAxis axis();
    void setAxis(const QScadaObjectInfoAxis &axis);

    bool axiesEnabled() const;
    void setAxiesEnabled(bool axiesEnabled);

    QRect geometry() const;
    void setGeometry(const QRect &geometry);

    bool isDynamic() const;
    void setIsDynamic(bool isDynamic);

    QScadaObjectInfoImage infoImage();
    QString imageName(QScadaObjectStatus);
    void setImageName(QString imageName, QScadaObjectStatus);

    bool showBackground() const;
    void setShowBackground(bool showBackground);

    bool showMarkers() const;
    void setShowMarkers(bool showMarkers);

    QScadaObjectAxisPosition axisPosition() const;
    void setAxisPosition(const QScadaObjectAxisPosition &axisPosition);

    QString backGroundImage() const;
    void setBackGroundImage(const QString &backGroundImage);

    bool showBackgroundImage() const;
    void setShowBackgroundImage(bool showBackgroundImage);

    int orderLevel() const;
    void setOrderLevel(int value);
    void urderUp();
    void orderDown();

signals:
    void infoChanged(QScadaObjectInfo *info);
    void geometryChanged(QScadaObjectInfo *info);
    void dynamicStatusChanged(QScadaObjectInfo *info);

private:
    bool mIsDynamic; //by default false
    QRect mGeometry;
    int mId;
    QString mTitle;
    QScadaObjectInfoAxis mAxis;
    bool mAxiesEnabled;
    QScadaObjectAxisPosition mAxisPosition;
    int mOrderLevel;

    QScadaObjectInfoImage mImageName;
    QString mBackGroundImage;
    bool mShowBackgroundImage;
    bool mShowBackground;
    bool mShowMarkers;
};

#endif // VOBJECTINFO_H
