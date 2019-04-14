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
    qmlRegisterType<fb::controllers::CommandController>("FB", 1, 0,
        "CommandController");
    qmlRegisterType<fb::framework::Command>("FB", 1, 0, "Command");

    fb::controllers::MasterController masterController;

    QQmlApplicationEngine engine;
    engine.addImportPath("qrc:/");
    engine.rootContext()->setContextProperty("masterController",
        &masterController);
    engine.load(QUrl(QStringLiteral("qrc:/views/MasterView.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
