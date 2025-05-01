#include "iae.h"
#include "zipHandler.h"
#include <QApplication>
#include <QGuiApplication>
#include <QCoreApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationName(QStringLiteral("ce216"));  // needed for settings usage in qml
    QApplication::setOrganizationName(QStringLiteral("ce216")); // needed for settings usage in qml

    QQmlApplicationEngine engine;

    IAE iae;
    const auto qmlUrl = QStringLiteral("iae");

    engine.rootContext()->setContextProperty(qmlUrl, &iae);

    ZipHandler zipHandler;
    engine.rootContext()->setContextProperty("zipHandler", &zipHandler);

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
