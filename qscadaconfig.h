#ifndef CONFIG_H
#define CONFIG_H

#include <QList>
#include <QStringList>
#include <QDirIterator>
#include <QDebug>

#define DEFAULT_QML_PATH ":/com/indeema/eeiot/EEIoT/"

struct QMLWidgetsConfig {
    QString title = "";
    bool isValid = false;

    QMLWidgetsConfig(QString path){
        if (!path.isEmpty()) {
            QDir directory(path);

            // Load all files with the *.PNG extension
            // (you can modify this to suit your needs.
            QStringList lQMLList = directory.entryList(QStringList("*.qml"));
            for (QString file : lQMLList) {
                qDebug() << file;
            }

            if (lQMLList.count() > 0) {
                mQMLWidgets = lQMLList;
                QString lTmpTitle = path.split('/').last();
                //let secure from having / at end or not having
                if (lTmpTitle.isEmpty()) {
                    title = path.split('/').at(path.split('/').count()-2);
                } else {
                    title = lTmpTitle;
                }
            }

//            QDirIterator it("path", QDirIterator::Subdirectories);
//            while (it.hasNext()) {
//                qDebug() << it.next();
//            }
        }
    }

private:
    QStringList mQMLWidgets;
};

class QMLConfig
{
    private:
        QMLConfig() {
            appendQMLPath(DEFAULT_QML_PATH);
        }
        QMLConfig(const QMLConfig&) {}
        QMLConfig& instance(const QMLConfig&) {
            return *this;
        }

    public:
        static QMLConfig & instance() {
            static QMLConfig * _instance = nullptr;
            if (_instance == nullptr) {
                _instance = new QMLConfig();
            }
            return *_instance;
        }

        void appendQMLPath(QString path) {
            mQMLPaths.append(path);
            mQMLWidgets.append(QMLWidgetsConfig(path));
        }

private:
    QStringList mQMLPaths;
    QList<QMLWidgetsConfig> mQMLWidgets;
};

enum QScadaStatus {
    QScadaStatusDefault,
    QScadaStatusGray,
    QScadaStatusGreen,
    QScadaStatusYellow,
    QScadaStatusRed
};

#endif // CONFIG_H
