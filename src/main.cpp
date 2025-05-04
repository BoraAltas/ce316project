#include "iae.h"
#include "zipHandler.h"
#include <QApplication>
#include <QGuiApplication>
#include <QCoreApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <QIcon>
#include <iostream>
#include <sqlite3.h>

int main(int argc, char* argv[]) {
     sqlite3* db;
        int exit = 0;

        // Open Database (creates the file if it doesn't exist)
        exit = sqlite3_open("example.db", &db);

        if (exit) {
            std::cerr << "Error open DB " << sqlite3_errmsg(db) << std::endl;
            return -1;
        } else {
            std::cout << "Opened Database Successfully!" << std::endl;
        }

        sqlite3_close(db);
     QApplication app(argc, argv);
    QApplication::setApplicationName("ce216");
    QApplication::setOrganizationName("ce216");

        qmlRegisterSingletonType<IAE>("iae", 1, 0, "IAE", [](QQmlEngine* engine, QJSEngine*) -> QObject* {

            return new IAE();
        });

          QQmlApplicationEngine engine;

          ZipHandler zipHandler;
          engine.rootContext()->setContextProperty("zipHandler", &zipHandler);

          QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed, &app,
                           []() { QCoreApplication::exit(-1); },
                           Qt::QueuedConnection);

          engine.load(QUrl(QStringLiteral("qrc:/src/qml/MainWindow.qml")));

          return app.exec();
      }
