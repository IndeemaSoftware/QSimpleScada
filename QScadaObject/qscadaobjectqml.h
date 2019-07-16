#ifndef QSCADAOBJECTQML_H
#define QSCADAOBJECTQML_H

#include "qscadaobject.h"

class QQuickItem;

class QScadaObjectQML : public QScadaObject
{
public:

    static const char *tagId;
    static const char *tagFrom;
    static const char *tagTo;
    static const char *tagValue;

    explicit QScadaObjectQML(QScadaObjectInfo *,QWidget *parent = nullptr);

    qreal value() const;
    void setValue(const qreal &value);

    void update();

    QQuickItem *QMLObject() const;
    void setQMLObject(QQuickItem *qMLObject);

private:
    void resize(int, int);
    void updateQMLGeometry();

    void setFrom(qreal);
    void setTo(qreal);

private:
    QQuickItem *mQMLObject;
};

#endif // QSCADAOBJECTQML_H
