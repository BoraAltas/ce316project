#include "iae.h"
#include <QGuiApplication>
#include <QCoreApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);
    QGuiApplication::setApplicationName(QStringLiteral("ce216"));  // needed for settings usage in qml
    QGuiApplication::setOrganizationName(QStringLiteral("ce216")); // needed for settings usage in qml
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
