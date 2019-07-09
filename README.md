# QSimpleScada
Qt/C++ based simple SCADA framework, with dashboard, static and dynamic components. By using QSimpleScada framework you can build complex SCADA UIs for your needs.

# Minimum requirements
Qt 5.8

# Installing with qpm
Just use qpm (https://www.qpm.io/) to install QSimpleScada in your project. Run qpm install com.indeema.QSimpleScada . in *.pro file include(vendor/Vendor.pri)

latest release v0.5.2

Or Compile QSimpleScada with QSimpleScada pro file, you will receive QSimpleScadaLib folder with compiled windows or macos libs.

# Sample

You can check example that uses QSimpleScada https://github.com/IndeemaSoftware/QSimpleScadaSample

# Sample in action
<img src="https://github.com/IndeemaSoftware/QSimpleScada/blob/Assets/qsimplescada.gif" />

# How to start

Create editable QScadaBoard
```cpp
    QScadaBoardController *mController = new QScadaBoardController();
    mController->initBoardForDeviceIp("127.0.0.0");
    mController->setEditingMode(true);

    QScadaBoard *mBoard = mController->getBoardListForDeviceIp("127.0.0.0").at(0);
```

You can connect to signals, to handle events
```cpp
signals:
    void objectDoubleClicked(QScadaObject*);
    void objectSelected(QScadaObject *);
```

So now you can create some default QScadaObject on the board
```cpp
mBoard->createNewObject();
```
Or create object with specific parameters
```cpp
QScadaObjectInfo *lInfo = new QScadaObjectInfo();
lInfo->setId(2);
lInfo->setBackGroundImage(":/resources/some_structure.png");
lInfo->setShowBackgroundImage(true);
lInfo->setShowMarkers(false);
```

Creating object with specific parameters could be useful when restoring dashboard from project file.

Also you have possibility to send object to front or to back
```cpp
void MainWindow::bringToFront()
{
    if (!mBoard->getSeletedObjects().isEmpty()) {
        QScadaObject *lObject = mBoard->getSeletedObjects().first();
        mBoard->bringToFront(lObject);
    }
}

void MainWindow::sendToBack()
{
    if (!mBoard->getSeletedObjects().isEmpty()) {
        QScadaObject *lObject = mBoard->getSeletedObjects().first();
        mBoard->sendToBack(lObject);
    }
}
```

Also save and open project file
```cpp
void MainWindow::save()
{
    if (mBoard->objects()->count() == 0) {
        QString lMessage(tr("Nothing to be saved"));

        QMessageBox lMsgBox;
        lMsgBox.setText(lMessage);
        lMsgBox.exec();
        return;
    }

    QFileDialog lDialog(this);
    lDialog.setFileMode(QFileDialog::AnyFile);
    lDialog.setAcceptMode(QFileDialog::AcceptSave);
    lDialog.setDirectory(QDir::currentPath());
    lDialog.setWindowTitle(tr("Save Project"));
    lDialog.setNameFilter(tr("iReDS Project (*.irp)"));

    QScadaDeviceInfo lDeviceInfo;
    lDeviceInfo.setName("Test Device");
    lDeviceInfo.setIp(QHostAddress("127.0.0.0"));
    QList<QScadaDeviceInfo> lList;
    lList.append(lDeviceInfo);

    if (lDialog.exec() == QDialog::Accepted) {
        QStringList lFiles = lDialog.selectedFiles();
        if (lFiles.count() > 0) {
            QString lFileName = lFiles.at(0);
            if (!lFileName.contains(".irp")) {
                lFileName.append(".irp");
            }

            QString lDevices = QConnectedDeviceInfo::XMLFromDeviceInfo(lList, mController);   //<----;

            //create xml for boards of each device

            QFile lFile(lFileName);
            if (lFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream lOut(&lFile);
                lOut.setCodec("UTF-8");
                lOut << lDevices;
            } else {
                QString lMessage(tr("Something went wrong while trying to create file"));
                lMessage.append(" ").append(lFileName);

                QMessageBox lMsgBox;
                lMsgBox.setText(lMessage);
                lMsgBox.exec();
            }
            lFile.close();
        }
    }

    mBoard->setEditable(true);
}

void MainWindow::open()
{
    QString lFileName = QFileDialog::getOpenFileName(this,
        tr("Open Project"), QDir::currentPath(), tr("iReDS Project (*.irp)"));

    if (!lFileName.isEmpty()) {
        mObjectInfoDialog->updateWithObjectInfo(nullptr);

        for (QScadaObject *object : *mBoard->objects()) {
                mBoard->deleteObject(object);
        }

        QConnectedDeviceInfo* lConnectedDevceInfo = new QConnectedDeviceInfo();
        QByteArray lRawData;
        QFile lFile(lFileName);
        if (lFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream lStreamFileOut(&lFile);
            lStreamFileOut.setCodec("UTF-8");
            lRawData = lStreamFileOut.readAll().toUtf8();
            lFile.close();

            lConnectedDevceInfo->initFromXml(lRawData);

            for (int i = 0; i < lConnectedDevceInfo->connecteDeviceList.count(); ++i) {
                for (QScadaBoardInfo *boardInfo : lConnectedDevceInfo->connecteDeviceList.at(i)->boardList) {
                    if (boardInfo != nullptr) {
                        mBoard->initBoard(boardInfo);
                    }
                }
            }

            mBoard->update();
            mBoard->setEditable(true);
        } else {
            qDebug() << "       - Error open preferences file -> " << lFile.fileName();
        }

        delete lConnectedDevceInfo;
    }
}
```

## Communication and Support
If you encounter an issue or you have any comments or propositions with using the QSimpleScada library then you can reach us in several different ways:
- Having difficulties with using QSimpleScada you can write at [Stackoverflow](https://stackoverflow.com/) or at [Qt forum](https://forum.qt.io). Don't forget about specifing the **QSimpleScada** tag. You will be helped by the community of this resource or our specialists will help you with an answer.

- If you find a bug and want to tell us about it - specify it in the section [Issues](https://github.com/IndeemaSoftware/QSimpleScada/issues).
In this section, we only consider bugs and ignore any questions relating to the support.

- For additional assistance with your project - please contact us at **support@indeema.com** and specify **QSimpleScada** in the subject line.

- You can also follow our news at [@IndeemaSoftware](https://twitter.com/IndeemaSoftware) or on our [blog](https://indeema.com/blog).

- For further questions on cooperation, simply email us at **support@indeema.com**.

## License
**QSimpleScada** works under the MIT license. For more information see [here](https://github.com/IndeemaSoftware/QSimpleScada/blob/master/LICENSE).

## Terms
**QSimpleScada** is released for testing purposes only. We make no guarantees with respect to its function. By using this software you agree that Indeema is not liable for any damage to your system and data.

To know more about us and our [IoT expertise](https://indeema.com/services/iot), visit our website http://indeema.com

