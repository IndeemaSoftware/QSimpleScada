#ifndef VVIEWCONFIGURAOT_H
#define VVIEWCONFIGURAOT_H

#include <QWidget>
#include <QList>

#include "../QScadaObject/qscadaobject.h"

class QScadaBoard;
class QScadaObjectInfoDialog;
class QScadaObjectInfo;
class QScadaObject;
class VUnitModel;
class QScadaBoardManager;
class QScadaBoardInfo;

class QScadaBoardController : public QWidget
{
    Q_OBJECT
public:
    explicit QScadaBoardController(QWidget *parent = nullptr);
    ~QScadaBoardController();

    void clearBoard(QScadaBoard*);
    void clearAllBoards();

    void initConnectedDevices(const QList<QScadaBoardInfo*>list);
    void initBoardForDeviceIp(QString);
    void updateBoardForDeviceIp(QString);
    QList<QScadaBoard*> getBoardList();
    QList<QScadaBoard*> getBoardListForDeviceIp(QString);

    void setEditingMode(bool);

signals:
    void objectDoubleClicked(QScadaObject*);

private slots:
    void showContextMenu(const QPoint&);
    void addNewObject();
    void bringToFront();
    void sendToBack();
    void showParameters();
    void updateObjectInfoDialog(QScadaObject *);
    void deleteObject(QScadaObjectInfo *);
    void updateSavedObject(QScadaObjectInfo *);
    void objectDoubleClickedHandler(QScadaObject*);

protected:
    void resizeEvent(QResizeEvent *event);

private:
    QScadaBoardManager *mBoardManager;
    QScadaBoard *mBoard;
    QScadaObjectInfoDialog *mParametersDialod;
    QList<int> mObjectIds;
};

#endif // VVIEWCONFIGURAOT_H
