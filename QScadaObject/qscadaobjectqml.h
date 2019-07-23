#ifndef QSCADAOBJECTQML_H
#define QSCADAOBJECTQML_H

#include "qscadaobject.h"

#include <QMultiMap>

class QQuickItem;

class QScadaObjectQML : public QScadaObject
{
public:

    static const char *funcUpdate;
    static const char *tagMetaData;

    explicit QScadaObjectQML(QScadaObjectInfo *,QWidget *parent = nullptr);

    void setProperty(QString, QVariant);
    void setProperty(char*, QVariant);
    void updateUIProperties();
    void updateValue(QVariant);

    void update();

    QQuickItem *QMLObject() const;
    void setQMLObject(QQuickItem *qMLObject);

    QMultiMap<QString, QVariant> QMLProperties() const;

private:
    void resize(int, int);
    void updateQMLGeometry();

protected slots:
    void initFromQML(QScadaObjectInfo *info);
    void dynamicStatusChanged(QScadaObjectInfo*);

private:
    QQuickItem *mQMLObject;
};

#endif // QSCADAOBJECTQML_H
