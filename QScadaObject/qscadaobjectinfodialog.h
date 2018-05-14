#ifndef VOBJECTINFODIALOG_H
#define VOBJECTINFODIALOG_H

#include <QWidget>

namespace Ui {
class QScadaObjectInfoDialog;
}

class QScadaObjectInfo;

class QScadaObjectInfoDialog : public QWidget
{
    Q_OBJECT

public:
    explicit QScadaObjectInfoDialog(QWidget *parent = 0);
    ~QScadaObjectInfoDialog();

public slots:
    void updateWithObjectInfo(QScadaObjectInfo *info);

private:
    void enableAxis(bool);
    void initAxiesList();

private slots:
    void geometryUpdated(QScadaObjectInfo*);
    void on_pushButton_2_pressed();

    void on_pushButton_pressed();

    void on_checkBoxAxis_stateChanged(int arg1);

    void on_pushButton_3_clicked();

signals:
    void deletePressed(QScadaObjectInfo*);
    void savePressed(QScadaObjectInfo*);

private:
    Ui::QScadaObjectInfoDialog *ui;

    QString mMarkerImage;

    QScadaObjectInfo *mLatestObject;
};

#endif // VOBJECTINFODIALOG_H
