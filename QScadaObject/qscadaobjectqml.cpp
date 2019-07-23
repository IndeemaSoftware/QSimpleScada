#include "qscadaobjectqml.h"
#include "qscadaobjectinfo.h"

#include <QQuickWidget>
#include <QVBoxLayout>
#include <QQuickItem>
#include <QJsonObject>

const char *QScadaObjectQML::funcUpdate = "update";
const char *QScadaObjectQML::tagMetaData = "metaData";

QScadaObjectQML::QScadaObjectQML(QScadaObjectInfo *info, QWidget *parent) :
    QScadaObject(info, parent),
    mQMLObject{nullptr}
{
    initFromQML(info);
}

void QScadaObjectQML::setProperty(QString key, QVariant value)
{
    setProperty(key.toLocal8Bit().data(), value);
}

void QScadaObjectQML::setProperty(char *key, QVariant value)
{
    mQMLObject->setProperty(key, value);
}

void QScadaObjectQML::updateValue(QVariant value)
{
    QVariant rReturn;
    QMetaObject::invokeMethod(mQMLObject, QScadaObjectQML::funcUpdate,
        Q_RETURN_ARG(QVariant, rReturn),
        Q_ARG(QVariant, value));
}

void QScadaObjectQML::update()
{
    QScadaObject::update();

    this->updateQMLGeometry();
    this->updateQMLProperties();
}

QQuickItem *QScadaObjectQML::QMLObject() const
{
    return mQMLObject;
}

void QScadaObjectQML::setQMLObject(QQuickItem *QMLObject)
{
    mQMLObject = QMLObject;
}

void QScadaObjectQML::updateQMLGeometry()
{
    mQMLObject->setX(0);
    mQMLObject->setX(0);
    mQMLObject->setWidth(this->width());
    mQMLObject->setHeight(this->height());
}

void QScadaObjectQML::updateQMLProperties()
{
    for (QString property : info()->UIProperties().keys()) {
        this->setProperty(property, info()->UIProperties().value(property));
    }
}

//private methods
void QScadaObjectQML::initFromQML(QScadaObjectInfo *info)
{
    QQuickWidget *lQmlWidget = new QQuickWidget();
    lQmlWidget->setClearColor(Qt::transparent);
    lQmlWidget->setSource(QUrl::fromLocalFile(info->uiResourcePath()));
    lQmlWidget->show();

    mQMLObject = static_cast<QQuickItem*>(lQmlWidget->rootObject());

    QVBoxLayout *lLayout = new QVBoxLayout(this);
    lLayout->setContentsMargins(0, 0, 0, 0);
    lLayout->addWidget(lQmlWidget);

    if (info->UIProperties().keys().count() == 0) {
        info->setUIProperties(this->QMLProperties());
;    } else {
        updateQMLProperties();
    }

    this->updateQMLGeometry();
}

void QScadaObjectQML::dynamicStatusChanged(QScadaObjectInfo *)
{

}

QMultiMap<QString, QVariant> QScadaObjectQML::QMLProperties() const
{
    QMultiMap<QString, QVariant> rProp;

    for (QVariant key : mQMLObject->property(QScadaObjectQML::tagMetaData).toList()) {
        rProp.insert(key.toString(), mQMLObject->property(key.toByteArray().data()));
    }

    return rProp;
}

