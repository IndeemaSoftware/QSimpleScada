#ifndef VOBJECTINFOAXIS_H
#define VOBJECTINFOAXIS_H

#include <QString>

#define AXIS_UP "Upside"
#define AXIS_ASIDE "Aside"
#define AXIS_INSIDE "Inside"
#define AXIS_UNDEFINED "UNDEFINED"

#define ZXIS_X "x"
#define ZXIS_Y "y"
#define ZXIS_Z "z"
#define ZXIS_UNDEFINED "-1"

typedef enum {
    VObjectInfoAxisDirrectionUndefined,
    VObjectInfoAxisDirrectionUpside,
    VObjectInfoAxisDirrectionAside,
    VObjectInfoAxisDirrectionInside
} QScadaObjectInfoAxisDirrection;

class QScadaObjectInfoAxis
{
public:
    explicit QScadaObjectInfoAxis();

    QString stringFromAxis(QScadaObjectInfoAxisDirrection dirr);

    QScadaObjectInfoAxisDirrection axisFromString(QString string);

    QString upsideAxisString();
    QString asideAxisString();
    QString insideAxisString();

    QString getStringX();
    QScadaObjectInfoAxisDirrection getX() const;
    void setX(const QScadaObjectInfoAxisDirrection &value);

    QString getStringY();
    QScadaObjectInfoAxisDirrection getY() const;
    void setY(const QScadaObjectInfoAxisDirrection &value);

    QString getStringZ();
    QScadaObjectInfoAxisDirrection getZ() const;
    void setZ(const QScadaObjectInfoAxisDirrection &value);

private:
    QScadaObjectInfoAxisDirrection mX;
    QScadaObjectInfoAxisDirrection mY;
    QScadaObjectInfoAxisDirrection mZ;
};

#endif // VOBJECTINFOAXIS_H
