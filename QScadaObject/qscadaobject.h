#ifndef VOBJECT_H
#define VOBJECT_H

#include "qscadaobjectinfo.h"

#include <QWidget>
#include <QLabel>
#include <QImage>

class QGraphicsDropShadowEffect;

class QScadaObject : public QWidget
{
    Q_OBJECT
public:
    explicit QScadaObject(QWidget *parent = nullptr);
    QScadaObject(QScadaObjectInfo *, QWidget *parent = nullptr);
    ~QScadaObject();

    void setGeometry(int x, int y, int width, int height);
    void setGeometry(const QRect &);
    QRect geometry();

    QScadaObjectInfo *info() const;
    void setInfo(QScadaObjectInfo *info);

    bool selected() const;
    void setSelected(bool selected);

    bool isEditable() const;
    void setIsEditable(bool isEditable);

    void update();

    QScadaObjectStatus status() const;
    void setStatus(const QScadaObjectStatus &status);

private:
    QScadaObjectAction action() const;
    void setAction(const QScadaObjectAction &action);

    void move(int x, int y);
    void resize(int x, int y);

    void initUI();

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent*);

    void paintEvent(QPaintEvent *e);

signals:
    void objectSelected(int id);
    void objectDoubleClicked(QScadaObject*);
    void objectMove(int x, int y);
    void objectResize(int x, int y);

protected slots:
    void dynamicStatusChanged(QScadaObjectInfo*);

private:
    QPoint mPosition;
    bool mIsEditable;//if tru object could be moveable, if false it's static

    QScadaObjectAction mAction;
    QScadaObjectInfo *mInfo;
    QGraphicsDropShadowEffect* mEffect;
    bool mSelected;

    QScadaObjectStatus mStatus;// default is VObjectStatusNone
};

#endif // VOBJECT_H
