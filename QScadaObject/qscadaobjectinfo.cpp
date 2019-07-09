#include "qscadaobjectinfo.h"

#include <QDebug>

QScadaObjectInfo::QScadaObjectInfo(QObject *parent) :
    QObject(parent),
    mIsDynamic{false},
    mTitle{"Noname"},
    mAxiesEnabled{false},
    mAxisPosition{QObjectAxisPositionLeft},
    mOrderLevel{0},
    mShowBackgroundImage{false},
    mShowBackground{true},
    mShowMarkers{true}
{
    setGeometry(QRect(100, 100, 100, 100));
}

QScadaObjectInfo::QScadaObjectInfo(QScadaObjectInfo *o):
    mIsDynamic{o->isDynamic()},
    mGeometry{o->geometry()},
    mId{o->id()},
    mTitle{o->title()},
    mAxiesEnabled{o->axiesEnabled()},
    mAxisPosition{o->axisPosition()},
    mOrderLevel{o->orderLevel()},
    mImageName{o->infoImage()},
    mBackGroundImage{o->backGroundImage()},
    mShowBackgroundImage{o->showBackgroundImage()},
    mShowBackground{o->showBackground()},
    mShowMarkers{o->showMarkers()}
{

}

int QScadaObjectInfo::id() const
{
    return mId;
}

void QScadaObjectInfo::setId(int id)
{
    mId = id;
}

QString QScadaObjectInfo::title() const
{
    return mTitle;
}

void QScadaObjectInfo::setTitle(const QString &title)
{
    mTitle = title;
}

QScadaObjectInfoAxis QScadaObjectInfo::axis()
{
    return mAxis;
}

void QScadaObjectInfo::setAxis(const QScadaObjectInfoAxis &axis)
{
    mAxis = axis;
}

bool QScadaObjectInfo::axiesEnabled() const
{
    return mAxiesEnabled;
}

void QScadaObjectInfo::setAxiesEnabled(bool axiesEnabled)
{
    mAxiesEnabled = axiesEnabled;
}

QRect QScadaObjectInfo::geometry() const
{
    return mGeometry;
}

void QScadaObjectInfo::setGeometry(const QRect &geometry)
{
    mGeometry = geometry;
    emit geometryChanged(this);
}

bool QScadaObjectInfo::isDynamic() const
{
    return mIsDynamic;
}

void QScadaObjectInfo::setIsDynamic(bool isDynamic)
{
    mIsDynamic = isDynamic;

    emit dynamicStatusChanged(this);
}

QScadaObjectInfoImage QScadaObjectInfo::infoImage()
{
    return mImageName;
}

QString QScadaObjectInfo::imageName(QScadaObjectStatus status)
{
    return mImageName.getImageNameForStatus(status);
}

void QScadaObjectInfo::setImageName(QString imageName, QScadaObjectStatus status)
{
    mImageName.setImageNameForState(imageName, status);
}

bool QScadaObjectInfo::showBackground() const
{
    return mShowBackground;
}

void QScadaObjectInfo::setShowBackground(bool showBackground)
{
    mShowBackground = showBackground;
}

bool QScadaObjectInfo::showMarkers() const
{
    return mShowMarkers;
}

void QScadaObjectInfo::setShowMarkers(bool showMarkers)
{
    mShowMarkers = showMarkers;
}

QScadaObjectAxisPosition QScadaObjectInfo::axisPosition() const
{
    return mAxisPosition;
}

void QScadaObjectInfo::setAxisPosition(const QScadaObjectAxisPosition &axisPosition)
{
    mAxisPosition = axisPosition;
}

QString QScadaObjectInfo::backGroundImage() const
{
    return mBackGroundImage;
}

void QScadaObjectInfo::setBackGroundImage(const QString &backGroundImage)
{
    mBackGroundImage = backGroundImage;
}

bool QScadaObjectInfo::showBackgroundImage() const
{
    return mShowBackgroundImage;
}

void QScadaObjectInfo::setShowBackgroundImage(bool showBackgroundImage)
{
    mShowBackgroundImage = showBackgroundImage;
}

int QScadaObjectInfo::orderLevel() const
{
    return mOrderLevel;
}

void QScadaObjectInfo::setOrderLevel(int value)
{
    mOrderLevel = value;
}

void QScadaObjectInfo::urderUp()
{
    mOrderLevel--;
    if (mOrderLevel < 0) {
        mOrderLevel = 0;
    }
}

void QScadaObjectInfo::orderDown()
{
    mOrderLevel++;
}

//implementations for VObjectInfoImage

QString QScadaObjectInfoImage::getImageNameForStatus(QScadaObjectStatus status)
{
    QString rValue;
    
    switch(status) {
    case QObjectStatusNone:
        rValue = normal;
        break;
    case QObjectStatusGreen:
        rValue = green;
        break;
    case QObjectStatusYellow:
        rValue = yellow;
        break;
    case QObjectStatusRed:
        rValue = red;
        break;
    }

    return rValue;
}

void QScadaObjectInfoImage::setImageNameForState(QString name, QScadaObjectStatus status)
{
    switch(status) {
    case QObjectStatusNone:
        normal = name;
        break;
    case QObjectStatusGreen:
        green = name;
        break;
    case QObjectStatusYellow:
        yellow = name;
        break;
    case QObjectStatusRed:
        red = name;
        break;
    }
}
