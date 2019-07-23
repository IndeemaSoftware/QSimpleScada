#include "qscadaobject.h"

#include <QMouseEvent>
#include <QDebug>
#include <QApplication>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPen>

#define RESIZE_FIELD_SIZE 10
#define RESIZE_AREA(x, y) ((geometry().width() - RESIZE_FIELD_SIZE) < x) && ((geometry().height() - RESIZE_FIELD_SIZE) < y)

QScadaObject::QScadaObject(QWidget *parent) :
    QWidget(parent),
    mInfo{new QScadaObjectInfo(this)},
    mEffect{new QGraphicsDropShadowEffect}
{
    this->initUI();
}

QScadaObject::QScadaObject(QScadaObjectInfo *info, QWidget *parent):
    QWidget(parent),
    mInfo{new QScadaObjectInfo(info)},
    mEffect{new QGraphicsDropShadowEffect}
{
    this->initUI();
}

QScadaObject::~QScadaObject()
{
    delete mInfo;
    delete mEffect;
}

void QScadaObject::setGeometry(int x, int y, int width, int height)
{
    setGeometry(QRect(x, y, width, height));
}

void QScadaObject::setGeometry(const QRect &r)
{
    info()->setGeometry(r);

    QWidget::setGeometry(r);
}

QRect QScadaObject::geometry()
{
    return info()->geometry();
}

void QScadaObject::mouseMoveEvent(QMouseEvent *event)
{
    if (mIsEditable) {
        switch (action()) {
        case QObjectActionMove:{
            move(event->x(), event->y());
            break;
        }
        case QObjectActionResize:{
            resize(event->x(), event->y());
            break;
        }
        case QObjectActionNone: {
            if (RESIZE_AREA(event->x(), event->y())) {
                QApplication::setOverrideCursor(Qt::SizeFDiagCursor);
            } else if (underMouse()) {
                QApplication::setOverrideCursor(Qt::OpenHandCursor);
            }
        }
        }
    } else {
        QWidget::mouseMoveEvent(event);
    }
}

void QScadaObject::mousePressEvent(QMouseEvent *event)
{
    if (mIsEditable) {
        if (event->button() == Qt::LeftButton) {
            int lX = event->x();
            int lY = event->y();

            if (RESIZE_AREA(lX, lY)) {
                setAction(QObjectActionResize);
            } else {
                setSelected(true);
                QApplication::setOverrideCursor(Qt::ClosedHandCursor);
                setAction(QObjectActionMove);

                mPosition.setX(lX);
                mPosition.setY(lY);
            }
        }
    } else {
        QWidget::mousePressEvent(event);
    }
}

void QScadaObject::mouseReleaseEvent(QMouseEvent *event)
{
    if (mIsEditable) {
        (void)event;
        setAction(QObjectActionNone);
        QApplication::setOverrideCursor(Qt::ArrowCursor);
    } else {
        QWidget::mouseReleaseEvent(event);
    }
}

void QScadaObject::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        emit objectDoubleClicked(this);
    }
}

void QScadaObject::paintEvent(QPaintEvent *e)
{
    QPainter lPainter(this);
    QPen lLinepen(Qt::black);
    lLinepen.setCapStyle(Qt::RoundCap);
    lPainter.setRenderHint(QPainter::Antialiasing,true);
    lPainter.setPen(lLinepen);

    int lX;
    int lY;

    //draw resize dots
    lX = geometry().width()-RESIZE_FIELD_SIZE;
    lY = geometry().height();

    lLinepen.setColor(Qt::darkGray);
    lLinepen.setWidth(1);
    lPainter.setPen(lLinepen);

    if (mIsEditable) {
        for (int i=1; i<=RESIZE_FIELD_SIZE; i++) {
            for (int j=1; j<=i; j++) {
                lPainter.drawPoint(QPoint(lX + 2*i, lY - 2*j));
            }
        }
    }

    if (info()->showBackground()) {
        lLinepen.setWidth(2);
        lPainter.setPen(lLinepen);
        lPainter.drawRoundedRect(0,0,width(), height(),3,3);
    }

    QWidget::paintEvent(e);
}

bool QScadaObject::isEditable() const
{
    return mIsEditable;
}

void QScadaObject::setIsEditable(bool isEditable)
{
    mIsEditable = isEditable;
}

void QScadaObject::update()
{
    QWidget::update();

    if (info()->showBackground()) {
        setPalette(QPalette(Qt::white));
        setAutoFillBackground(true);
    } else {
        setPalette(QPalette(Qt::transparent));
        setAutoFillBackground(true);
    }

    setGeometry(info()->geometry());
    dynamicStatusChanged(info());
}

bool QScadaObject::selected() const
{
    return mSelected;
}

void QScadaObject::setSelected(bool selected)
{
    mSelected = selected;

    if (mSelected) {
        emit objectSelected(mInfo->id());

        mEffect->setBlurRadius(50);
    } else {
        mEffect->setBlurRadius(10);
    }
}

QScadaObjectInfo *QScadaObject::info() const
{
    return mInfo;
}

void QScadaObject::setInfo(QScadaObjectInfo *info)
{
    mInfo = info;
}

QScadaObjectAction QScadaObject::action() const
{
    return mAction;
}

void QScadaObject::setAction(const QScadaObjectAction &action)
{
    mAction = action;
}

void QScadaObject::move(int x, int y)
{
    int lX = geometry().x() + x - mPosition.x();
    int lY = geometry().y() + y - mPosition.y();

    setGeometry(lX,
                lY,
                geometry().width(),
                geometry().height());

    if (x != 0 && x != 0) {
        emit objectMove(lX, lY);
    }
}

void QScadaObject::resize(int x, int y)
{
    int lX = x - geometry().width();
    int lY = y - geometry().height();

    setGeometry(geometry().x(),
                geometry().y(),
                geometry().width() + lX,
                geometry().height() + lY);

    repaint();

    if (x != 0 && y != 0) {
        emit objectResize(lX, lY);
    }
}

void QScadaObject::initUI()
{
    setGeometry(info()->geometry());
    if (info()->showBackground()) {
        setPalette(QPalette(Qt::transparent));
        setAutoFillBackground(true);
    }

    mEffect->setOffset(0);
    setGraphicsEffect(mEffect);

    setAction(QObjectActionNone);
    setMouseTracking(true);//this not mouseMoveEven is called everytime mouse is moved

    connect(mInfo, SIGNAL(dynamicStatusChanged(QScadaObjectInfo *)), this, SLOT(dynamicStatusChanged(QScadaObjectInfo *)));

    setGeometry(info()->geometry());
}
