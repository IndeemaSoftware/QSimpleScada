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

    QTableWidget *lTableWidget = new QTableWidget(this);
    lTableWidget->setRowCount(properties.count());
    lTableWidget->setColumnCount(2);
    int i = 0;

    for (QString key : properties.keys()) {
        QTableWidgetItem *lNewItem = new QTableWidgetItem(key);
        lNewItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
        lTableWidget->setItem(i, 0, lNewItem);

        QTableWidgetItem *lNewValueItem = new QTableWidgetItem(properties.value(key).toString());
        lTableWidget->setItem(i, 1, lNewValueItem);

        i++;
    }
    ui->properties->setWidget(lTableWidget);
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
        ui->lineEditName->setText(mLatestObject->title());
        ui->checkBoxDynamic->setChecked(info->isDynamic());
        ui->spinBoxId->setValue(info->id());
        ui->checkBoxShowBackground->setChecked(info->showBackground());
        ui->checkBoxShowBackGroundImage->setChecked(mLatestObject->showBackgroundImage());

        mMarkerImage = mLatestObject->backGroundImage();

        showObjectProperties(info->UIProperties());
        //geometry
        geometryUpdated(mLatestObject);
    } else {
        lEnabled = false;
        //clear title
        ui->lineEditName->clear();

        //geometry
        ui->spinBoxX->clear();
        ui->spinBoxY->clear();
        ui->spinBoxWidth->clear();
        ui->spinBoxHeight->clear();
    }

    ui->checkBoxDynamic->setEnabled(lEnabled);
    ui->checkBoxShowBackground->setEnabled(lEnabled);
    ui->checkBoxShowBackGroundImage->setEnabled(lEnabled);
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
        //general
        mLatestObject->setTitle(ui->lineEditName->text());
        mLatestObject->setIsDynamic(ui->checkBoxDynamic->isChecked());
        mLatestObject->setId(ui->spinBoxId->value());

        //geometry
        int lX = ui->spinBoxX->value();
        int lY = ui->spinBoxY->value();
        int lWidth = ui->spinBoxWidth->value();
        int lHeight = ui->spinBoxHeight->value();

        mLatestObject->setGeometry(QRect(lX, lY, lWidth, lHeight));
        mLatestObject->setShowBackground(ui->checkBoxShowBackground->isChecked());
        mLatestObject->setShowBackgroundImage(ui->checkBoxShowBackGroundImage->isChecked());
        //axies

        if (!mMarkerImage.isEmpty()) {
            mLatestObject->setBackGroundImage(mMarkerImage);
        }

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

void QScadaObjectInfoDialog::on_pushButton_3_clicked()
{
    mMarkerImage = QFileDialog::getOpenFileName(this,
         tr("Open Image"), "/home/vshevchyk", tr("Image Files (*.png *.jpg *.bmp *.jpeg)"));
}
