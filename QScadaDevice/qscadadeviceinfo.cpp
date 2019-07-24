#include "qscadadeviceinfo.h"

void QScadaDeviceInfo::init(QScadaDeviceInfo *deviceInfo)
{
    setName(deviceInfo->name());
    setType(deviceInfo->type());
    setDomain(deviceInfo->domain());
    setHost(deviceInfo->host());
    setIp(deviceInfo->ip());
    setIpv6(deviceInfo->ipv6());
    setInterfaceIndex(deviceInfo->interfaceIndex());
    setPort(deviceInfo->port());
    setDeviceStatus(QScadaStatusGray);
    setUnitCount(0);
}

QString QScadaDeviceInfo::name() const
{
    return mName;
}

void QScadaDeviceInfo::setName(const QString &name)
{
    mName = name;
}

QString QScadaDeviceInfo::type() const
{
    return mType;
}

void QScadaDeviceInfo::setType(const QString &type)
{
    mType = type;
}

QString QScadaDeviceInfo::domain() const
{
    return mDomain;
}

void QScadaDeviceInfo::setDomain(const QString &domain)
{
    mDomain = domain;
}

QString QScadaDeviceInfo::host() const
{
    return mHost;
}

void QScadaDeviceInfo::setHost(const QString &host)
{
    mHost = host;
}

QHostAddress QScadaDeviceInfo::ip() const
{
    return mIp;
}

void QScadaDeviceInfo::setIp(const QHostAddress &ip)
{
    mIp = ip;
}

QHostAddress QScadaDeviceInfo::ipv6() const
{
    return mIpv6;
}

void QScadaDeviceInfo::setIpv6(const QHostAddress &ipv6)
{
    mIpv6 = ipv6;
}

quint32 QScadaDeviceInfo::interfaceIndex() const
{
    return mInterfaceIndex;
}

void QScadaDeviceInfo::setInterfaceIndex(const quint32 &interfaceIndex)
{
    mInterfaceIndex = interfaceIndex;
}

quint16 QScadaDeviceInfo::port() const
{
    return mPort;
}

void QScadaDeviceInfo::setPort(const quint16 &port)
{
    mPort = port;
}

bool QScadaDeviceInfo::operator ==(const QScadaDeviceInfo &other)
{
    return mName == other.mName && mType == other.mType && mHost == other.mHost &&
            mInterfaceIndex == other.mInterfaceIndex && mIp == other.mIp && mIpv6 == other.mIpv6 && mDomain == other.mDomain &&
            mHost == other.mHost && mDeviceStatus == other.mDeviceStatus && mUnitCount == other.mUnitCount;
}

QScadaStatus QScadaDeviceInfo::deviceStatus() const
{
    return mDeviceStatus;
}

void QScadaDeviceInfo::setDeviceStatus(const QScadaStatus &deviceStatus)
{
    mDeviceStatus = deviceStatus;
}

int QScadaDeviceInfo::unitCount() const
{
    return mUnitCount;
}

void QScadaDeviceInfo::setUnitCount(int unitCount)
{
    mUnitCount = unitCount;
}

bool QScadaDeviceInfo::operator<(const QScadaDeviceInfo &deviceInfo)
{
    QString lName = name().remove(" ");
    QString rName = deviceInfo.name().remove(" ");
    bool rValue = lName.compare(rName, Qt::CaseInsensitive) < 0 ? true : false;

    return rValue;
}

QList<int> QScadaDeviceInfo::boardIds() const
{
    return mBoardIds;
}

void QScadaDeviceInfo::appendBoardId(int boardId)
{
    mBoardIds.append(boardId);
}
