#ifndef VBOARDMANAGER_H
#define VBOARDMANAGER_H

#include <QObject>
#include <QMap>

class QScadaBoard;
class QScadaBoardController;
class QScadaDeviceInfo;
class QScadaBoardInfo;

class QScadaBoardManager : public QObject
{
    Q_OBJECT
public:
    explicit QScadaBoardManager(QObject *parent = nullptr);

    QScadaBoard* getBoard(QString deviceIp, int boardId);
    QScadaBoard* getBoard(QScadaDeviceInfo *device, int boardId);
    QScadaBoard* initBoardForDeviceIp(QString deviceIp);
    QScadaBoard* initBoardForDeviceIp(QString deviceIp, QScadaBoardInfo*);

    void resetAll();

    QList<QScadaBoard*> getBoardListForDeviceIp(QString);
    QList<QScadaBoard*> getBoardList();

    //if device with the same ip address exists appeding will be ignored
    //this method appends device to list and create all boards in list
    void appendDevice(QScadaDeviceInfo *deviceInfo);

    QList<QScadaDeviceInfo *> getDevices() const;

private:
    QScadaDeviceInfo* deviceForIp(QString ip);
    int generateIdForNewBoard();
    QScadaBoard *initBoardWithId(int id);
    QScadaBoard *initBoardWithInfo(QScadaBoardInfo*);

private:
    QMap<int, QScadaBoard*> mBoards;//list of devices with unique id
    QList<QScadaDeviceInfo*> mDevices;//list of devices
};

#endif // VBOARDMANAGER_H
