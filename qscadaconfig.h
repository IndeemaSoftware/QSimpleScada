#ifndef CONFIG_H
#define CONFIG_H

#include <QList>
#include <QStringList>
#include <QDirIterator>
#include <QDebug>

#define DEFAULT_QML_PATH ":/com/indeema/eeiot/EEIoT/"

struct QMLInfo {
    QString groupTitle = "";
    QString groupPath = "";
};

struct QMLWidgetsConfig {
    QMLInfo info;
    bool isValid = false;

    QStringList widgets() {
        return mQMLWidgets;
    }

    QMLWidgetsConfig(){}
    QMLWidgetsConfig(QString path){
        if (!path.isEmpty()) {
            QDir directory(path);

            // Load all files with the *.PNG extension
            // (you can modify this to suit your needs.
            QStringList lQMLList = directory.entryList(QStringList("*.qml"));

            if (lQMLList.count() > 0) {
                mQMLWidgets = lQMLList;
                info.groupPath = path;

                QString lTmpTitle = path.split('/').last();
                //let secure from having / at end or not having
                if (lTmpTitle.isEmpty()) {
                    info.groupTitle = path.split('/').at(path.split('/').count()-2);
                } else {
                    info.groupTitle = lTmpTitle;
                }
            }
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
            QMLWidgetsConfig lConf = QMLWidgetsConfig(path);

            if (lConf.isValid) {
                //check if QML group is unique.
                //if it's already in list, it will be replaced with new
                for (int i=0; i < mQMLWidgets.count(); i++) {
                    QMLWidgetsConfig tmpConf = mQMLWidgets.at(i);
                    if (tmpConf.info.groupTitle == lConf.info.groupTitle) {
                        mQMLWidgets.removeAt(i);
                    }
                }
                mQMLWidgets.append(lConf);
            }
        }

        QMLWidgetsConfig qmlWidgetConfForGroup(QString qmlGroupTitle) {
            QMLWidgetsConfig rConf;

            for (QMLWidgetsConfig tmpConf : mQMLWidgets) {
                if (qmlGroupTitle == tmpConf.info.groupTitle) {
                    rConf = tmpConf;
                }
            }

            return rConf;
        }

        QList<QMLWidgetsConfig> QMLWidgets(){
            return mQMLWidgets;
        }
private:
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
