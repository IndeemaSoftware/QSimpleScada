#include "qscadaobjectqml.h"
#include "qscadaobjectinfo.h"

#include <QQuickWidget>
#include <QVBoxLayout>
#include <QQuickItem>

const char *QScadaObjectQML::tagId = "id";
const char *QScadaObjectQML::tagFrom = "from";
const char *QScadaObjectQML::tagTo = "to";
const char *QScadaObjectQML::tagValue = "value";

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
    QMetaObject::invokeMethod(mQMLObject, "update",
        Q_RETURN_ARG(QVariant, rReturn),
        Q_ARG(QVariant, value));
}

void QScadaObjectQML::update()
{
    QScadaObject::update();

    this->updateQMLGeometry();
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

//private methods
void QScadaObjectQML::setFrom(qreal from)
{
    mQMLObject->setProperty(QScadaObjectQML::tagFrom, QVariant(from));
}

void QScadaObjectQML::setTo(qreal to)
{
    mQMLObject->setProperty(QScadaObjectQML::tagTo, QVariant(to));
}

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

    this->updateQMLGeometry();

    mMetaData = mQMLObject->property("metaData").toStringList();
    qDebug() << mMetaData;
}

void QScadaObjectQML::dynamicStatusChanged(QScadaObjectInfo *)
{

}

QStringList QScadaObjectQML::metaData() const
{
    return mMetaData;
}
