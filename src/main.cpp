#include "filedialoghelper.h"
#include <QApplication> // Use QApplication instead of QGuiApplication
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);  // Switch to QApplication
    QApplication::setApplicationName("ce216");  // Needed for settings usage in QML
    QApplication::setOrganizationName("ce216");  // Needed for settings usage in QML

    QQmlApplicationEngine engine;

    // Connect to handle errors
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app,
        [](QObject *obj, const QUrl &objUrl) {
            if (!obj) {
                QCoreApplication::exit(-1);
            }
        }, Qt::QueuedConnection);

    FileDialogHelper fileDialogHelper;

    // Expose FileDialogHelper to QML
    engine.rootContext()->setContextProperty("fileDialogHelper", &fileDialogHelper);

    // Load the QML file (make sure the QML file path is correct)
    engine.load(QUrl(QStringLiteral("qrc:/qt/qml/src/qml/mainWindow.qml")));

    return app.exec();  // Start the event loop
}
