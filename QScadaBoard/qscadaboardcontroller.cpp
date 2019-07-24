#include "qscadaboardcontroller.h"
#include "qscadaboardmanager.h"

#include "qscadaboard.h"
#include "qscadaboardinfo.h"
#include "../QScadaObject/qscadaobjectinfodialog.h"
#include "../QScadaDevice/qscadadeviceinfo.h"
#include "../QScadaEntity/qscadaconnecteddeviceinfo.h"

#include <QGridLayout>
#include <QMenu>
#include <QDebug>
#include <QMessageBox>

QScadaBoardController::QScadaBoardController(QWidget *parent) :
    QWidget(parent),
    mBoard{nullptr},
    mParametersDialod{nullptr}
{
    mMainLayout = new QGridLayout(this);
    mBoardManager = new QScadaBoardManager();

    setPalette(QPalette(Qt::transparent));
    setAutoFillBackground(true);

    this->setParametersDialod(new QScadaObjectInfoDialog());
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),this, SLOT(showContextMenu(const QPoint&)));
}

QScadaBoardController::~QScadaBoardController()
{
    delete mMainLayout;
    delete mBoardManager;
    delete mParametersDialod;
    delete mBoard;
}

void QScadaBoardController::appendDevice(QScadaDeviceInfo *deviceInfo)
{
    mBoardManager->appendDevice(deviceInfo);
}

void QScadaBoardController::clearBoard(QScadaBoard* board)
{
    for (QScadaObject *object : *board->objects()) {
        board->deleteObject(object);
    }
    board->update();
}

void QScadaBoardController::clearAllBoards()
{
    for (QScadaBoard *board:mBoardManager->getBoardList()) {
        clearBoard(board);
    }
}

void QScadaBoardController::resetAllboards()
{
    clearAllBoards();
    mBoardManager->resetAll();
    mBoard = nullptr;
}

void QScadaBoardController::initConnectedDevices(const QList<QScadaBoardInfo *> list)
{
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

void QScadaBoardController::initBoardForDeviceIp(QString ip)
{
    if (mBoard != nullptr
            && mBoard->isVisible()) {
        mBoard->hide();
        disconnect(mBoard, SIGNAL(objectSelected(QScadaObject *)), this, SLOT(updateObjectInfoDialog(QScadaObject *)));
    }

    mBoard = mBoardManager->initBoardForDeviceIp(ip);
    if (mBoard != nullptr) {
        mMainLayout->addWidget(mBoard);
        mBoard->setGeometry(QRect(0, 0, this->geometry().width(), this->geometry().height()));
        connect(mBoard, SIGNAL(objectSelected(QScadaObject *)), this, SLOT(updateObjectInfoDialog(QScadaObject *)));
        connect(mBoard, SIGNAL(objectDoubleClicked(QScadaObject*)), this, SLOT(objectDoubleClickedHandler(QScadaObject*)));
        connect(mBoard, SIGNAL(newObjectCreated(QScadaObject*)), this, SLOT(updateObjectInfoDialog(QScadaObject *)));
    } else {
        qDebug() << "QScadaBoardController::" << __FUNCTION__<< " No device with ip " << ip << " found";
    }
}

void QScadaBoardController::initBoardForDeviceIp(QString ip, QScadaBoardInfo *boardInfo)
{
    if (mBoard != nullptr
            && mBoard->isVisible()) {
        mBoard->hide();
        disconnect(mBoard, SIGNAL(objectSelected(QScadaObject *)), this, SLOT(updateObjectInfoDialog(QScadaObject *)));
    }

    mBoard = mBoardManager->initBoardForDeviceIp(ip, boardInfo);
    if (mBoard != nullptr) {
        mMainLayout->addWidget(mBoard);
        mBoard->setGeometry(QRect(0, 0, this->geometry().width(), this->geometry().height()));
        connect(mBoard, SIGNAL(objectSelected(QScadaObject *)), this, SLOT(updateObjectInfoDialog(QScadaObject *)));
        connect(mBoard, SIGNAL(objectDoubleClicked(QScadaObject*)), this, SLOT(objectDoubleClickedHandler(QScadaObject*)));
        connect(mBoard, SIGNAL(newObjectCreated(QScadaObject*)), this, SLOT(updateObjectInfoDialog(QScadaObject *)));
    } else {
        qDebug() << "QScadaBoardController::" << __FUNCTION__<< " No device with ip " << ip << " found";
    }
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
        bool lShowOrder = (mBoard->getSeletedObjects().count() > 0);
        QMenu lContextMenu{this};

        QMenu lAddWidget(tr("Add Object"));
        lContextMenu.addMenu(&lAddWidget);

        for (QMLWidgetsConfig group : QMLConfig::instance().QMLWidgets()) {
            QMenu *lWidgetMenu = lAddWidget.addMenu(group.info.groupTitle);

            for (QString widget : group.widgets()) {
                QString lWidgetTitle = widget.split(".").at(0);
                QIcon lIcon(group.info.groupPath + lWidgetTitle +".png");
                QAction *lWidgetAction = new QAction(lIcon, lWidgetTitle);//remove qml from name
                lWidgetAction->setData(QVariant::fromValue<QMLWidgetsConfig>(group));

                lWidgetMenu->addAction(lWidgetAction);
                connect(lWidgetAction, SIGNAL(triggered()), this, SLOT(addNewObject()));
            }
        }

        lContextMenu.addAction(tr("Show Parameters"), this, SLOT(showParameters()))->setEnabled(lSelectedObject);

        QMenu *lOrder = lContextMenu.addMenu("Order");
        lOrder->setEnabled(lShowOrder);

        if (lShowOrder) {
            lOrder->addAction(tr("Bring to front"), this, SLOT(bringToFront()));
            lOrder->addAction(tr("Send to back"), this, SLOT(sendToBack()));
        }

        lContextMenu.exec(this->mapToGlobal(pos));
    }
}

