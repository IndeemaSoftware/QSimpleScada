#ifndef VBOARDINFO_H
#define VBOARDINFO_H

#include <QList>
#include <QDebug>

class QScadaObjectInfo;

class QScadaBoardInfo
{
public:
    void appendObjectInfo(QScadaObjectInfo* object){
        mObjectList.append(object);
    }

    QList<QScadaObjectInfo*> objectList() {
        return mObjectList;
    }
private:
    QList<QScadaObjectInfo*> mObjectList;
};
#endif // VBOARDINFO_H
