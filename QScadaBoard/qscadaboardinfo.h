#ifndef VBOARDINFO_H
#define VBOARDINFO_H

#include "../QScadaObject/qscadaobjectinfo.h"

#include <QList>
#include <QDebug>


class QScadaBoardInfo
{
public:
    void appendObjectInfo(QScadaObjectInfo* object){
        mObjectList.append(object);
    }

    QList<QScadaObjectInfo*> objectList() {
        return mObjectList;
    }

    QList<QScadaObjectInfo*> orderedObjectList() {
        QList<QScadaObjectInfo*>  rList;
        if (mObjectList.count() > 1) {
            rList.append(mObjectList.at(0));
            for (int j=0; j< mObjectList.count(); j++) {
                for (int i=0; i< rList.count(); i++) {
                    if (rList.at(i)->orderLevel() < mObjectList.at(j)->orderLevel()) {
                        rList.prepend(mObjectList.at(j));
                    } else {
                        rList.append(mObjectList.at(j));
                    }
                }
            }

        } else if (mObjectList.count() == 1){
            rList.append(mObjectList.at(0));
        }

        return rList;
    }

    int id() const{
        return mId;
    }
    void setId(int id){
        mId = id;
    }

private:
    int mId;
    QList<QScadaObjectInfo*> mObjectList;
};
#endif // VBOARDINFO_H
