#ifndef VBOARD_H
#define VBOARD_H

#include "../QScadaObject/qscadaobject.h"

#include <QWidget>
#include <QList>

class QScadaObjectInfo;

class QScadaBoard : public QWidget
{
    Q_OBJECT
public:
    explicit QScadaBoard(QWidget *parent = 0);
    ~QScadaBoard();

    void createNewObject();
    void createNewObject(QScadaObjectInfo *);
    void createNewObject(int id);

    bool editable() const;
    void setEditable(bool editable);

    bool showGrid() const;
    void setShowGrid(bool showGrid);

    int grid() const;
    void setGrid(int grid);

    void deleteObjectWithId(int);
    void deleteObject(QScadaObject*);
    void updateObjectWithId(int);
    void updateStatusWithId(int, QScadaObjectStatus);

    QList<QScadaObject *> *objects() const;

    QList<QScadaObject*> getSeletedObjects();

    void resetGridPixmap();

public slots:
    //objects order
    void bringToFront(QScadaObject*);
    void sendToBack(QScadaObject*);

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *e);

signals:
    void objectDoubleClicked(QScadaObject*);
    void objectSelected(QScadaObject *);

private slots:
    void newObjectSelected(int id);
    void objectMove(int x, int y);
    void objectResize(int x, int y);

private:
    QList<QScadaObject*> *mObjects;
    bool mEditable; //default value false
    bool mShowGrid; //default value true
    int mGrid; //default value 10
    QPixmap *mPixmap;
    bool mUpdateGridPixmap;
};

#endif // VBOARD_H
