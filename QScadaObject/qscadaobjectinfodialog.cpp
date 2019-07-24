#include "qscadaobjectinfodialog.h"
#include "ui_qscadaobjectinfodialog.h"

#include "qscadaobjectinfo.h"

#include <QFileDialog>
#include <QDebug>
#include <QBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem>

QScadaObjectInfoDialog::QScadaObjectInfoDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QScadaObjectInfoDialog),
    mLatestObject{nullptr}
{
    ui->setupUi(this);
}

QScadaObjectInfoDialog::~QScadaObjectInfoDialog()
{
    delete ui;
}

void QScadaObjectInfoDialog::showObjectProperties(QMultiMap<QString, QVariant> properties)
{
    mProperties = properties;

    ui->tableWidget->setRowCount(properties.count());
    ui->tableWidget->setColumnCount(2);
    int i = 0;

    for (QString key : properties.keys()) {
        QTableWidgetItem *lNewItem = new QTableWidgetItem(key);
        lNewItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
        ui->tableWidget->setItem(i, 0, lNewItem);

        QTableWidgetItem *lNewValueItem = new QTableWidgetItem(properties.value(key).toString());
        ui->tableWidget->setItem(i, 1, lNewValueItem);

        i++;
    }
}

void QScadaObjectInfoDialog::updateWithObjectInfo(QScadaObjectInfo *info)
{
    if (mLatestObject != nullptr) {
        disconnect(mLatestObject, SIGNAL(geometryChanged(QScadaObjectInfo*)), this, SLOT(geometryUpdated(QScadaObjectInfo *)));
    }
    mLatestObject = info;
    bool lEnabled = true;;

    if (mLatestObject != nullptr) {
        //General
        connect(mLatestObject, SIGNAL(geometryChanged(QScadaObjectInfo*)), this, SLOT(geometryUpdated(QScadaObjectInfo *)));
        ui->spinBoxId->setValue(info->id());
        ui->checkBoxShowBackground->setChecked(info->showBackground());

        showObjectProperties(info->UIProperties());
        //geometry
        geometryUpdated(mLatestObject);
    } else {
        lEnabled = false;

        //geometry
        ui->spinBoxX->clear();
        ui->spinBoxY->clear();
        ui->spinBoxWidth->clear();
        ui->spinBoxHeight->clear();

        ui->tableWidget->clear();
    }

    ui->checkBoxShowBackground->setEnabled(lEnabled);
    ui->spinBoxId->setEnabled(lEnabled);
}

void QScadaObjectInfoDialog::geometryUpdated(QScadaObjectInfo *info)
{
    ui->spinBoxX->setValue(info->geometry().x());
    ui->spinBoxY->setValue(info->geometry().y());
    ui->spinBoxWidth->setValue(info->geometry().width());
    ui->spinBoxHeight->setValue(info->geometry().height());
}
//save button pressed
void QScadaObjectInfoDialog::on_pushButton_2_pressed()
{
    if (mLatestObject != nullptr) {
        //properties
        for (int i=0; i< ui->tableWidget->rowCount(); i++) {
            QTableWidgetItem *lKeyItem  = ui->tableWidget->item(i, 0);//taking лун
            QTableWidgetItem *lValueItem  = ui->tableWidget->item(i, 1);//taking values
            mProperties.replace(lKeyItem->text(), QVariant::fromValue<QString>(lValueItem->text()));//replacing all values
        }
        mLatestObject->setUIProperties(mProperties);

        //general
        mLatestObject->setId(ui->spinBoxId->value());

        //geometry
        int lX = ui->spinBoxX->value();
        int lY = ui->spinBoxY->value();
        int lWidth = ui->spinBoxWidth->value();
        int lHeight = ui->spinBoxHeight->value();

        mLatestObject->setGeometry(QRect(lX, lY, lWidth, lHeight));
        mLatestObject->setShowBackground(ui->checkBoxShowBackground->isChecked());

        if (mLatestObject != nullptr) {
            emit savePressed(mLatestObject);
        }
    }
}
//delete button pressed
void QScadaObjectInfoDialog::on_pushButton_pressed()
{
    if (mLatestObject != nullptr) {
        emit deletePressed(mLatestObject);
        mLatestObject = nullptr;
    }
}
