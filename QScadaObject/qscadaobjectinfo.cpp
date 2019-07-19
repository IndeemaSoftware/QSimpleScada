#include "qscadaobjectinfo.h"

#include <QDebug>

QScadaObjectInfo::QScadaObjectInfo(QObject *parent) :
    QObject(parent),
    mIsDynamic{false},
    mTitle{"Noname"},
    mOrderLevel{0},
    mShowBackgroundImage{false},
    mShowBackground{true},
    mShowMarkers{true},
    mFrom{0},
    mTo{100},
    mUIResourcePath{""}
{
    setGeometry(QRect(100, 100, 100, 100));
}

QScadaObjectInfo::QScadaObjectInfo(QScadaObjectInfo *o):
    mIsDynamic{o->isDynamic()},
    mGeometry{o->geometry()},
    mId{o->id()},
    mTitle{o->title()},
    mOrderLevel{o->orderLevel()},
    mBackGroundImage{o->backGroundImage()},
    mShowBackgroundImage{o->showBackgroundImage()},
    mShowBackground{o->showBackground()},
    mShowMarkers{o->showMarkers()},
    mFrom{0},
    mTo{100},
    mUIResourcePath{""}
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

QScadaObjectType QScadaObjectInfo::type() const
{
    return mType;
}

void QScadaObjectInfo::setType(const QScadaObjectType &type)
{
    mType = type;
}

QString QScadaObjectInfo::uiResourcePath() const
{
    return mUIResourcePath;
}

void QScadaObjectInfo::setUIResourcePath(const QString &uIResourcePath)
{
    mUIResourcePath = uIResourcePath;
}

qreal QScadaObjectInfo::from() const
{
    return mFrom;
}

void QScadaObjectInfo::setFrom(const qreal &from)
{
    mFrom = from;
}

qreal QScadaObjectInfo::to() const
{
    return mTo;
}

void QScadaObjectInfo::setTo(const qreal &to)
{
    mTo = to;
}

QMLInfo QScadaObjectInfo::qMLInfo() const
{
    return mQMLInfo;
}

void QScadaObjectInfo::setQMLInfo(const QMLInfo &qMLInfo)
{
    mQMLInfo = qMLInfo;
}
