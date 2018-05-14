#include "qscadaboardcontroller.h"
#include "qscadaboardmanager.h"

#include "qscadaboard.h"
#include "qscadaboardinfo.h"
#include "../QScadaObject/qscadaobjectinfodialog.h"

#include <QGridLayout>
#include <QMenu>
#include <QDebug>

QScadaBoardController::QScadaBoardController(QWidget *parent) :
    QWidget(parent),
    mBoard{nullptr}
{
    mBoardManager = new QScadaBoardManager();

    setPalette(QPalette(Qt::transparent));
    setAutoFillBackground(true);

    mParametersDialod = new QScadaObjectInfoDialog();
    connect(mParametersDialod, SIGNAL(deletePressed(QScadaObjectInfo*)), this, SLOT(deleteObject(QScadaObjectInfo *)));
    connect(mParametersDialod, SIGNAL(savePressed(QScadaObjectInfo*)), this, SLOT(updateSavedObject(QScadaObjectInfo *)));

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),this, SLOT(showContextMenu(const QPoint&)));
}

QScadaBoardController::~QScadaBoardController()
{
    delete mBoardManager;
    delete mParametersDialod;
    delete mBoard;
}

void QScadaBoardController::initConnectedDevices(const QList<QScadaBoardInfo *> list)
{
    qDebug() << __FUNCTION__;
    for (QScadaBoardInfo *boardInfo : list) {
        if (boardInfo != nullptr) {
            mBoard->setEditable(false);
            for (QScadaObjectInfo *info : boardInfo->objectList()) {
                QScadaObjectInfo *lInfo = new QScadaObjectInfo(info);
                mBoard->createNewObject(lInfo);
            }
        }
    }
    mBoard->update();
}

void QScadaBoardController::clearBoard(QScadaBoard* board)
{
    qDebug() << __FUNCTION__;

    for (QScadaObject *object : *board->objects()) {
        board->deleteObject(object);
    }
    board->update();
}

void QScadaBoardController::clearAllBoards()
{
    qDebug() << __FUNCTION__;
    for (QScadaBoard *board:mBoardManager->getBoardList()) {
        clearBoard(board);
    }
}

void QScadaBoardController::initBoardForDeviceIp(QString ip)
{
    qDebug() << __FUNCTION__;
    if (mBoard != nullptr
            && mBoard->isVisible()) {
        mBoard->hide();
        disconnect(mBoard, SIGNAL(objectSelected(QScadaObject *)), this, SLOT(updateObjectInfoDialog(QScadaObject *)));
    }

    mBoard = mBoardManager->getBoardForDeviceWithIp(ip);
    mBoard->setGeometry(QRect(0, 0, this->geometry().width(), this->geometry().height()));
    connect(mBoard, SIGNAL(objectSelected(QScadaObject *)), this, SLOT(updateObjectInfoDialog(QScadaObject *)));
    connect(mBoard, SIGNAL(objectDoubleClicked(QScadaObject*)), this, SLOT(objectDoubleClickedHandler(QScadaObject*)));
}

void QScadaBoardController::updateBoardForDeviceIp(QString ip)
{
    initBoardForDeviceIp(ip);

    mBoard->setParent(this);
    mBoard->show();
}

void QScadaBoardController::showContextMenu(const QPoint &pos)
{
    if (mBoard != nullptr
            && mBoard->isVisible()
            && mBoard->editable()) { //show context menu if only VBoard is visible
        bool lSelectedObject = (mBoard->getSeletedObjects().count()>0);
        QMenu lContextMenu{this};

        lContextMenu.addAction(tr("Add Object"), this, SLOT(addNewObject()));
        lContextMenu.addAction(tr("Show Parameters"), this, SLOT(showParameters()))->setEnabled(lSelectedObject);
        QMenu *lOrderMenu = lContextMenu.addMenu(tr("Order"));
        lOrderMenu->setEnabled(lSelectedObject);

        if (lSelectedObject) {
            lOrderMenu->addAction(tr("Bring to front"), this, SLOT(bringToFront()));
            lOrderMenu->addAction(tr("Send to back"), this, SLOT(sendToBack()));
        }

        lContextMenu.exec(mapToGlobal(pos));
    }
}

void QScadaBoardController::addNewObject()
{
    mBoard->createNewObject();
}

void QScadaBoardController::bringToFront()
{
    if (!mBoard->getSeletedObjects().isEmpty()) {
        QScadaObject *lObject = mBoard->getSeletedObjects().first();
        mBoard->bringToFront(lObject);
    }
}

void QScadaBoardController::sendToBack()
{
    if (!mBoard->getSeletedObjects().isEmpty()) {
        QScadaObject *lObject = mBoard->getSeletedObjects().first();
        mBoard->sendToBack(lObject);
    }
}

void QScadaBoardController::showParameters()
{
    if (mParametersDialod->isVisible()) {
        mParametersDialod->raise();
    } else {
        mParametersDialod->show();
    }
}

void QScadaBoardController::updateObjectInfoDialog(QScadaObject *object)
{
    if (object != nullptr) {
        mParametersDialod->updateWithObjectInfo(object->info());
    } else {
        mParametersDialod->updateWithObjectInfo(nullptr);
    }
}

void QScadaBoardController::deleteObject(QScadaObjectInfo *info)
{
    qDebug() << __FUNCTION__;

    if (info != nullptr) {
        mBoard->deleteObjectWithId(info->id());
    }
}

void QScadaBoardController::updateSavedObject(QScadaObjectInfo *info)
{
    qDebug() << __FUNCTION__;

    if (info != nullptr) {
        mBoard->updateObjectWithId(info->id());
    }
}

void QScadaBoardController::updateStatus()
{
    qDebug() << __FUNCTION__;

    int lNumber = 3;
    int lRandomValue = qrand() % lNumber;
    mBoard->updateStatusWithId(0, (QScadaObjectStatus)lRandomValue);
    lRandomValue = qrand() % lNumber;
    mBoard->updateStatusWithId(1, (QScadaObjectStatus)lRandomValue);
}

void QScadaBoardController::objectDoubleClickedHandler(QScadaObject *o)
{
    emit objectDoubleClicked(o);
}

void QScadaBoardController::resizeEvent(QResizeEvent*)
{
    qDebug() << __FUNCTION__;

    if (mBoard != nullptr) {
        mBoard->setGeometry(QRect(0, 0, this->geometry().width(), this->geometry().height()));
    }
}

QList<QScadaBoard *> QScadaBoardController::getBoardList()
{
    return mBoardManager->getBoardList();
}

QList<QScadaBoard *> QScadaBoardController::getBoardListForDeviceIp(QString ip)
{
    QList<QScadaBoard *> rList;
    rList.append(mBoardManager->getBoardForDeviceWithIp(ip));

    return rList;
}

void QScadaBoardController::updateStatus(QString id, int objectId, QScadaObjectStatus status)
{
    qDebug() << __FUNCTION__;

    QScadaBoard *lBoard = mBoardManager->getBoardForDeviceWithIp(id);

    for (QScadaObject *object :*lBoard->objects()) {
        if (object->info()->id() == objectId) {
            object->setStatus(status);
        }
    }
}

void QScadaBoardController::updateStatus(QStringList device, QList<int> objectId, QList<QScadaObjectStatus> status)
{
    for (int i=0; i< device.count(); i++) {
        updateStatus(device.at(i), objectId.at(i), status.at(i));
    }
}

void QScadaBoardController::setEditingMode(bool editing)
{
    mBoard->setEditable(editing);
}
