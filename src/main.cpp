#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    QGuiApplication::setApplicationName("ce216"); //needed for settings usage in qml
    QGuiApplication::setOrganizationName("ce216"); //needed for settings usage in qml
    QQmlApplicationEngine engine;

    // Connect to handle errors
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app,
        [](QObject *obj, const QUrl &objUrl) {
             if (!obj) {
                 QCoreApplication::exit(-1);
             }
        }, Qt::QueuedConnection);

    engine.load(QUrl(QStringLiteral("qrc:/qt/qml/src/qml/mainWindow.qml")));

    return app.exec();
}
