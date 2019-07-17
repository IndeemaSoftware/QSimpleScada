#include "qscadaboard.h"
#include "qscadaboardinfo.h"
#include "../QScadaObject/qscadaobjectinfo.h"
#include "../QScadaObject/qscadaobjectqml.h"
#include "../QScadaWidgets/qledindicator.h"

#include <QApplication>
#include <QPainter>
#include <QPen>
#include <QDebug>

QScadaBoard::QScadaBoard(QWidget *parent) :
    QWidget(parent),
    mObjects{new QList<QScadaObject*>()},
    mEditable{false},
    mShowGrid{true},
    mGrid{10},
    mPixmap{nullptr}
{
    setPalette(QPalette(Qt::transparent));
    setAutoFillBackground(true);

    setMouseTracking(true);//this not mouseMoveEven is called everytime mouse is moved

    resetGridPixmap();
}

QScadaBoard::QScadaBoard(QScadaBoardInfo *boardInfo)
{
    this->initBoard(boardInfo);
}

QScadaBoard::~QScadaBoard()
{
    qDeleteAll(*mObjects);
    delete mObjects;
}

void QScadaBoard::initBoard(QScadaBoardInfo *boardInfo)
{
    if (boardInfo != nullptr) {
        this->setEditable(false);

        for (int i=boardInfo->objectList().count()-1; i>=0; i--) {
            for (QScadaObjectInfo *info : boardInfo->objectList()) {
                if (info->orderLevel() == i) {
                   this->initNewObject(info);
                }
            }
        }
    }
}

QScadaObject *QScadaBoard::initNewObject(QScadaObjectInfo *info)
{
    QScadaObject *rObject;

    switch(info->type()){
    case QScadaObjectTypeWidget:
        rObject = new QLedIndicator(info, this);
        break;
    case QScadaObjectTypeQML:
        rObject = new QScadaObjectQML(info, this);
        break;
    }
    rObject->setIsEditable(mEditable);
    connect(rObject, SIGNAL(objectDoubleClicked(QScadaObject*)), this , SIGNAL(objectDoubleClicked(QScadaObject*)));
    connect(rObject, SIGNAL(objectSelected(int)), this , SLOT(newObjectSelected(int)));
    connect(rObject, SIGNAL(objectMove(int,int)), this , SLOT(objectMove(int,int)));
    connect(rObject, SIGNAL(objectResize(int,int)), this , SLOT(objectResize(int,int)));
    rObject->show();
    rObject->update();
    mObjects->append(rObject);

    return rObject;
}

void QScadaBoard::createNewObject()
{
    createNewObject(mObjects->count());
}

void QScadaBoard::createNewObject(QScadaObjectInfo *info)
{
    QScadaObject *lObject = this->initNewObject(info);

    bringToFront(lObject);
}

void QScadaBoard::createNewObject(int id)
{
    QScadaObjectInfo *lInfo = new QScadaObjectInfo();
    lInfo->setId(id);
    lInfo->setShowMarkers(true);
    lInfo->setShowBackground(true);
    lInfo->setShowBackgroundImage(false);
    lInfo->setIsDynamic(true);
    lInfo->setType(QScadaObjectTypeWidget);

    createNewObject(lInfo);
}

void QScadaBoard::createQMLObject(int id, QString path)
{
    QScadaObjectInfo *lInfo = new QScadaObjectInfo();
    lInfo->setId(id);
    lInfo->setShowMarkers(true);
    lInfo->setShowBackground(true);
    lInfo->setShowBackgroundImage(false);
    lInfo->setIsDynamic(true);
    lInfo->setFrom(0);
    lInfo->setTo(100);
    lInfo->setType(QScadaObjectTypeQML);
    lInfo->setUIResourcePath(path);

    createNewObject(lInfo);
}

void QScadaBoard::createQMLObject(QString path)
{
    this->createQMLObject(mObjects->count(), path);
}

void QScadaBoard::mouseMoveEvent(QMouseEvent *event)
{
    (void)event;
    QApplication::setOverrideCursor(Qt::ArrowCursor);
}

void QScadaBoard::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        newObjectSelected(-1);
        emit objectSelected(nullptr);
    }
}

