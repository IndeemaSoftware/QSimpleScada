#ifndef CONFIG_H
#define CONFIG_H

#define MIN_PAUSE_TIME 5

enum QScadaStatus {
    QScadaStatusDefault,
    QScadaStatusGray,
    QScadaStatusGreen,
    QScadaStatusYellow,
    QScadaStatusRed
};

enum QScadaUnitStatus{
    QScadaUnitStatusNotConfigured = -1,
    QScadaUnitStatusConfigured,
    QScadaUnitStatusNotConnected
} ;

typedef enum {
    QScadaMachineryClass1 = 1,
    QScadaMachineryClass2,
    QScadaMachineryClass3,
    QScadaMachineryClass4
} QScadaMachineryClass;

enum VUpdateMode {
    VUpdateModeFromTo,
    VUpdateMode24Hours,
    VUpdateModeDaysCount
};

enum QScadaTrendPaintMode {
    QScadaTrendPaintModeDefault,
    QScadaTrendPaintModeHours,
    QScadaTrendPaintModeDays
};

inline double GetRealRecordingPeriod(QList<int>& recordingDurations, double&& pauseTime = 0, int&& totalRecordingTime = 0)
{
    totalRecordingTime = 0;
    pauseTime = 0;

    for(const int& value : recordingDurations) {
        totalRecordingTime += value;
    }

    pauseTime = totalRecordingTime * 0.5;

    if(pauseTime < (double)MIN_PAUSE_TIME) {
        pauseTime = (double)MIN_PAUSE_TIME;
    }

    double rRecordingPeriod = (double)totalRecordingTime + pauseTime;

    return rRecordingPeriod;
}

#endif // CONFIG_H
