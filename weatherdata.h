#ifndef WEATHERDATA_H
#define WEATHERDATA_H

#include <QObject>
#include <QNetworkAccessManager> // um Anfragen zu Starten
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply> //um mit Antowrten umgehen zu können
#include <QMainWindow>
#include <QPixmap> // Das man auch mit Fotos arbeiten könnte
#include <QPushButton>

class Weatherdata : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QList<QObject*> list READ getDailyWeatherList NOTIFY dailyWeatherListChanged)
        Q_PROPERTY(QString city READ getCity NOTIFY cityChanged)


public:
    Weatherdata();

    Q_INVOKABLE void downloadWeatherData();

    Q_INVOKABLE void setPosition(double lon, double lat);

    QObjectList getDailyWeatherList();

    QString getCity();



signals:
    void dailyWeatherListChanged();
    void cityChanged();

private:
    void parseJson(QJsonDocument doc);

    QNetworkAccessManager nam;

    QObjectList dailyWeatherList;
    double latitude = 47.908748;
    double longitude = 8.158202;

    QString city = "foo";




};

#endif // WEATHERDATA_H