void QScadaBoard::paintEvent(QPaintEvent *e)
{
    if (mPixmap == nullptr) {
        resetGridPixmap();
    }

    if ((mPixmap->width() != this->width())
            || (mPixmap->height() != this->height())) {
        delete mPixmap;
        resetGridPixmap();
    }

    if (mUpdateGridPixmap) {
        QPainter lPainter(mPixmap);
        mPixmap->fill(Qt::white);
        QPen lLinepen(Qt::darkGray);
        lLinepen.setCapStyle(Qt::RoundCap);
        lLinepen.setWidth(1);
        lPainter.setRenderHint(QPainter::Antialiasing,true);
        lPainter.setPen(lLinepen);

        int lX = this->width();
        int lY = this->height();

        mPixmap->scaledToWidth(lX);
        mPixmap->scaledToHeight(lY);

        for (int i=0; i<=lX; i++) {
            for (int j=1; j<=lY; j++) {
                lPainter.drawPoint(QPoint(mGrid*i, mGrid*j));
            }
        }

        mUpdateGridPixmap = false;
    }

    QPainter lPainter(this);
    lPainter.drawPixmap(0, 0, mPixmap->width(), mPixmap->height(), *mPixmap);

    QWidget::paintEvent(e);
}

void QScadaBoard::newObjectSelected(int id)
{
    for(QScadaObject *object : *mObjects) {
        if (id != object->info()->id()) {
            object->setSelected(false);
        } else {
            this->bringToFront(object);
            emit objectSelected(object);
        }
    }
}

void QScadaBoard::objectMove(int, int)
{
    update();
}

void QScadaBoard::objectResize(int, int)
{
    update();
}

QList<QScadaObject *> *QScadaBoard::objects() const
{
    return mObjects;
}

QList<QScadaObject*> QScadaBoard::getSeletedObjects()
{
    QList<QScadaObject*> rList;

    for(QScadaObject *object : *mObjects) {
        if (object->selected()) {
            rList.append(object);
        }
    }

    return rList;
}

void QScadaBoard::resetGridPixmap()
{
    mPixmap = new QPixmap(this->width(), this->height());
    mUpdateGridPixmap = true;
}

#include <QDebug>
void QScadaBoard::orderObject(QScadaObject *o)
{
    bool lIsNew = false;
    for (int i=0;i<mObjects->count();i++) {
        if (mObjects->at(i)->info()->orderLevel() == o->info()->orderLevel()
                && mObjects->at(i)->info()->id() != o->info()->id()) {
            lIsNew = true;
        }
    }

    if (lIsNew){
        for (int i=0;i<mObjects->count();i++) {
            mObjects->at(i)->info()->orderDown();

            if (mObjects->at(i)->info()->orderLevel() >= mObjects->count()) {
                mObjects->at(i)->info()->setOrderLevel(mObjects->count()-1);
            }
        }
    } else {
        for (int i=0;i<mObjects->count();i++) {
            if (mObjects->at(i)->info()->orderLevel() < o->info()->orderLevel()) {
                mObjects->at(i)->info()->orderDown();
            }
        }
    }

    o->info()->setOrderLevel(0);
}

void QScadaBoard::bringToFront(QScadaObject *o)
{
    orderObject(o);
    o->raise();
}

void QScadaBoard::sendToBack(QScadaObject *o)
{
    o->lower();
}

int QScadaBoard::grid() const
{
    return mGrid;
}

void QScadaBoard::setGrid(int grid)
{
    mGrid = grid;
}

void QScadaBoard::deleteObjectWithId(int id)
{
    for (QScadaObject *object : *mObjects) {
        if (id == object->info()->id()) {
            mObjects->removeOne(object);
            delete object;
            repaint();
        }
    }
}

void QScadaBoard::deleteObject(QScadaObject *object)
{
    deleteObjectWithId(object->info()->id());
}

void QScadaBoard::updateObjectWithId(int id)
{
    for (QScadaObject *object : *mObjects) {
        if (id == object->info()->id()) {
            object->update();
        }
    }
}

void QScadaBoard::updateStatusWithId(int id, QScadaObjectStatus status)
{
    for (QScadaObject *object : *mObjects) {
        if (id == object->info()->id()) {
            object->setStatus(status);
            object->update();
        }
    }
}

bool QScadaBoard::showGrid() const
{
    qDebug() << __FUNCTION__;
    return mShowGrid;
}

void QScadaBoard::setShowGrid(bool showGrid)
{
    qDebug() << __FUNCTION__;
    mShowGrid = showGrid;

    repaint();
}

bool QScadaBoard::editable() const
{
    qDebug() << __FUNCTION__;
    return mEditable;
}

void QScadaBoard::setEditable(bool editable)
{
    qDebug() << __FUNCTION__;
    mEditable = editable;

    for (QScadaObject *object : *mObjects) {
        object->setIsEditable(editable);
    }

    update();
}
