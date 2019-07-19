#include "../QScadaObject/qscadaobjectinfo.h"

#include "qscadaconnecteddeviceinfo.h"

#include "../QScadaObject/qscadaobject.h"
#include "../QScadaObject/qscadaobjectqml.h"
#include "../QScadaBoard/qscadaboard.h"

#include <QByteArray>
#include <QDebug>
#include <QFile>

const QString QConnectedDeviceInfo::tag_devices = QString("devices");
const QString QConnectedDeviceInfo::tag_device = QString("device");
const QString QConnectedDeviceInfo::tag_device_name = QString("name");
const QString QConnectedDeviceInfo::tag_device_ip = QString("ip");
const QString QConnectedDeviceInfo::tag_device_ip_v6 = QString("ip_v6");
const QString QConnectedDeviceInfo::tag_device_host = QString("host");
const QString QConnectedDeviceInfo::tag_device_domain = QString("domain");

const QString QConnectedDeviceInfo::tag_boards = QString("boards");
const QString QConnectedDeviceInfo::tag_board = QString("board");
const QString QConnectedDeviceInfo::tag_object = QString("object");
const QString QConnectedDeviceInfo::tag_title = QString("title");
const QString QConnectedDeviceInfo::tag_show_background = QString("show_background");//new
const QString QConnectedDeviceInfo::tag_show_background_image = QString("show_background_image"); //
const QString QConnectedDeviceInfo::tag_show_marker = QString("show_marker");//new
const QString QConnectedDeviceInfo::tag_background_image = QString("background_image");//new
const QString QConnectedDeviceInfo::tag_id = QString("id");
const QString QConnectedDeviceInfo::tag_is_dynamic = QString("is_dynamic");
const QString QConnectedDeviceInfo::tag_geometry_x = QString("geometry_x");
const QString QConnectedDeviceInfo::tag_geometry_y = QString("geometry_y");
const QString QConnectedDeviceInfo::tag_geometry_width = QString("geometry_width");
const QString QConnectedDeviceInfo::tag_geometry_height = QString("geometry_height");
const QString QConnectedDeviceInfo::tag_axis_enabled = QString("axis_enabled");
const QString QConnectedDeviceInfo::tag_axis_position = QString("axis_position");//new
const QString QConnectedDeviceInfo::tag_axis_x = QString("axis_x");
const QString QConnectedDeviceInfo::tag_axis_y = QString("axis_y");
const QString QConnectedDeviceInfo::tag_axis_z = QString("axis_z");
const QString QConnectedDeviceInfo::order_level = QString("order_level");
const QString QConnectedDeviceInfo::properties = QString("properties");

QConnectedDeviceInfo::QConnectedDeviceInfo(QObject *parent):
    QScadaBasePrefEntity(parent) {
}

QString QConnectedDeviceInfo::formTag(QString tag, bool isClosing, bool newLine, int numOfTabs)
{
    QString lReturn;

    if (isClosing) {
        lReturn = "/";
    }

    lReturn.append(tag);
    lReturn = QString("<").append(lReturn).append(">");

    int i = 0;
    while (numOfTabs > i) {
        lReturn = QString("\t").append(lReturn);
        i++;
    }

    if (newLine) {
        lReturn.append("\n");
    }

    return lReturn;
}

QString QConnectedDeviceInfo::formTagValue(QString tag, QString value, bool newLine, int numOfTabs)
{
    QString lReturn;

    lReturn = formTag(tag, false, false, numOfTabs);
    lReturn.append(value).append(formTag(tag, true, newLine, 0));

    return lReturn;
}

QString QConnectedDeviceInfo::formProperties(QMultiMap<QString, QVariant> prop)
{
    QString rProperties;
    rProperties = "\t\t\t\t<" + QConnectedDeviceInfo::properties +">\n";
    for (QString key : prop.keys()) {
        rProperties += "\t\t\t\t\t<" + key + ">" + prop.value(key).toString() + "</" + key + ">\n";
    }
    rProperties += "\t\t\t\t</" + QConnectedDeviceInfo::properties + ">\n";

    return rProperties;
}

