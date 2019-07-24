#ifndef VBOARDMANAGER_H
#define VBOARDMANAGER_H

#include <QObject>
#include <QMap>

class QScadaBoard;
class QScadaBoardController;
class QScadaDeviceInfo;

class QScadaBoardManager : public QObject
{
    Q_OBJECT
public:
    explicit QScadaBoardManager(QObject *parent = nullptr);

    QScadaBoard* getBoard(QString deviceIp, int boardId);
    QScadaBoard* getBoard(QScadaDeviceInfo *device, int boardId);
    QScadaBoard* initBoardForDeviceIp(QString deviceIp);

    QList<QScadaBoard*> getBoardListForDeviceIp(QString);
    QList<QScadaBoard*> getBoardList();

    //if device with the same ip address exists appeding will be ignored
    void appendDevice(QScadaDeviceInfo *deviceInfo);

private:
    QScadaDeviceInfo* deviceForIp(QString ip);
    int generateIdForNewBoard();

private:
    QMap<int, QScadaBoard*> mBoards;//list of devices with unique id
    QList<QScadaDeviceInfo*> mDevices;//list of devices
};

#endif // VBOARDMANAGER_H
