#ifndef VBOARDMANAGER_H
#define VBOARDMANAGER_H

#include <QObject>
#include <QMap>

class QScadaBoard;

class QScadaBoardManager : public QObject
{
    Q_OBJECT
public:
    explicit QScadaBoardManager(QObject *parent = 0);

    QScadaBoard* getBoardForDeviceWithIp(QString);

    QList<QScadaBoard*> getBoardList();

private:
    QMap<QString, QScadaBoard*> mBoardMap;
};

#endif // VBOARDMANAGER_H
