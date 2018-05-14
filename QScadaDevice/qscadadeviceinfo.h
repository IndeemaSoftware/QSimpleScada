#ifndef VDEVICEINFO_H
#define VDEVICEINFO_H

#include <QHostAddress>

#include "../qscadaconfig.h"

class QZeroConfService;

class QScadaDeviceInfo
{
public:
    void init(QScadaDeviceInfo *);

    QString name() const;
    void setName(const QString &name);

    QString type() const;
    void setType(const QString &type);

    QString domain() const;
    void setDomain(const QString &domain);

    QString host() const;
    void setHost(const QString &host);

    QHostAddress ip() const;
    void setIp(const QHostAddress &ip);

    QHostAddress ipv6() const;
    void setIpv6(const QHostAddress &ipv6);

    quint32 interfaceIndex() const;
    void setInterfaceIndex(const quint32 &interfaceIndex);

    quint16 port() const;
    void setPort(const quint16 &port);

    bool operator ==(const QScadaDeviceInfo& other);

    QScadaStatus deviceStatus() const;
    void setDeviceStatus(const QScadaStatus &deviceStatus);

    int unitCount() const;
    void setUnitCount(int unitCount);

    bool operator<(const QScadaDeviceInfo &deviceInfo);

private:
    QString mName;
    QString	mType;
    QString	mDomain;
    QString	mHost;
    QHostAddress mIp;
    QHostAddress mIpv6;
    quint32 mInterfaceIndex;
    quint16	mPort;
    QScadaStatus mDeviceStatus;
    int mUnitCount;
};

#endif // VDEVICEINFO_H
