#include "iae.h"
#include <QApplication>
#include <QGuiApplication>
#include <QCoreApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
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
    QApplication::setApplicationName(QStringLiteral("ce216"));  // needed for settings usage in qml
    QApplication::setOrganizationName(QStringLiteral("ce216")); // needed for settings usage in qml
    QQmlApplicationEngine engine;

    IAE iae;
    const auto qmlUrl = QStringLiteral("iae");

    engine.rootContext()->setContextProperty(qmlUrl, &iae);

    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
        []() {
            QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    const QUrl url = QUrl(QStringLiteral("qrc:/src/qml/mainWindow.qml"));
    engine.load(url);

    return QGuiApplication::exec();
}