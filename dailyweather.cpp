#include "dailyweather.h"


DailyWeather::DailyWeather(QString day, int high, int low, QString icon, QObject *parent)
    : QObject(parent)
    , day(day)
    , high(high)
    , low(low)
    , icon(icon)
{
}

QString DailyWeather::getDay()
{
    return day;
}

double DailyWeather::getHigh()
{
    return high;
}

double DailyWeather::getLow()
{
    return low;
}

QString DailyWeather::getIcon()
{
    return icon;
}

