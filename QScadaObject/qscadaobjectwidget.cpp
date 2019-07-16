#include "qscadaobjectwidget.h"

#include <QPainter>

QScadaObjectWidget::QScadaObjectWidget(QScadaObjectInfo *info, QWidget *parent): QScadaObject(info, parent)
{

}

void QScadaObjectWidget::update()
{
    QScadaObject::update();
}

void QScadaObjectWidget::dynamicStatusChanged(QScadaObjectInfo*)
{
    if (info()->isDynamic() && info()->type() == QScadaObjectTypeWidget) {
        switch(status()) {
        case QObjectStatusNone:
            setPalette(QPalette(Qt::lightGray));
            break;
        case QObjectStatusRed:
            setPalette(QPalette(Qt::red));
            break;
        case QObjectStatusYellow:
            setPalette(QPalette(Qt::yellow));
            break;
        case QObjectStatusGreen:
            setPalette(QPalette(Qt::green));
            break;
        }
    } else {
        setPalette(QPalette(Qt::white));
    }

    if (!info()->showBackground()) {
        setPalette(QPalette(Qt::transparent));
    }
}

void QScadaObjectWidget::paintEvent(QPaintEvent *e)
{
    QScadaObject::paintEvent(e);

    QPainter lPainter(this);
    QPixmap lMarkerPixmap(info()->imageName(this->status()));
    QPixmap lBackgroundPixmap(info()->backGroundImage());
    QPen lLinepen(Qt::black);
    lLinepen.setCapStyle(Qt::RoundCap);
    lPainter.setRenderHint(QPainter::Antialiasing,true);
    lPainter.setPen(lLinepen);

    //draw object title
    lPainter.drawText(QPoint(10, 20), this->info()->title());
    int lX;
    int lY;

    if (info()->axiesEnabled()) {
        //draw axies

        switch(info()->axisPosition()) {
        case QObjectAxisPositionLeft:
            lX = 12;
            break;
        case QObjectAxisPositionRight:
            lX = geometry().width() - 30;
            break;
        }
        lY = geometry().height() - 10;
        int lWidth = 10;
        int lInner = 4;

        lPainter.drawEllipse(lX-lInner/2,
                             lY-lInner/2,
                             lInner,
                             lInner);
        lPainter.drawText(QPoint(lX - 10, geometry().height()-2), info()->axis().insideAxisString());//inside position

        lPainter.drawLine(lX, lY,
                          lX, lY -lWidth);
        lPainter.drawLine(lX, lY -lWidth-1,
                          lX-3, lY -lWidth+3);
        lPainter.drawLine(lX, lY -lWidth-1,
                          lX+3, lY -lWidth+3);
        lPainter.drawText(QPoint(lX - 3, lY -lWidth-3), info()->axis().upsideAxisString());//up possibtion

        lPainter.drawLine(lX, lY,
                          lX + lWidth, lY);
        lPainter.drawLine(lX + lWidth+1, lY,
                          lX + lWidth-3, lY-3);
        lPainter.drawLine(lX + lWidth+1, lY,
                          lX + lWidth-3, lY+3);
        lPainter.drawText(QPoint(lX + lWidth + 3, lY+3), info()->axis().asideAxisString());//aside position
    }

    if (info()->showMarkers()) {
        QSize lSize = lMarkerPixmap.size();
        lPainter.drawPixmap(QRect((width() - lSize.width()) /2,
                                  (height() - lSize.height()) / 2,
                                  lSize.width(),
                                  lSize.height()),
                            lMarkerPixmap);
    }

    if (info()->showBackgroundImage()) {
        lPainter.drawPixmap(QRect(0,
                                  0,
                                  width(),
                                  height()),
                            lBackgroundPixmap.scaled(width(), height(),
                                                     Qt::KeepAspectRatioByExpanding));
    }

    if (info()->isDynamic()) {
        switch(this->status()) {
        case QObjectStatusNone:
            lLinepen.setColor(Qt::darkGray);
            break;
        case QObjectStatusRed:
            lLinepen.setColor(QColor(171, 27, 227, 255));
            break;
        case QObjectStatusYellow:
            lLinepen.setColor(QColor(228, 221, 29, 255));
            break;
        case QObjectStatusGreen:
            lLinepen.setColor(QColor(14, 121, 7, 255));
            break;
        }
    } else {
        lLinepen.setColor(Qt::black);
    }

    QWidget::paintEvent(e);
}