void QConnectedDeviceInfo::initFromXml(const QByteArray &xmlData) {
    QXmlStreamReader lXmlStreamReader(QString::fromStdString(xmlData.toStdString()));

    QScadaDeviceConfig *lDeviceConfig = nullptr;
    QScadaBoardInfo *lBoard = nullptr;
    QScadaObjectInfo *lObjectInfo = nullptr;

    while (!lXmlStreamReader.atEnd() && !lXmlStreamReader.hasError()) {
        QXmlStreamReader::TokenType tokenType = lXmlStreamReader.readNext();

        if (tokenType == QXmlStreamReader::StartDocument) {
            continue;
        } else if (tokenType == QXmlStreamReader::StartElement){
            if (lXmlStreamReader.name() == tag_device) {
                lDeviceConfig = new QScadaDeviceConfig();
            } else if (lXmlStreamReader.name() == tag_device_name) {
                lDeviceConfig->name = lXmlStreamReader.readElementText();
            } else if (lXmlStreamReader.name() == tag_device_ip) {
                lDeviceConfig->ip = QHostAddress(lXmlStreamReader.readElementText());
            } else if (lXmlStreamReader.name() == tag_device_ip_v6) {
                lDeviceConfig->ipv6 = QHostAddress(lXmlStreamReader.readElementText());
            } else if (lXmlStreamReader.name()== tag_device_host) {
                lDeviceConfig->host = lXmlStreamReader.readElementText();
            } else if (lXmlStreamReader.name() == tag_device_domain) {
                lDeviceConfig->domain = lXmlStreamReader.readElementText();
            }
            //parsing boards data
            else if (lXmlStreamReader.name() == tag_board) {
                lBoard = new QScadaBoardInfo();
                lDeviceConfig->boardList.append(lBoard);
            } else if (lXmlStreamReader.name() == tag_object) {
                lObjectInfo = new QScadaObjectInfo();
                lBoard->appendObjectInfo(lObjectInfo);
            } else if (lXmlStreamReader.name() == tag_title) {
                lObjectInfo->setTitle(lXmlStreamReader.readElementText());
            } else if (lXmlStreamReader.name() == tag_id) {
                lObjectInfo->setId(lXmlStreamReader.readElementText().toInt());//----------------------
            } else if (lXmlStreamReader.name() == tag_show_background) {
                lObjectInfo->setShowBackground(static_cast<bool>(lXmlStreamReader.readElementText().toInt()));//new
            }  else if (lXmlStreamReader.name() == tag_show_background_image) {
                lObjectInfo->setShowBackgroundImage(static_cast<bool>(lXmlStreamReader.readElementText().toInt()));//new
            } else if (lXmlStreamReader.name() == tag_background_image) {
                lObjectInfo->setBackGroundImage(lXmlStreamReader.readElementText());//new
            } else if (lXmlStreamReader.name() == tag_is_dynamic) {
                lObjectInfo->setIsDynamic(static_cast<bool>(lXmlStreamReader.readElementText().toInt()));///------
            } else if (lXmlStreamReader.name() == tag_geometry_x) {
                QRect lRect = lObjectInfo->geometry();
                lRect.setRect(lXmlStreamReader.readElementText().toInt(),
                              lRect.y(),
                              lRect.width(),
                              lRect.height());

                lObjectInfo->setGeometry(lRect);
            } else if (lXmlStreamReader.name() == tag_geometry_y) {
                QRect lRect = lObjectInfo->geometry();
                lRect.setRect(lRect.x(),
                              lXmlStreamReader.readElementText().toInt(),
                              lRect.width(),
                              lRect.height());

                lObjectInfo->setGeometry(lRect);
            } else if (lXmlStreamReader.name() == tag_geometry_width) {
                QRect lRect = lObjectInfo->geometry();
                lRect.setRect(lRect.x(),
                              lRect.y(),
                              lXmlStreamReader.readElementText().toInt(),
                              lRect.height());

                lObjectInfo->setGeometry(lRect);
            } else if (lXmlStreamReader.name() == tag_geometry_height) {
                QRect lRect = lObjectInfo->geometry();
                lRect.setRect(lRect.x(),
                              lRect.y(),
                              lRect.width(),
                              lXmlStreamReader.readElementText().toInt());

                lObjectInfo->setGeometry(lRect);
            } else if (lXmlStreamReader.name() == order_level) {
                lObjectInfo->setOrderLevel(lXmlStreamReader.readElementText().toInt());
            }

        } else if (tokenType == QXmlStreamReader::EndElement) {
            if (lXmlStreamReader.name() == tag_device) {
                if (lDeviceConfig != nullptr) {
                    connecteDeviceList.append(lDeviceConfig);
                }
            }
        }
    }
}

