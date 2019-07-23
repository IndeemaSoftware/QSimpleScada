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

    mBoard = mBoardManager->getBoardForDeviceWithIp(ip);
    mMainLayout->addWidget(mBoard);
    mBoard->setGeometry(QRect(0, 0, this->geometry().width(), this->geometry().height()));
    connect(mBoard, SIGNAL(objectSelected(QScadaObject *)), this, SLOT(updateObjectInfoDialog(QScadaObject *)));
    connect(mBoard, SIGNAL(objectDoubleClicked(QScadaObject*)), this, SLOT(objectDoubleClickedHandler(QScadaObject*)));
    connect(mBoard, SIGNAL(newObjectCreated(QScadaObject*)), this, SLOT(updateObjectInfoDialog(QScadaObject *)));
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

void QScadaBoardController::setEditingMode(bool editing)
{
    mBoard->setEditable(editing);
}
