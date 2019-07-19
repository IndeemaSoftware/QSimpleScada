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

    void setProperty(QString, QVariant);
    void setProperty(char*, QVariant);
    void updateValue(QVariant);

    void update();

    QQuickItem *QMLObject() const;
    void setQMLObject(QQuickItem *qMLObject);

    QStringList metaData() const;

private:
    void resize(int, int);
    void updateQMLGeometry();

    void setFrom(qreal);
    void setTo(qreal);

protected slots:
    void initFromQML(QScadaObjectInfo *info);
    void dynamicStatusChanged(QScadaObjectInfo*);

private:
    QQuickItem *mQMLObject;
    QStringList mMetaData;
};

#endif // QSCADAOBJECTQML_H
