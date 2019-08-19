  <img src="https://i.imgur.com/VDz3b4Y.png" />

# QSimpleScada
Qt/C++ based simple SCADA library for your IoT projects. We created QSimpleScada to speed up and simplify visualising any data, so we (and you) can concentrate on developing automation algorithms that rock. It completely handles connection to and editing of widgets. Using QSimpleScada, you can easily create a visualization of IoT data with static and dynamic components. After you are satisfied with the layout, save the generated .xml file and use it in your project.

Library is created with Qt/C++ and basic widget is based on C++. There is interface for QML, so you can independently create individual widgets on QML (as when creating classic QML UIs) and upload them to your app on a go. 

<p>
    <a href="https://github.com/IndeemaSoftware/QSimpleScada/blob/master/LICENSE.md">
    <img src="https://img.shields.io/badge/License-MIT-yellow.svg" /> </a>
   <img src="https://img.shields.io/badge/keywords-qpm%2C%20Qt%2C%20C%2B%2B%2C%20library%2C%20SCADA-green" />
   <img src="https://img.shields.io/badge/qt-%3E%3D5.8-blue" />
</p>

## Installing 
### Minimum requirements
Qt 5.8
### Using qpm:
To install via qpm, run:
```
qpm install com.indeema.qsimplescada
```
And add:
```
 include (../vendor/vendor.pri)
 ```
To *.pro file of your project. 

As a bonus, try out our preset of widgets:
```
qpm install com.indeema.eeiot
```
### From GitHub:
To clone the repo, go to:
https://github.com/IndeemaSoftware/QSimpleScada
To also add preconfigured widgets, clone:
https://github.com/IndeemaSoftware/EEIoT

And add:
```include($$PWD/com/indeema/QSimpleScada/com_indeema_QSimpleScada.pri)
include($$PWD/com/indeema/eeiot/com_indeema_eeiot.pri)
```
To the* *.pro* file. You'll receive QSimpleScadaLib folder with compiled Windows or MacOS libs.

### Using binary release:
https://github.com/IndeemaSoftware/QSimpleScada/releases

## Structure

One QScadaController can keep many devices with unique IP addresses. IP address is a unique id for each device.
Each device can have several dashboards with unique ids. On each board, you can set up many widgets. You can save the architecture to a* *.irp* file.

## Sample of usage

You can check examples of QSimpleScada use at https://github.com/IndeemaSoftware/QSimpleScadaSample
#### How the dynamic components look in the sample
<img src="https://github.com/IndeemaSoftware/QSimpleScada/blob/Assets/qsimplescada.gif" width="430" /> <img src="https://github.com/IndeemaSoftware/QSimpleScada/blob/Assets/QSimpleScada%20builder.gif" width="400" />


# Using QSimpleScada

1. Create your device:

```cpp
   QScadaDeviceInfo *lDeviceInfo = new QScadaDeviceInfo();
   lDeviceInfo->setName("Test Device");
   lDeviceInfo->setIp(QHostAddress("127.0.0.1"));
```

2.  Init your board controller. Your boardcontroller object is the main contact spot.
```cpp
   QScadaBoardController *mController = new QScadaBoardController();   
   mController->appendDevice(lDeviceInfo);
```
3. Init your board. Board ids are iterators. So if you will create one more board for this device, its id will be 1.
```cpp
   mController->initBoardForDeviceIp("127.0.0.1");
```

* To handle events, you can connect to signals:
```cpp
signals:
    void objectDoubleClicked(QScadaObject*);
```
* You can get pointers to specific board by calling methods:
```cpp
    QList<QScadaBoard*> getBoardList();
    QList<QScadaBoard*> getBoardListForDeviceIp(QString);
```

* And you can create new object by calling method of QScadaBoard object:
```cpp
    QScadaObject *initNewObject(QScadaObjectInfo *);
    void createNewObject(QScadaObjectInfo *);

    void createQMLObject(int id, QString path);
    void createQMLObject(QString path);
```

4. Define the editable or static type for your controller:
```cpp
    mController->setEditingMode(true);
```

5. Include your controller widget to you central widget:
```cpp
   QGridLayout *mainLayout = new QGridLayout(ui->centralWidget);
   mainLayout->addWidget(mController);
```

Now your board controller is initialized. Next steps are setting up the widget resources.
We’ve also developed a EEIoT library with a set of preconfigured widgets. You can download it at https://github.com/IndeemaSoftware/EEIoT and try it out as a start. Read wiki page to know the rules on [how to create qml widgets that can be used by QSimpleScada](https://github.com/IndeemaSoftware/QSimpleScada/wiki/How-to-create-QML-Widgets)

**To use a widget collection**:
Call the function with QML resources url to let the controller know the location of QML widgets:
```cpp
    QMLConfig::instance.appendQMLPath(:/com/indeema/eeiot/EEIoT/);
```
Path ```:/com/indeema/eeiot/EEIoT/``` is added by default, so you don't need to add it manually. If you call ```appendQMLPath``` with different path to EEIoT, it will replace the default path. Also you can add your own custom widgets.

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

## Support
* If you have suggestions, feedback, or encounter any issues, write to [Stackoverflow](https://stackoverflow.com/), [Qt forum](https://forum.qt.io) with **QSimpleScada** tag, or contact us at support@indeema.com. 
* If you find a bug, create an [issue](https://github.com/IndeemaSoftware/QSimpleScada/issues). 
* To learn more about our [IoT expertise](https://indeema.com/services/iot), visit https://indeema.com, follow our news at [@IndeemaSoftware](https://twitter.com/IndeemaSoftware) or subscribe to our [blog](https://indeema.com/blog).

## License
[MIT license](https://github.com/IndeemaSoftware/QSimpleScada/blob/master/LICENSE)
Copyright 2019 © [Indeema Software](https://indeema.com).
#### Developed by Volodymyr Shevchyk


