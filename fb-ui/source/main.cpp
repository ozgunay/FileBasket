#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <controllers/master-controller.h>
// DATA
#include <data/IntDecorator.h>
#include <data/StringDecorator.h>
#include <data/EnumeratorDecorator.h>
#include <data/IPDecorator.h>
// MODELS
#include <models/Connection.h>

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

    qmlRegisterType<fb::data::IntDecorator>("FB", 1, 0, "IntDecorator");
    qmlRegisterType<fb::data::IpDecorator>("FB", 1, 0, "IpDecorator");
    qmlRegisterType<fb::data::StringDecorator>("FB", 1, 0, "StringDecorator");
    qmlRegisterType<fb::data::EnumeratorDecorator>("FB", 1, 0,
        "EnumeratorDecorator");
    qmlRegisterType<fb::models::Connection>("FB", 1, 0, "Connection");

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