void QScadaBoardController::addNewObject()
{
    QAction *lSender = static_cast<QAction*>(QObject::sender());
    QMLWidgetsConfig lConfig = lSender->data().value<QMLWidgetsConfig>();

    QString lQMLFilePath = lConfig.info.groupPath + lSender->text() + ".qml";//add qml to name

    mBoard->createQMLObject(lQMLFilePath);
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
    if (info != nullptr) {
        mBoard->deleteObjectWithId(info->id());
    }
}

void QScadaBoardController::updateSavedObject(QScadaObjectInfo *info)
{
    if (info != nullptr) {
        mBoard->updateObjectWithId(info->id());
    }
}

void QScadaBoardController::objectDoubleClickedHandler(QScadaObject *o)
{
    emit objectDoubleClicked(o);
}

void QScadaBoardController::resizeEvent(QResizeEvent*)
{
    if (mBoard != nullptr) {
        mBoard->setGeometry(QRect(0, 0, this->geometry().width(), this->geometry().height()));
    }
}

QScadaObjectInfoDialog *QScadaBoardController::getParametersDialod() const
{
    return mParametersDialod;
}

void QScadaBoardController::setParametersDialod(QScadaObjectInfoDialog *parametersDialod)
{
    if (mParametersDialod != nullptr) {
        delete mParametersDialod;
    }
    mParametersDialod = parametersDialod;
    connect(mParametersDialod, SIGNAL(deletePressed(QScadaObjectInfo*)), this, SLOT(deleteObject(QScadaObjectInfo *)));
    connect(mParametersDialod, SIGNAL(savePressed(QScadaObjectInfo*)), this, SLOT(updateSavedObject(QScadaObjectInfo *)));
}

