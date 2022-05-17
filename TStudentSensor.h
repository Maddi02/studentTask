#ifndef TSTUDENTSENSOR_H
#define TSTUDENTSENSOR_H

#include <QObject>
#include <QDebug>
#include <qlogging.h>
#include <QList>

#include <QBluetoothDeviceDiscoveryAgent>
#include <QLowEnergyController>
#include <QPushButton>

// Service Container class, used to offload some stuff from TStudentSensor
class BleService : public QObject
{
  Q_OBJECT
public:
  BleService(QLowEnergyService* pService, QObject* parent = nullptr);
public slots:
  void onStateChanged(QLowEnergyService::ServiceState newState);
  void onCharacteristicChanged(const QLowEnergyCharacteristic &characteristic, const QByteArray &newValue);

signals:
  void dataChanged(float temperature, float humidity);

private:
  QLowEnergyService* m_pService;
};

class TStudentSensor : public QObject
{
  Q_OBJECT

  Q_PROPERTY(float temperature READ getTemperature WRITE setTemperature NOTIFY temperatureChanged)
  Q_PROPERTY(float humidity READ getHumidity WRITE setHumidity NOTIFY humidityChanged)
  Q_PROPERTY(bool searching READ getSearching NOTIFY searchingChanged)

public:
  TStudentSensor(QObject* parent = nullptr);
  ~TStudentSensor();

  float getTemperature() const;
  void setTemperature(float value);

  float getHumidity() const;
  void setHumidity(float value);

  bool getSearching();

public slots:
  void setTemperatureHumidity(float temperature, float humidity);
  void deviceFound(const QBluetoothDeviceInfo&);
  void deviceConnected();
  void deviceDisconnected();
  void serviceDiscovered(const QBluetoothUuid &serviceUuid);
  void serviceDetailsDiscovered();

signals:
  void temperatureChanged();
  void humidityChanged();
  void searchingChanged();

private:
  int m_temperature; // wenn man den Datentyp hier auf int umändert werden nur ganzzahlige Temperaturen ausgegeben
  // normal war m_temperature vom datentyp float.
  int m_humidity;
  bool m_searching;

  bool m_deviceConnected = false;
  QBluetoothDeviceDiscoveryAgent* m_pDeviceDiscoveryAgent;
  QLowEnergyController* m_pLowEnergyController;
  QList<BleService*> m_services;
};

#endif // TSTUDENTSENSOR_H
