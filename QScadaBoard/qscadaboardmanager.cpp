#include "qscadaboardmanager.h"
#include "qscadaboard.h"
#include "qscadaboardcontroller.h"
#include "../QScadaDevice/qscadadeviceinfo.h"

#include <QWidget>

QScadaBoardManager::QScadaBoardManager(QObject *parent) : QObject(parent)
{

}

QScadaBoard *QScadaBoardManager::getBoard(QString deviceIp, int boardId)
{
    QScadaDeviceInfo *lDevice = deviceForIp(deviceIp);

    return getBoard(lDevice, boardId);
}

QScadaBoard* QScadaBoardManager::getBoard(QScadaDeviceInfo *device, int boardId)
{
    QScadaBoard *rBoard = nullptr;

    if (device != nullptr) {
        for (int id : device->boardIds()) {
            if (id == boardId) {
                rBoard = mBoards.value(id);//get board object for id
            }
        }
    }

    return rBoard;
}

QScadaBoard *QScadaBoardManager::initBoardForDeviceIp(QString deviceIp)
{
    QScadaBoard *rBoard = nullptr;
    QScadaDeviceInfo *lDevice = deviceForIp(deviceIp);

    if (lDevice != nullptr) {
        rBoard = new QScadaBoard(generateIdForNewBoard());
        mBoards.insert(rBoard->getId(), rBoard);
        lDevice->appendBoardId(rBoard->getId());
    }

    return rBoard;
}

QList<QScadaBoard *> QScadaBoardManager::getBoardListForDeviceIp(QString deviceIp)
{
    QList<QScadaBoard *> rBoards;

    QScadaDeviceInfo *lDevice = deviceForIp(deviceIp);

    if (lDevice != nullptr) {
        for (int boardId : lDevice->boardIds()) {
            rBoards.append(mBoards.value(boardId));
        }
    }

    return rBoards;
}

QList<QScadaBoard *> QScadaBoardManager::getBoardList()
{
    return mBoards.values();
}

QScadaDeviceInfo *QScadaBoardManager::deviceForIp(QString ip)
{
    QScadaDeviceInfo *rDevice = nullptr;

    for (QScadaDeviceInfo *device : mDevices) {
        if (device->ip().toString() == ip) {
            rDevice = device;
        }
    }

    return rDevice;
}

int QScadaBoardManager::generateIdForNewBoard()
{
    int rId = 0;
    //new id should bigeer than all exisitng is list
    for (int id : mBoards.keys()) {
        if (id > rId) {
            rId = id + 1;
        }
    }

    return rId;
}

void QScadaBoardManager::appendDevice(QScadaDeviceInfo *deviceInfo)
{
    QScadaDeviceInfo *lDevice = deviceForIp(deviceInfo->ip().toString());

    //if device with the same ip address exists appeding will be ignored
    if (lDevice == nullptr) {
        mDevices.append(deviceInfo);
    } else {
        qDebug() << "QScadaBoardManager::" << __FUNCTION__ << " device with ip " << deviceInfo->ip().toString() << " already added";
    }
}
