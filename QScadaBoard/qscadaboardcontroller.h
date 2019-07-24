#ifndef VVIEWCONFIGURAOT_H
#define VVIEWCONFIGURAOT_H

#include <QWidget>
#include <QList>
#include <QGridLayout>

#include "../QScadaObject/qscadaobject.h"

class QScadaBoard;
class QScadaObjectInfoDialog;
class QScadaObjectInfo;
class QScadaObject;
class VUnitModel;
class QScadaBoardManager;
class QScadaBoardInfo;
class QScadaDeviceInfo;

class QScadaBoardController : public QWidget
{
    Q_OBJECT
public:
    explicit QScadaBoardController(QWidget *parent = nullptr);
    ~QScadaBoardController();

    //this method appends device to list and create all boards in list
    void appendDevice(QScadaDeviceInfo*);

    void clearBoard(QScadaBoard*);
    void clearAllBoards();

    void initConnectedDevices(const QList<QScadaBoardInfo*>list);
    void initBoardForDeviceIp(QString);
    void updateBoardForDeviceIp(QString);
    void initBoard(QScadaBoardInfo*);
    QList<QScadaBoard*> getBoardList();
    QList<QScadaBoard*> getBoardListForDeviceIp(QString);

    void setEditingMode(bool);

    QScadaObjectInfoDialog *getParametersDialod() const;
    void setParametersDialod(QScadaObjectInfoDialog *parametersDialod);

    void updateValue(QString deviceIp, int boardId, int id, QVariant value);
    void setPropertyWithId(QString deviceIp, int boardId, int id, QString property, QVariant value);

    void openProject(QString file);
    void saveProject(QString file);

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
    QGridLayout *mMainLayout;
    QList<QScadaDeviceInfo*> mDevices;
};

#endif // VVIEWCONFIGURAOT_H