void QConnectedDeviceInfo::saveXmlToFile(const QString &filePath)
{
    qDebug() << __FUNCTION__;
    QFile lFile(filePath);
    if (lFile.open(QIODevice::WriteOnly | QIODevice::Text)) {

        QString lDevices;

        lDevices = "<devices>\n";
        for (QScadaDeviceConfig *deviceConfig : connecteDeviceList) {
            lDevices += "<device>\n";
            lDevices += "\t<name>" + deviceConfig->name + "</name>\n";
            lDevices += "\t<ip>" + deviceConfig->ip.toString() + "</ip>\n";
            lDevices += "\t<ip_v6>" + deviceConfig->ipv6.toString() + "</ip_v6>\n";
            lDevices += "\t<host>" + deviceConfig->host + "</host>\n";
            lDevices += "\t<domain>" + deviceConfig->domain + "</domain>\n";
            lDevices += "</device>\n";
        }
        lDevices += "</devices>";

        QTextStream lTextStream(&lFile);
        lTextStream << lDevices;

        lFile.flush();
        lFile.close();
    } else {
        qDebug() << "       - Error open file -> " << lFile.fileName();
    }
}

QString QConnectedDeviceInfo::XMLFromDeviceInfo(QList<QScadaDeviceInfo> deviceList, QScadaBoardController *boardController)
{
    //create xml for devices
    QConnectedDeviceInfo i;
    QString rDevices = i.formTag(tag_devices, false, true, 0);
    for (QScadaDeviceInfo info: deviceList) {
        rDevices += i.formTag(tag_device, false, true, 1);
        rDevices += i.formTagValue(tag_device_name, info.name(), true, 2);
        rDevices += i.formTagValue(tag_device_ip, info.ip().toString(), true, 2);
        rDevices += i.formTagValue(tag_device_ip_v6, info.ipv6().toString(), true, 2);
        rDevices += i.formTagValue(tag_device_host, info.host(), true, 2);
        rDevices += i.formTagValue(tag_device_domain, info.domain(), true, 2);
        rDevices += i.formTag(tag_device, true, true, 1);//"\t</device>\n";

        rDevices += i.formTag(tag_boards, false, true, 1);
        for(QScadaBoard *board : boardController->getBoardListForDeviceIp(info.ip().toString())) {
            rDevices += i.formTag(tag_board, false, true, 2);
            for (QScadaObject *object : *board->objects()) {
                rDevices += i.formTag(tag_object, false, true, 3);
                rDevices += i.formTagValue(tag_title, object->info()->title(), true, 4);
                rDevices += i.formTagValue(tag_id, QString::number(object->info()->id()), true, 4);
                rDevices += i.formTagValue(tag_is_dynamic, QString::number(object->info()->isDynamic()), true, 4);
                rDevices += i.formTagValue(tag_show_background, QString::number(object->info()->showBackground()), true, 4);
                rDevices += i.formTagValue(tag_show_background_image, QString::number(object->info()->showBackgroundImage()), true, 4);
                rDevices += i.formTagValue(tag_background_image, object->info()->backGroundImage(), true, 4);
                rDevices += i.formTagValue(tag_geometry_x, QString::number(object->info()->geometry().x()), true, 4);
                rDevices += i.formTagValue(tag_geometry_y, QString::number(object->info()->geometry().y()), true, 4);
                rDevices += i.formTagValue(tag_geometry_width, QString::number(object->info()->geometry().width()), true, 4);
                rDevices += i.formTagValue(tag_geometry_height, QString::number(object->info()->geometry().height()), true, 4);
                rDevices += i.formTagValue(order_level, QString::number(object->info()->orderLevel()), true, 4);
                if (object->info()->type() == QScadaObjectTypeQML) {
                    rDevices += QConnectedDeviceInfo::formProperties(static_cast<QScadaObjectQML*>(object)->QMLProperties());
                }
                rDevices += i.formTag(tag_object, true, true, 3);//"\t\t\t</object>\n";
            }
            rDevices += i.formTag(tag_board, true, true, 2);//"\t\t</board>\n";
        }
        rDevices += i.formTag(tag_boards, true, true, 1);//"\t</boards>\n";
    }
    rDevices += i.formTag(tag_devices, true, true, 0);//"</devices>";

    return rDevices;
}
