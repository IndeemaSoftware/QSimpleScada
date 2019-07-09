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

    initAxiesList();
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
        ui->checkBoxShowMarkers->setChecked(info->showMarkers());
        ui->checkBoxShowBackGroundImage->setChecked(mLatestObject->showBackgroundImage());

        mMarkerImage = mLatestObject->backGroundImage();

        //axies
        QScadaObjectInfoAxis lAxis = mLatestObject->axis();
        enableAxis(info->axiesEnabled());
        ui->comboBoxAxisPosition->setCurrentText(info->axisPosition() == QObjectAxisPositionLeft? "Left" : "Right");
        ui->comboBoxX->setCurrentText(lAxis.getStringX());
        ui->comboBoxY->setCurrentText(lAxis.getStringY());
        ui->comboBoxZ->setCurrentText(lAxis.getStringZ());

        //geometry
        geometryUpdated(mLatestObject);
    } else {
        //clear title
        ui->lineEditName->clear();

        //clear axies
        enableAxis(false);

        //geometry
        ui->spinBoxX->clear();
        ui->spinBoxY->clear();
        ui->spinBoxWidth->clear();
        ui->spinBoxHeight->clear();
    }
}

void QScadaObjectInfoDialog::enableAxis(bool enable)
{
    ui->checkBoxAxis->setChecked(enable);

    ui->comboBoxAxisPosition->setEnabled(enable);

    ui->comboBoxX->setEnabled(enable);
    ui->comboBoxY->setEnabled(enable);
    ui->comboBoxZ->setEnabled(enable);
}

void QScadaObjectInfoDialog::initAxiesList()
{
    ui->comboBoxX->clear();
    ui->comboBoxY->clear();
    ui->comboBoxZ->clear();

    QStringList lList;
    lList.append(AXIS_UP);
    lList.append(AXIS_ASIDE);
    lList.append(AXIS_INSIDE);

    ui->comboBoxX->addItems(lList);
    ui->comboBoxY->addItems(lList);
    ui->comboBoxZ->addItems(lList);
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
        mLatestObject->setShowMarkers(ui->checkBoxShowMarkers->isChecked());
        mLatestObject->setShowBackgroundImage(ui->checkBoxShowBackGroundImage->isChecked());
        //axies

        mLatestObject->setAxiesEnabled(ui->checkBoxAxis->isChecked());//status
        mLatestObject->setAxisPosition(ui->comboBoxAxisPosition->currentText() == "Left" ? QObjectAxisPositionLeft : QObjectAxisPositionRight);
        QScadaObjectInfoAxis lAxis;
        lAxis.setX(lAxis.axisFromString(ui->comboBoxX->currentText()));
        lAxis.setY(lAxis.axisFromString(ui->comboBoxY->currentText()));
        lAxis.setZ(lAxis.axisFromString(ui->comboBoxZ->currentText()));
        mLatestObject->setAxis(lAxis);

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

    enableAxis(lEnabled);
}

void QScadaObjectInfoDialog::on_pushButton_3_clicked()
{
    mMarkerImage = QFileDialog::getOpenFileName(this,
         tr("Open Image"), "/home/vshevchyk", tr("Image Files (*.png *.jpg *.bmp *.jpeg)"));
}
