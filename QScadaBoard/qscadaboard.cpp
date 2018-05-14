#include "qscadaboard.h"

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

QScadaBoard::~QScadaBoard()
{
    qDeleteAll(*mObjects);
    delete mObjects;
}

void QScadaBoard::createNewObject()
{
    createNewObject(mObjects->count());
}

void QScadaBoard::createNewObject(QScadaObjectInfo *info)
{
    QScadaObject *lObject = new QScadaObject(this);
    lObject->setInfo(info);
    //rize object if it's dynamic so general image will be on background
    if (info->isDynamic()) {
        bringToFront(lObject);
    }
    lObject->setIsEditable(mEditable);
    connect(lObject, SIGNAL(objectDoubleClicked(QScadaObject*)), this , SIGNAL(objectDoubleClicked(QScadaObject*)));
    connect(lObject, SIGNAL(objectSelected(int)), this , SLOT(newObjectSelected(int)));
    connect(lObject, SIGNAL(objectMove(int,int)), this , SLOT(objectMove(int,int)));
    connect(lObject, SIGNAL(objectResize(int,int)), this , SLOT(objectResize(int,int)));
    lObject->show();
    lObject->update();
    mObjects->append(lObject);
}

void QScadaBoard::createNewObject(int id)
{
    QScadaObjectInfo *lInfo = new QScadaObjectInfo();
    lInfo->setId(id);
    lInfo->setShowMarkers(true);
    lInfo->setShowBackground(true);
    lInfo->setShowBackgroundImage(false);
    lInfo->setIsDynamic(true);

    createNewObject(lInfo);
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

void QScadaBoard::bringToFront(QScadaObject *o)
{
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
