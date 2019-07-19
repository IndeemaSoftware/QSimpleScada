#include "qscadaobjectinfo.h"

#include <QDebug>

QScadaObjectInfo::QScadaObjectInfo(QObject *parent) :
    QObject(parent),
    mIsDynamic{false},
    mTitle{"Noname"},
    mOrderLevel{0},
    mShowBackgroundImage{false},
    mShowBackground{true},
    mType{QScadaObjectTypeWidget},
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
    mType{o->type()},
    mUIResourcePath{o->uiResourcePath()}
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

QMultiMap<QString, QVariant> QScadaObjectInfo::UIProperties() const
{
    return mUIProperties;
}

void QScadaObjectInfo::setUIProperties(const QMultiMap<QString, QVariant> &qMLProperties)
{
    mUIProperties = qMLProperties;
}
