QT += bluetooth
#include <QPushButton>
QT += core



# allows to add DEPLOYMENTFOLDERS and links to the Felgo library and QtCreator auto-completion
CONFIG += felgo

qmlFolder.source = qml
DEPLOYMENTFOLDERS += qmlFolder # comment for publishing

assetsFolder.source = assets
# DEPLOYMENTFOLDERS += assetsFolder # uncomment if you add any assets to the project

# Add more folders to ship with the application here

RESOURCES += \
#    resources_weather.qrc # uncomment for publishing

# NOTE: for PUBLISHING, perform the following steps:
# 1. comment the DEPLOYMENTFOLDERS += qmlFolder line above, to avoid shipping your qml files with the application (instead they get compiled to the app binary)
# 2. uncomment the resources.qrc file inclusion and add any qml subfolders to the .qrc file; this compiles your qml files and js files to the app binary and protects your source code
# 3. change the setMainQmlFile() call in main.cpp to the one starting with "qrc:/" - this loads the qml files from the resources
# for more details see the "Deployment Guides" in the Felgo Documentation

# during development, use the qmlFolder deployment because you then get shorter compilation times (the qml files do not need to be compiled to the binary but are just copied)
# also, for quickest deployment on Desktop disable the "Shadow Build" option in Projects/Builds - you can then select "Run Without Deployment" from the Build menu in Qt Creator if you only changed QML files; this speeds up application start, because your app is not copied & re-compiled but just re-interpreted


# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
           TStudentSensor.cpp \
    weatherdata.cpp \
    dailyweather.cpp

HEADERS += TStudentSensor.h \
    weatherdata.h \
    dailyweather.h

android {
  ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
  OTHER_FILES += android/AndroidManifest.xml
}

ios {
  QMAKE_INFO_PLIST = ios/Project-Info.plist
  OTHER_FILES += $$QMAKE_INFO_PLIST
}


DISTFILES +=
