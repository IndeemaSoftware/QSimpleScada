#-------------------------------------------------
#
# Project created by QtCreator 2017-07-01T17:39:42
#
#-------------------------------------------------

QT += widgets network

TARGET = QSimpleScada
TEMPLATE = lib
VERSION = 0.5.3

include(com_indeema_QSimpleScada.pri)

win32 {
    DLLDESTDIR +=../QSimpleScadaLib/lib
}

mac {
    DESTDIR +=../QSimpleScadaLib/lib
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}
