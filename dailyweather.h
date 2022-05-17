#ifndef DAILYWEATHER_H
#define DAILYWEATHER_H


#include <QObject>

class DailyWeather : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString day READ getDay NOTIFY dayChanged)
    Q_PROPERTY(double high READ getHigh NOTIFY highChanged)
    Q_PROPERTY(double low READ getLow NOTIFY lowChanged)
    Q_PROPERTY(QString icon READ getIcon NOTIFY iconChanged)


public:
    explicit DailyWeather(QString day, int high, int low, QString icon, QObject *parent = nullptr);
    QString getDay();
    double getHigh();
    double getLow();
    QString getIcon();
signals:
    void dayChanged();
    void highChanged();
    void lowChanged();
    void iconChanged();

public slots:

private:

    QString day;
    double high;
    double low;
    QString icon;





};

#endif // DAILYWEATHER_H
