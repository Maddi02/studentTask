#include "dailyweather.h"
#include "weatherdata.h"
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <chrono>
#include <thread>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QPushButton>
#include <QTimer>

Weatherdata::Weatherdata() : QObject()
{

}

void Weatherdata::downloadWeatherData()
{
    dailyWeatherList.clear();
    QNetworkRequest request(QUrl("https://api.openweathermap.org/data/2.5/forecast?lat=" + QString::number(latitude) + "&lon=" + QString::number(longitude) + "&APPID=560031faef4f72dee4d154f888eafe0d"));

    qDebug() << "sending network request";

    connect(&nam, &QNetworkAccessManager::sslErrors, [&](QNetworkReply *reply, const QList<QSslError> &errors) {
        Q_UNUSED(reply);
        for(QSslError error : errors) {
            qDebug() << error.errorString();
        }
    });

    connect(&nam, &QNetworkAccessManager::finished, [&](QNetworkReply* reply){
        QByteArray response_data = reply->readAll();
        QJsonDocument json = QJsonDocument::fromJson(response_data);
        parseJson(json);
        reply->deleteLater();
    });

    QNetworkReply *reply = nam.get(request);
    Q_UNUSED(reply);
}

void Weatherdata::setPosition(double lon, double lat)
{
    longitude = lon;
    latitude = lat;
}

QObjectList Weatherdata::getDailyWeatherList()
{
    return dailyWeatherList;
}

QString Weatherdata::getCity()
{
    return city;
}

void Weatherdata::parseJson(QJsonDocument doc)
{
    QJsonObject root = doc.object();
    QJsonArray list = root.value("list").toArray();
    QJsonObject cityObj = root.value("city").toObject();
    QString c = cityObj.value("name").toString();
    if(c != "") {
        city = c;
        emit cityChanged();
    }

    QString day(""); // Wenn man die hintereinader schreiben würde, kann man schnell den Überblick verlieren
    int loops = 0;
    double totalTemp = 0;
    double minTemp = 0;
    double maxTemp = 0;
    int id = 0;
    int clearWeatherLoops = 0;
    int snowWeatherLoops = 0;
    int sunnyWeatherLoops = 0;
    int stormWeatherLoops = 0;
    int rainyWeatherLoops = 0;
    int extremWeatherLoops = 0;
    int  onlySunnyWeatherLoops= 0;
    QString symbol; // Alle QString typen untereinadere, damit man alle auf einmal findet

    for(auto value : list) {
        QJsonObject obj = value.toObject();
        QDateTime time = QDateTime::fromSecsSinceEpoch(obj.value("dt").toInt());
        QString currentDay = time.toString("dddd");
        if(day == "") {
            day = currentDay;
        }

        QJsonObject main = obj.value("main").toObject();


        QJsonArray weather = obj.value("weather").toArray();
        if(day != currentDay) {

            qDebug() << sunnyWeatherLoops << clearWeatherLoops <<  snowWeatherLoops << symbol<< endl;


            maxTemp = maxTemp - 273.15;
            minTemp = minTemp - 273.15;

            // Die if Anweiseung in eine serpate Klasse

            if(stormWeatherLoops > sunnyWeatherLoops && stormWeatherLoops > snowWeatherLoops && stormWeatherLoops > rainyWeatherLoops && stormWeatherLoops > extremWeatherLoops && stormWeatherLoops > onlySunnyWeatherLoops) // schauen ob es Sturm gibt
            {
                symbol = "";
            }
            else if (sunnyWeatherLoops > rainyWeatherLoops && sunnyWeatherLoops > snowWeatherLoops && sunnyWeatherLoops > stormWeatherLoops && sunnyWeatherLoops > extremWeatherLoops && sunnyWeatherLoops > onlySunnyWeatherLoops) // schauen ob es Wolken gibt
            {

                symbol = "";
            }
            else if (snowWeatherLoops > sunnyWeatherLoops && snowWeatherLoops > stormWeatherLoops && snowWeatherLoops > rainyWeatherLoops && snowWeatherLoops > extremWeatherLoops && snowWeatherLoops > onlySunnyWeatherLoops ) // schauen ob es Schnee gibt
            {

                symbol = "";
            }

            else if (rainyWeatherLoops > sunnyWeatherLoops && rainyWeatherLoops > snowWeatherLoops && rainyWeatherLoops > stormWeatherLoops && rainyWeatherLoops > extremWeatherLoops && rainyWeatherLoops > onlySunnyWeatherLoops ) // schauen ob es Regen gibt
            {
                symbol = "";
            }

            else if (extremWeatherLoops > rainyWeatherLoops && extremWeatherLoops > stormWeatherLoops && extremWeatherLoops > sunnyWeatherLoops && extremWeatherLoops > snowWeatherLoops && extremWeatherLoops > onlySunnyWeatherLoops ) // schauen ob es Extreme gibt
            {
                symbol = "";

            }

            else if(onlySunnyWeatherLoops > sunnyWeatherLoops && onlySunnyWeatherLoops > snowWeatherLoops && onlySunnyWeatherLoops> rainyWeatherLoops && onlySunnyWeatherLoops > extremWeatherLoops && onlySunnyWeatherLoops > stormWeatherLoops ) // schauen ob es Sonne gibt
            {
                symbol = "";
            }


            qDebug() << onlySunnyWeatherLoops << sunnyWeatherLoops << snowWeatherLoops << rainyWeatherLoops << extremWeatherLoops << stormWeatherLoops;

            DailyWeather* dw = new DailyWeather(day, maxTemp, minTemp, symbol, this);
            // int average = totalTemp / loops;
            dailyWeatherList.append(dw);
            emit dailyWeatherListChanged();
            day = currentDay;
            loops = 0;
            totalTemp = 0;
            minTemp = 0;
            maxTemp = 0;

            snowWeatherLoops = 0;
            sunnyWeatherLoops = 0;
            stormWeatherLoops = 0;
            rainyWeatherLoops = 0;
            extremWeatherLoops = 0;
            onlySunnyWeatherLoops = 0;

        }

        if(day == currentDay) {
            for(auto weatherObject: weather)
            {

                id = weatherObject.toObject().value("id").toInt();
                if (id < 259)
                {
                    stormWeatherLoops++;
                }

                else if (id > 299 && id < 550)
                {
                    rainyWeatherLoops++;
                }

                else if (id > 560 && id < 630)
                {
                    snowWeatherLoops++;
                }

                else if (id > 640 && id < 802)
                {
                    onlySunnyWeatherLoops++;
                }
                else if (id > 802 && id < 805)
                {
                    sunnyWeatherLoops++;
                }

                else if (id > 880 && id < 910)
                {
                    extremWeatherLoops++;
                }

            }

            loops++;
            totalTemp += main.value("temp").toDouble();
            double currentMinTemp = main.value("temp_min").toDouble();
            double currentMaxTemp = main.value("temp_max").toDouble();

            if(minTemp == 0) {
                minTemp = currentMinTemp;
            }

            if(maxTemp == 0) {
                maxTemp = currentMaxTemp;
            }

            if(minTemp > currentMinTemp) {
                minTemp = currentMinTemp;
            }

            if(maxTemp < currentMaxTemp) {
                maxTemp = currentMaxTemp;
            }
        }





    }
    qDebug() << sunnyWeatherLoops << clearWeatherLoops <<  snowWeatherLoops << symbol<< endl;
    for(auto value : dailyWeatherList) {
        DailyWeather* w = dynamic_cast<DailyWeather*>(value);
        qDebug() << "Value: " << w->getDay() << w->getLow() << w->getHigh() << w->getIcon();
    }
}


