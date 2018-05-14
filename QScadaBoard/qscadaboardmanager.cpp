#include "qscadaboardmanager.h"
#include "qscadaboard.h"

QScadaBoardManager::QScadaBoardManager(QObject *parent) : QObject(parent)
{

}

QScadaBoard *QScadaBoardManager::getBoardForDeviceWithIp(QString ip)
{
    QScadaBoard *rBoard = nullptr;

    if (mBoardMap.contains(ip)) {
        rBoard = mBoardMap.value(ip);
    } else {
        rBoard = new QScadaBoard();
        rBoard->setEditable(true);
        mBoardMap.insert(ip, rBoard);
    }

    return rBoard;
}

QList<QScadaBoard *> QScadaBoardManager::getBoardList()
{
    return mBoardMap.values();
}
