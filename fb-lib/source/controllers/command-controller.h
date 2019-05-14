#ifndef COMMANDCONTROLLER_H
#define COMMANDCONTROLLER_H

#include <QObject>
#include <QtQml/QQmlListProperty>
#include <fb-lib_global.h>
#include <framework/command.h>

#include <models/Connection.h>

namespace fb {
namespace controllers {

class FBLIBSHARED_EXPORT CommandController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<fb::framework::Command> ui_createConnectionViewContextCommands READ ui_createConnectionViewContextCommands CONSTANT)
public:
    explicit CommandController(QObject* _parent = nullptr, models::Connection* newConnection = nullptr);
    ~CommandController();
    QQmlListProperty<framework::Command> ui_createConnectionViewContextCommands();
public slots:
    void onCreateClientSaveExecuted();
private:
    class Implementation;
    QScopedPointer<Implementation> implementation;
};

} // namespace controllers
} // namespace fb
#endif
