#ifndef QLEDINDICATOR_H
#define QLEDINDICATOR_H

#include "../QScadaObject/qscadaobject.h"
#include "../QScadaObject/qscadaobjectinfo.h"

class QLedIndicator : public QScadaObject
{
public:
    explicit QLedIndicator(QScadaObjectInfo *, QWidget *parent = nullptr);

    void update();

protected:
    void paintEvent(QPaintEvent *e);

protected slots:
    void dynamicStatusChanged(QScadaObjectInfo*);
};

#endif // QLEDINDICATOR_H
