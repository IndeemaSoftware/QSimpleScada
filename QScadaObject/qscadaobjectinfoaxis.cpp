#include "qscadaobjectinfoaxis.h"

QScadaObjectInfoAxis::QScadaObjectInfoAxis()
{
    mX = VObjectInfoAxisDirrectionUpside;
    mY = VObjectInfoAxisDirrectionAside;
    mZ = VObjectInfoAxisDirrectionInside;
}

QString QScadaObjectInfoAxis::stringFromAxis(QScadaObjectInfoAxisDirrection dirr)
{
    switch (dirr) {
    case VObjectInfoAxisDirrectionUpside:
        return AXIS_UP;
    case VObjectInfoAxisDirrectionAside:
        return AXIS_ASIDE;
    case VObjectInfoAxisDirrectionInside:
        return AXIS_INSIDE;
    case VObjectInfoAxisDirrectionUndefined:
        return AXIS_UNDEFINED;
    }
}

QScadaObjectInfoAxisDirrection QScadaObjectInfoAxis::axisFromString(QString string)
{
    if (string == QString(AXIS_UP)) {
        return VObjectInfoAxisDirrectionUpside;
    } else if (string == AXIS_ASIDE) {
        return VObjectInfoAxisDirrectionAside;
    } else if (string == AXIS_INSIDE) {
        return VObjectInfoAxisDirrectionInside;
    } else {
        return VObjectInfoAxisDirrectionUndefined;
    }
}

QString QScadaObjectInfoAxis::upsideAxisString()
{
    if (getX() == VObjectInfoAxisDirrectionUpside) {
        return ZXIS_X;
    } else if (getY() == VObjectInfoAxisDirrectionUpside) {
        return ZXIS_Y;
    } else if (getZ() == VObjectInfoAxisDirrectionUpside) {
        return ZXIS_Z;
    } else {
        return ZXIS_UNDEFINED;
    }
}

QString QScadaObjectInfoAxis::asideAxisString()
{
    if (getX() == VObjectInfoAxisDirrectionAside) {
        return ZXIS_X;
    } else if (getY() == VObjectInfoAxisDirrectionAside) {
        return ZXIS_Y;
    } else if (getZ() == VObjectInfoAxisDirrectionAside) {
        return ZXIS_Z;
    } else {
        return ZXIS_UNDEFINED;
    }
}

QString QScadaObjectInfoAxis::insideAxisString()
{
    if (getX() == VObjectInfoAxisDirrectionInside) {
        return ZXIS_X;
    } else if (getY() == VObjectInfoAxisDirrectionInside) {
        return ZXIS_Y;
    } else if (getZ() == VObjectInfoAxisDirrectionInside) {
        return ZXIS_Z;
    } else {
        return ZXIS_UNDEFINED;
    }
}

QString QScadaObjectInfoAxis::getStringX()
{
    return stringFromAxis(getX());
}

QScadaObjectInfoAxisDirrection QScadaObjectInfoAxis::getX() const
{
    return mX;
}

void QScadaObjectInfoAxis::setX(const QScadaObjectInfoAxisDirrection &value)
{
    mX = value;
}

QString QScadaObjectInfoAxis::getStringY()
{
    return stringFromAxis(getY());
}

QScadaObjectInfoAxisDirrection QScadaObjectInfoAxis::getY() const
{
    return mY;
}

void QScadaObjectInfoAxis::setY(const QScadaObjectInfoAxisDirrection &value)
{
    mY = value;
}

QString QScadaObjectInfoAxis::getStringZ()
{
    return stringFromAxis(getZ());
}

QScadaObjectInfoAxisDirrection QScadaObjectInfoAxis::getZ() const
{
    return mZ;
}

void QScadaObjectInfoAxis::setZ(const QScadaObjectInfoAxisDirrection &value)
{
    mZ = value;
}
