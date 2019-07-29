#-------------------------------------------------
#
# Project created by QtCreator 2017-07-01T17:39:42
#
#-------------------------------------------------

QT += widgets network qml quick quickwidgets

TARGET = QSimpleScada
TEMPLATE = lib
VERSION = 0.9.0

include(com_indeema_QSimpleScada.pri)

win32 {
    DLLDESTDIR +=../QSimpleScadaLib/
}

mac {
    DESTDIR +=../QSimpleScadaLib/
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}
