# QSimpleScada
Qt/C++ based simple SCADA framework, with dashboard, static and dynamic components. By using QSimpleScada framework you can build complex SCADA uis for your needs.

# Minimum requirements
Qt 5.8

# Installing with qpm
Just use qpm (https://www.qpm.io/) to install QSimpleScada in your project. Run qpm install com.indeema.QSimpleScada . in *.pro file include(vendor/Vendor.pri)

Or Compile QSimpleScada with QSimpleScada pro file, you will receive QSimpleScadaLib folder with compiled windows or macos libs.

# Sample

You can check examples that uses QSimpleScada https://github.com/IndeemaSoftware/QSimpleScadaSample

# Sample in action
<img src="https://github.com/IndeemaSoftware/QSimpleScada/blob/Assets/qsimplescada.gif" />
<img src="https://github.com/IndeemaSoftware/QSimpleScada/blob/Assets/QSimpleScada%20builder.gif" />
# How to start

First create your device

```cpp
   QScadaDeviceInfo *lDeviceInfo = new QScadaDeviceInfo();
   lDeviceInfo->setName("Test Device");
   lDeviceInfo->setIp(QHostAddress("127.0.0.1"));
```

Then init your board controller. Your boardcontroller object is main contact spot.
```cpp
   QScadaBoardController *mController = new QScadaBoardController();   
   mController->appendDevice(lDeviceInfo);
```
Now init your board
```cpp
   mController->initBoardForDeviceIp("127.0.0.1");
```

You can connect to signals, to handle events
```cpp
signals:
    void objectDoubleClicked(QScadaObject*);
```
You can get pointers to specific board by calling methods:
```cpp
    QList<QScadaBoard*> getBoardList();
    QList<QScadaBoard*> getBoardListForDeviceIp(QString);
```
Next just make your controller editable or static. That depends on your needs.
```cpp
    mController->setEditingMode(true);
```

Now just puth controllwe widget to you central widget
```cpp
   QGridLayout *mainLayout = new QGridLayout(ui->centralWidget);
   mainLayout->addWidget(mController);
```

Now your board controller is initialized. Next steps are setting ups widget resources.
We’ve also developed a EEIoT library with a set of preconfigured widgets. You can download it at https://github.com/IndeemaSoftware/EEIoT and try it out as a start. Read wiki page to know rules on [how to create qml widgets that can be used by QSimpleScada](https://github.com/IndeemaSoftware/QSimpleScada/wiki/How-to-create-QML-Widgets)

To use a widget collection:
Call the function with url to QML resources to let the controller know the location of QML widgets:
```cpp
    QMLConfig::instance.appendQMLPath(:/com/indeema/eeiot/EEIoT/);
```
Path :/com/indeema/eeiot/EEIoT/ is added by default so you don't need to add it manually. If you call appendQMLPath with different path to EEIoT it will replace default path. Also you can add your custom widgets.

You can use our simple editor to create your first dashboard https://github.com/IndeemaSoftware/QSimpleScadaSample 

Then set up QScadaBoardController in your app without any devices and boards and call:
```cpp
    mController->openProject(QString <file>)
```

where <file> is a full path to your project file (*.irp)

For example:
```cpp
    mController->openProject(QString <file>)
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


