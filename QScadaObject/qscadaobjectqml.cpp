#include "qscadaobjectqml.h"
#include "qscadaobjectinfo.h"

#include <QQuickWidget>
#include <QVBoxLayout>

QScadaObjectQML::QScadaObjectQML(QScadaObjectInfo *info, QWidget *parent) : QScadaObject(info, parent)
{
    QQuickWidget *lQmlWidget = new QQuickWidget();
    lQmlWidget->setSource(QUrl::fromLocalFile(info->uiResourcePath()));
    lQmlWidget->show();

    QVBoxLayout *lLayout = new QVBoxLayout(this);
    lLayout->setContentsMargins(0, 0, 0, 0);
    lLayout->addWidget(lQmlWidget);
}
