#include <FelgoApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QLoggingCategory>

#include "TStudentSensor.h"
#include "weatherdata.h"
#include <QPushButton>
#include <QTimer>

int main(int argc, char *argv[])
{

  QApplication app(argc, argv);

  FelgoApplication felgo;
  felgo.setPreservePlatformFonts(true);

  // QQmlApplicationEngine is the preferred way to start qml projects since Qt 5.2
  // if you have older projects using Qt App wizards from previous QtCreator versions than 3.1, please change them to QQmlApplicationEngine
  QQmlApplicationEngine engine;

  TStudentSensor sensor;
  Weatherdata weatherdata;
  weatherdata.downloadWeatherData();

  engine.rootContext()->setContextProperty("testoSensor", &sensor);
  engine.rootContext()->setContextProperty("weatherdata", &weatherdata);

  felgo.initialize(&engine);

  // use this instead of the below call for faster deployment on desktop when you comment the RESOURCES in the .pro file (this avoids compiling all your resources into the binary and has shorter compile times; also it allows re-running the same project in non-shadow builds)
  // this is the preferred deployment option during development on Desktop (for deployment on mobile use the qrc approach below)
  felgo.setMainQmlFileName(QStringLiteral("qml/WeatherMain.qml"));

  // use this instead of the above call to avoid deployment of the qml files and compile them into the binary with qt's resource system qrc
  // this is the preferred deployment option for debug and release builds on mobile, and for publish builds on all platforms
  // to avoid deployment of your qml files and images, also comment the DEPLOYMENTFOLDERS command in the .pro file
  // only use the above non-qrc approach, during development on desktop
//  felgo.setMainQmlFileName(QStringLiteral("qrc:/qml/WeatherMain.qml"));

  engine.load(QUrl(felgo.mainQmlFileName()));

  return app.exec();
}
