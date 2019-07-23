#ifndef VOBJECTINFODIALOG_H
#define VOBJECTINFODIALOG_H

#include <QWidget>
#include <QMultiMap>

namespace Ui {
class QScadaObjectInfoDialog;
}

class QScadaObjectInfo;

class QScadaObjectInfoDialog : public QWidget
{
    Q_OBJECT

public:
    explicit QScadaObjectInfoDialog(QWidget *parent = nullptr);
    ~QScadaObjectInfoDialog();

private:
    void showObjectProperties(QMultiMap<QString, QVariant>);

public slots:
    void updateWithObjectInfo(QScadaObjectInfo *info);

private slots:
    void geometryUpdated(QScadaObjectInfo*);
    void on_pushButton_2_pressed();

    void on_pushButton_pressed();

    void on_pushButton_3_clicked();

signals:
    void deletePressed(QScadaObjectInfo*);
    void savePressed(QScadaObjectInfo*);

private:
    Ui::QScadaObjectInfoDialog *ui;

    QString mMarkerImage;

    QScadaObjectInfo *mLatestObject;
    QMultiMap<QString, QVariant> mProperties;
};

#endif // VOBJECTINFODIALOG_H
