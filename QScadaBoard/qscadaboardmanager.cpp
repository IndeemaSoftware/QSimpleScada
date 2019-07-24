#include "qscadaboardmanager.h"
#include "qscadaboard.h"
#include "qscadaboardcontroller.h"
#include "../QScadaDevice/qscadadeviceinfo.h"
#include "../QScadaBoard/qscadaboardinfo.h"

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

QScadaBoard *QScadaBoardManager::initBoardForDeviceIp(QString deviceIp, QScadaBoardInfo *boardInfo)
{
    QScadaBoard *rBoard = nullptr;
    QScadaDeviceInfo *lDevice = deviceForIp(deviceIp);

    if  (boardInfo == nullptr) {
        boardInfo = new QScadaBoardInfo();
        boardInfo->setId(generateIdForNewBoard());
    }

    if (lDevice != nullptr) {
        rBoard = initBoardWithInfo(boardInfo);
        lDevice->appendBoardId(rBoard->getId());
    }

    return rBoard;
}

void QScadaBoardManager::resetAll()
{
    for (QScadaBoard * board : mBoards.values()) {
        board->hide();
        delete board;
    }

    for (QScadaDeviceInfo *info : mDevices) {
        delete info;
    }

    mDevices.clear();
    mBoards.clear();
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

QScadaBoard *QScadaBoardManager::initBoardWithId(int id)
{
    QScadaBoardInfo *lInfo = new QScadaBoardInfo();
    lInfo->setId(id);
    QScadaBoard *rBoard = initBoardWithInfo(lInfo);

    return  rBoard;
}

QScadaBoard *QScadaBoardManager::initBoardWithInfo(QScadaBoardInfo *boardInfo)
{
    QScadaBoard *rBoard = new QScadaBoard(boardInfo);
    mBoards.insert(rBoard->getId(), rBoard);

    return  rBoard;
}

QList<QScadaDeviceInfo *> QScadaBoardManager::getDevices() const
{
    return mDevices;
}

void QScadaBoardManager::appendDevice(QScadaDeviceInfo *deviceInfo)
{
    QScadaDeviceInfo *lDevice = deviceForIp(deviceInfo->ip().toString());

    //if device with the same ip address exists appeding will be ignored
    if (lDevice == nullptr) {
        mDevices.append(deviceInfo);
        for (int id : deviceInfo->boardIds()) {
            initBoardWithId(id);
        }
    } else {
        qDebug() << "QScadaBoardManager::" << __FUNCTION__ << " device with ip " << deviceInfo->ip().toString() << " already added";
    }
}
