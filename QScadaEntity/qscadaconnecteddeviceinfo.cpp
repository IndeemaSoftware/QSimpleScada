#include "../QScadaObject/qscadaobjectinfo.h"

#include "qscadaconnecteddeviceinfo.h"

#include "../QScadaObject/qscadaobject.h"
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

void QConnectedDeviceInfo::initFromXml(const QByteArray &xmlData) {
    QXmlStreamReader lXmlStreamReader(QString::fromStdString(xmlData.toStdString()));

    QScadaDeviceConfig *lDeviceConfig = nullptr;
    QScadaBoardInfo *lBoard = nullptr;
    QScadaObjectInfo *lObjectInfo = nullptr;
    QScadaObjectInfoAxis lAxis;

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
                lObjectInfo->setShowBackground((bool)lXmlStreamReader.readElementText().toInt());//new
            }  else if (lXmlStreamReader.name() == tag_show_background_image) {
                lObjectInfo->setShowBackgroundImage((bool)lXmlStreamReader.readElementText().toInt());//new
            }  else if (lXmlStreamReader.name() == tag_show_marker) {
                lObjectInfo->setShowMarkers((bool)lXmlStreamReader.readElementText().toInt());//new
            } else if (lXmlStreamReader.name() == tag_background_image) {
                lObjectInfo->setBackGroundImage(lXmlStreamReader.readElementText());//new
            } else if (lXmlStreamReader.name() == tag_is_dynamic) {
                lObjectInfo->setIsDynamic((bool)lXmlStreamReader.readElementText().toInt());///------
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
            } else if (lXmlStreamReader.name() == tag_axis_enabled) {
                lObjectInfo->setAxiesEnabled((bool)lXmlStreamReader.readElementText().toInt());//-----------------
            } else if (lXmlStreamReader.name() == tag_axis_position) {
                lObjectInfo->setAxisPosition((QScadaObjectAxisPosition)lXmlStreamReader.readElementText().toInt());//new
            } else if (lXmlStreamReader.name() == tag_axis_x) {
                lAxis.setX((QScadaObjectInfoAxisDirrection)lXmlStreamReader.readElementText().toInt());//-----------
                lObjectInfo->setAxis(lAxis);
            } else if (lXmlStreamReader.name() == tag_axis_y) {
                lAxis.setY((QScadaObjectInfoAxisDirrection)lXmlStreamReader.readElementText().toInt());
                lObjectInfo->setAxis(lAxis);
            } else if (lXmlStreamReader.name() == tag_axis_z) {
                lAxis.setZ((QScadaObjectInfoAxisDirrection)lXmlStreamReader.readElementText().toInt());
                lObjectInfo->setAxis(lAxis);
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
    QString rDevices = i.formTag(tag_devices, false, true, 0);//"<devices>\n";
    for (QScadaDeviceInfo info: deviceList) {
        rDevices += i.formTag(tag_device, false, true, 1);//"\t<device>\n";
        rDevices += i.formTagValue(tag_device_name, info.name(), true, 2);//"\t\t<name>" + info.name() + "</name>\n";
        rDevices += i.formTagValue(tag_device_ip, info.ip().toString(), true, 2);//"\t\t<ip>" + info.ip().toString() + "</ip>\n";
        rDevices += i.formTagValue(tag_device_ip_v6, info.ipv6().toString(), true, 2);//"\t\t<ip_v6>" + info.ipv6().toString() + "</ip_v6>\n";
        rDevices += i.formTagValue(tag_device_host, info.host(), true, 2);//"\t\t<host>" + info.host() + "</host>\n";
        rDevices += i.formTagValue(tag_device_domain, info.domain(), true, 2);//"\t\t<domain>" + info.domain() + "</domain>\n";
        rDevices += i.formTag(tag_device, true, true, 1);//"\t</device>\n";

        rDevices += i.formTag(tag_boards, false, true, 1);//"\t<boards>\n";
        for(QScadaBoard *board : boardController->getBoardListForDeviceIp(info.ip().toString())) {
            rDevices += i.formTag(tag_board, false, true, 2);//"\t\t<board>\n";
            for (QScadaObject *object : *board->objects()) {
                rDevices += i.formTag(tag_object, false, true, 3);//"\t\t\t<object>\n";
                rDevices += i.formTagValue(tag_title, object->info()->title(), true, 4);//"\t\t\t\t<title>"+ object->info()->title() + "</title>\n";
                rDevices += i.formTagValue(tag_id, QString::number(object->info()->id()), true, 4);//"\t\t\t\t<id>"+ QString::number(object->info()->id()) + "</id>\n";
                rDevices += i.formTagValue(tag_is_dynamic, QString::number(object->info()->isDynamic()), true, 4);//"\t\t\t\t<is_dynamic>"+ QString::number(object->info()->isDynamic()) + "</is_dynamic>\n";
                rDevices += i.formTagValue(tag_show_background, QString::number(object->info()->showBackground()), true, 4);
                rDevices += i.formTagValue(tag_show_background_image, QString::number(object->info()->showBackgroundImage()), true, 4);
                rDevices += i.formTagValue(tag_show_marker, QString::number(object->info()->showMarkers()), true, 4);
                rDevices += i.formTagValue(tag_background_image, object->info()->backGroundImage(), true, 4);
                rDevices += i.formTagValue(tag_geometry_x, QString::number(object->info()->geometry().x()), true, 4);//"\t\t\t\t<geometry_x>"+ QString::number(object->info()->geometry().x()) + "</geometry_x>\n";
                rDevices += i.formTagValue(tag_geometry_y, QString::number(object->info()->geometry().y()), true, 4);//"\t\t\t\t<geometry_y>"+ QString::number(object->info()->geometry().y()) + "</geometry_y>\n";
                rDevices += i.formTagValue(tag_geometry_width, QString::number(object->info()->geometry().width()), true, 4);//"\t\t\t\t<geometry_width>"+ QString::number(object->info()->geometry().width()) + "</geometry_width>\n";
                rDevices += i.formTagValue(tag_geometry_height, QString::number(object->info()->geometry().height()), true, 4);//"\t\t\t\t<geometry_height>"+ QString::number(object->info()->geometry().height()) + "</geometry_height>\n";
                rDevices += i.formTagValue(tag_axis_enabled, QString::number(object->info()->axiesEnabled()), true, 4);//"\t\t\t\t<axis_enabled>"+ QString::number(object->info()->axiesEnabled()) + "</axis_enabled>\n";
                rDevices += i.formTagValue(tag_axis_position, QString::number(object->info()->axisPosition()), true, 4);
                rDevices += i.formTagValue(tag_axis_x, QString::number(object->info()->axis().getX()), true, 4);//"\t\t\t\t<axis_x>"+ QString::number(object->info()->axis().getX()) + "</axis_x>\n";
                rDevices += i.formTagValue(tag_axis_y, QString::number(object->info()->axis().getY()), true, 4);//"\t\t\t\t<axis_y>"+ QString::number(object->info()->axis().getY()) + "</axis_y>\n";
                rDevices += i.formTagValue(tag_axis_z, QString::number(object->info()->axis().getZ()), true, 4);//"\t\t\t\t<axis_z>"+ QString::number(object->info()->axis().getZ()) + "</axis_z>\n";
                rDevices += i.formTag(tag_object, true, true, 3);//"\t\t\t</object>\n";
            }
            rDevices += i.formTag(tag_board, true, true, 2);//"\t\t</board>\n";
        }
        rDevices += i.formTag(tag_boards, true, true, 1);//"\t</boards>\n";
    }
    rDevices += i.formTag(tag_devices, true, true, 0);//"</devices>";

    return rDevices;
}
