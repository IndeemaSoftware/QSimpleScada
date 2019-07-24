#include "qscadaobjectinfo.h"

#include <QDebug>

QScadaObjectInfo::QScadaObjectInfo(QObject *parent) :
    QObject(parent),
    mOrderLevel{0},
    mShowBackground{true},
    mType{QScadaObjectTypeWidget},
    mUIResourcePath{""}
{
    setGeometry(QRect(100, 100, 100, 100));
}

QScadaObjectInfo::QScadaObjectInfo(QScadaObjectInfo *o):
    mGeometry{o->geometry()},
    mId{o->id()},
    mOrderLevel{o->orderLevel()},
    mShowBackground{o->showBackground()},
    mUIProperties{o->UIProperties()},
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

QRect QScadaObjectInfo::geometry() const
{
    return mGeometry;
}

void QScadaObjectInfo::setGeometry(const QRect &geometry)
{
    mGeometry = geometry;
    emit geometryChanged(this);
}

bool QScadaObjectInfo::showBackground() const
{
    return mShowBackground;
}

void QScadaObjectInfo::setShowBackground(bool showBackground)
{
    mShowBackground = showBackground;
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
