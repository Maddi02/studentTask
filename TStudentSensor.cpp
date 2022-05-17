#include "TStudentSensor.h"

#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QLoggingCategory>
#include <limits>
#include <QTimer>

Q_LOGGING_CATEGORY(studentSensor, "tstudentsensor")

BleService::BleService(QLowEnergyService *pService, QObject *parent)
  : QObject(parent)
  , m_pService(pService)
{
  // connect to the signals
  connect(m_pService, &QLowEnergyService::stateChanged, this, &BleService::onStateChanged);
  connect(m_pService, &QLowEnergyService::characteristicChanged,
          this, &BleService::onCharacteristicChanged);

  // discover details if not yet discovered
  if(pService->state() == QLowEnergyService::DiscoveryRequired)
    pService->discoverDetails();
}

void BleService::onStateChanged(QLowEnergyService::ServiceState newState)
{
  qCDebug(studentSensor) << m_pService->serviceName() << m_pService->type() << m_pService->state();
  const QList<QLowEnergyCharacteristic> chars = m_pService->characteristics();
  foreach (const QLowEnergyCharacteristic &ch, chars) {
    qCDebug(studentSensor) << "Characteristic" << ch.name() << ch.uuid();
    foreach(const QLowEnergyDescriptor &descr, ch.descriptors())
    {
      qCDebug(studentSensor) << "Descriptor: " << descr.uuid() << descr.name() << descr.type();
    }
    QLowEnergyDescriptor notification = ch.descriptor(
          QBluetoothUuid::ClientCharacteristicConfiguration);
    if (!notification.isValid())
      return;

    // establish hook into notifications
    connect(m_pService, &QLowEnergyService::characteristicChanged, this, &BleService::onCharacteristicChanged);

    // enable notification
    m_pService->writeDescriptor(notification, QByteArray::fromHex("0100"));
  }
}

void BleService::onCharacteristicChanged(const QLowEnergyCharacteristic &characteristic, const QByteArray &newValue)
{
  QString newValueStr(newValue);
  QRegularExpression re("(?<temperature>[0-9]+[.][0-9]+),(?<humidity>[0-9]+[.][0-9]+)");
  QRegularExpressionMatch match = re.match(newValueStr);
  if(match.hasMatch())
  {
    emit dataChanged(match.captured("temperature").toFloat(), match.captured("humidity").toFloat());
  }

  qCDebug(studentSensor) << m_pService->serviceName() << m_pService->serviceUuid() << characteristic.name() << newValue;
  const QList<QLowEnergyCharacteristic> chars = m_pService->characteristics();

  foreach (const QLowEnergyCharacteristic &ch, chars) {
    qCDebug(studentSensor) << "Characteristic" << ch.name() << ch.uuid();
  }
}

TStudentSensor::TStudentSensor(QObject* parent)
  : QObject(parent)
  , m_temperature(std::numeric_limits<float>::quiet_NaN())
  , m_humidity(std::numeric_limits<float>::quiet_NaN())
  , m_searching(true)
  , m_pDeviceDiscoveryAgent(new QBluetoothDeviceDiscoveryAgent())
{
  connect(m_pDeviceDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &TStudentSensor::deviceFound);
  m_pDeviceDiscoveryAgent->start();
}

TStudentSensor::~TStudentSensor()
{
  delete m_pDeviceDiscoveryAgent;
  if(m_pLowEnergyController)
    delete m_pLowEnergyController;
}

float TStudentSensor::getTemperature() const
{
  return m_temperature;
}

void TStudentSensor::setTemperature(float value)
{
  if(value != m_temperature)
  {
    m_temperature = value;
    emit temperatureChanged();
  }
}

float TStudentSensor::getHumidity() const
{
  return m_humidity;
}

void TStudentSensor::setHumidity(float value)
{
  if(value != m_humidity)
  {
    m_humidity = value;
    emit humidityChanged();
  }
}

bool TStudentSensor::getSearching()
{
  return m_searching;
}

void TStudentSensor::setTemperatureHumidity(float temperature, float humidity)
{
  setTemperature(temperature);
  setHumidity(humidity);
  if(m_searching)
  {
    m_searching = false;
    emit searchingChanged();
  }
}

void TStudentSensor::deviceFound(const QBluetoothDeviceInfo &info)
{
  // Filter for BLE & testo student sensor
  if((info.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration) && info.name() == "Testo Student Sensor")
  {
    qCDebug(studentSensor) << "Found BLE device: " << info.name() << info.address() << info.rssi();

    // connect to the BLE device
    m_pLowEnergyController = new QLowEnergyController(info);

    // stop looking for devices
    m_pDeviceDiscoveryAgent->stop();

    connect(m_pLowEnergyController, &QLowEnergyController::serviceDiscovered, this, &TStudentSensor::serviceDiscovered);

    connect(m_pLowEnergyController, QOverload<QLowEnergyController::Error>::of(&QLowEnergyController::error),
            [=](QLowEnergyController::Error newError){
      qCDebug(studentSensor) << newError << "connection error code";
    });

    connect(m_pLowEnergyController, &QLowEnergyController::connected, this, &TStudentSensor::deviceConnected);
    connect(m_pLowEnergyController, &QLowEnergyController::disconnected, this, &TStudentSensor::deviceDisconnected);

    m_pLowEnergyController->connectToDevice();
  }
}

void TStudentSensor::deviceConnected()
{
  qCDebug(studentSensor) << "Controller connected. Searching services...";
  m_pLowEnergyController->discoverServices();
}

void TStudentSensor::deviceDisconnected()
{
  qCDebug(studentSensor) << "LowEnergy controller disconnected";
}

void TStudentSensor::serviceDiscovered(const QBluetoothUuid &serviceUuid)
{
  QLowEnergyService* pService = m_pLowEnergyController->createServiceObject(serviceUuid);
  BleService* pServiceContainer = new BleService(pService);
  connect(pServiceContainer, &BleService::dataChanged, this, &TStudentSensor::setTemperatureHumidity);
  m_services.append(pServiceContainer);

  qCDebug(studentSensor) << "Service found" << serviceUuid;
  qCDebug(studentSensor) << pService->serviceName() << pService->state() << pService->type();
  const QList<QLowEnergyCharacteristic> chars = pService->characteristics();
  foreach (const QLowEnergyCharacteristic &ch, chars) {
    qCDebug(studentSensor) << "Characteristic" << ch.name() << ch.uuid();
  }
}

void TStudentSensor::serviceDetailsDiscovered()
{

}
