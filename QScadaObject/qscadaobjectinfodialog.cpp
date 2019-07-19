#include "qscadaobjectinfodialog.h"
#include "ui_qscadaobjectinfodialog.h"

#include "qscadaobjectinfo.h"
#include "QFileDialog"
#include "QDebug"

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

void QScadaObjectInfoDialog::updateWithObjectInfo(QScadaObjectInfo *info)
{
    if (mLatestObject != nullptr) {
        disconnect(mLatestObject, SIGNAL(geometryChanged(QScadaObjectInfo*)), this, SLOT(geometryUpdated(QScadaObjectInfo *)));
    }
    mLatestObject = info;

    if (mLatestObject != nullptr) {
        //General
        connect(mLatestObject, SIGNAL(geometryChanged(QScadaObjectInfo*)), this, SLOT(geometryUpdated(QScadaObjectInfo *)));
        ui->lineEditName->setText(mLatestObject->title());
        ui->checkBoxDynamic->setChecked(info->isDynamic());
        ui->spinBoxId->setValue(info->id());
        ui->checkBoxShowBackground->setChecked(info->showBackground());
        ui->checkBoxShowBackGroundImage->setChecked(mLatestObject->showBackgroundImage());

        mMarkerImage = mLatestObject->backGroundImage();

        //geometry
        geometryUpdated(mLatestObject);
    } else {
        //clear title
        ui->lineEditName->clear();

        //geometry
        ui->spinBoxX->clear();
        ui->spinBoxY->clear();
        ui->spinBoxWidth->clear();
        ui->spinBoxHeight->clear();
    }
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

void QScadaObjectInfoDialog::on_checkBoxAxis_stateChanged(int arg1)
{
    bool lEnabled = false;
    switch(arg1) {
    case Qt::Unchecked: {
        lEnabled = false;
        break;
    }
    case Qt::Checked:{
        lEnabled = true;
        break;
    }
    }
}

void QScadaObjectInfoDialog::on_pushButton_3_clicked()
{
    mMarkerImage = QFileDialog::getOpenFileName(this,
         tr("Open Image"), "/home/vshevchyk", tr("Image Files (*.png *.jpg *.bmp *.jpeg)"));
}