void QScadaBoardController::updateValue(QString deviceIp, int boardId, int id, QVariant value)
{
    QScadaBoard *lBoard = mBoardManager->getBoard(deviceIp, boardId);

    if (lBoard != nullptr) {
        lBoard->updateValue(id, value);
    } else {
        qDebug() << "QScadaBoardController::" << __FUNCTION__ << " no available board id" << boardId << " on device with ip " << deviceIp;
    }
}

void QScadaBoardController::setPropertyWithId(QString deviceIp, int boardId, int id, QString property, QVariant value)
{
    QScadaBoard *lBoard = mBoardManager->getBoard(deviceIp, boardId);

    if (lBoard != nullptr) {
        lBoard->setPropertyWithId(id, property, value);
    } else {
        qDebug() << "QScadaBoardController::" << __FUNCTION__ << " for available device with ip " << deviceIp;
    }
}

void QScadaBoardController::openProject(QString file)
{
    if (!file.isEmpty()) {
        QConnectedDeviceInfo* lConnectedDevceInfo = new QConnectedDeviceInfo();
        QByteArray lRawData;
        QFile lFile(file);
        if (lFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream lStreamFileOut(&lFile);
            lStreamFileOut.setCodec("UTF-8");
            lRawData = lStreamFileOut.readAll().toUtf8();
            lFile.close();

            if (mParametersDialod != nullptr) {
                mParametersDialod->updateWithObjectInfo(nullptr);
            }
            //clear all boards before opening new
            resetAllboards();

            lConnectedDevceInfo->initFromXml(lRawData);

            for (int i = 0; i < lConnectedDevceInfo->connecteDeviceList.count(); ++i) {
                QScadaDeviceConfig *lConfig = lConnectedDevceInfo->connecteDeviceList.at(i);
                QScadaDeviceInfo *lInfo = new QScadaDeviceInfo();
                lInfo->setName(lConfig->name);
                lInfo->setDomain(lConfig->domain);
                lInfo->setHost(lConfig->host);
                lInfo->setIp(lConfig->ip);
                lInfo->setIpv6(lConfig->ipv6);

                appendDevice(lInfo);
                for (QScadaBoardInfo *boardInfo : lConfig->boardList) {
                    if (boardInfo != nullptr) {
                        initBoardForDeviceIp(lInfo->ip().toString(), boardInfo);
                    }
                }
            }

            mBoard->update();
            } else {
                qDebug() << "       - Error open preferences file -> " << lFile.fileName();
            }

            delete lConnectedDevceInfo;
    } else {
        qDebug() << "QScadaBoardController::" << __FUNCTION__ << " File name can't be empty";
    }
}

void QScadaBoardController::saveProject(QString file)
{
    if (!file.isEmpty()) {
        if (!file.contains(".irp")) {
            file.append(".irp");
        }

        QString lDevices = QConnectedDeviceInfo::XMLFromDeviceInfo(mBoardManager->getDevices(), this);   //<----;

        //create xml for boards of each device

        QFile lFile(file);
        if (lFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream lOut(&lFile);
            lOut.setCodec("UTF-8");
            lOut << lDevices;
        } else {
            QString lMessage(tr("Something went wrong while trying to create file"));
            lMessage.append(" ").append(file);

            QMessageBox lMsgBox;
            lMsgBox.setText(lMessage);
            lMsgBox.exec();
        }
        lFile.close();
    } else {
        qDebug() << "QScadaBoardController::" << __FUNCTION__ << " File name can't be empty";
    }
}

QList<QScadaBoard *> QScadaBoardController::getBoardList()
{
    return mBoardManager->getBoardList();
}

QList<QScadaBoard *> QScadaBoardController::getBoardListForDeviceIp(QString ip)
{
    QList<QScadaBoard *> rList;

    rList.append(mBoardManager->getBoardListForDeviceIp(ip));

    return rList;
}

void QScadaBoardController::setEditingMode(bool editing)
{
    for (QScadaBoard *board : mBoardManager->getBoardList()) {
        board->setEditable(editing);
        board->setShowGrid(editing);
    }
}
