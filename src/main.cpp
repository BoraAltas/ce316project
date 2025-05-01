#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "filedialoghelper.h"
#include "iae.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);  // QWidget desteği sağlar
    QApplication::setApplicationName("ce216");
    QApplication::setOrganizationName("ce216");

    QQmlApplicationEngine engine;

    // QFileDialog'ı QML'de kullanmak için context property olarak ekleniyor
    FileDialogHelper fileDialogHelper;
    engine.rootContext()->setContextProperty("fileDialogHelper", &fileDialogHelper);

    // QML_SINGLETON olarak IAE sınıfını tanıt
    static IAE iaeInstance;
    qmlRegisterSingletonInstance("ce216", 1, 0, "IAE", &iaeInstance);

    // Yükleme başarısızsa uygulamayı sonlandır
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [=](QObject *obj, const QUrl &objUrl) {
        if (!obj && objUrl == QUrl(QStringLiteral("qrc:/src/qml/mainWindow.qml"))) {
            QCoreApplication::exit(-1);
        }
    }, Qt::QueuedConnection);

    engine.load(QUrl(QStringLiteral("qrc:/src/qml/mainWindow.qml")));

    return app.exec();
}
