#ifndef QSCADAOBJECTWIDGET_H
#define QSCADAOBJECTWIDGET_H

#include "qscadaobject.h"

class QScadaObjectWidget : public QScadaObject
{
public:
    explicit QScadaObjectWidget(QScadaObjectInfo *, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *e);

protected slots:
    void dynamicStatusChanged(QScadaObjectInfo*);
};

#endif // QSCADAOBJECTWIDGET_H
