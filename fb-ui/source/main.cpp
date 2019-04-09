#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <controllers/master-controller.h>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qmlRegisterType<fb::controllers::MasterController>("FB", 1, 0,
        "MasterController");
    qmlRegisterType<fb::controllers::NavigationController>("FB", 1, 0,
        "NavigationController");

    fb::controllers::MasterController masterController;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("masterController",
        &masterController);
    engine.load(QUrl(QStringLiteral("qrc:/views/MasterView.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
