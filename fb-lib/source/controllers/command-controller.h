#ifndef COMMANDCONTROLLER_H
#define COMMANDCONTROLLER_H

#include <QObject>
#include <QtQml/QQmlListProperty>
#include <fb-lib_global.h>
#include <framework/command.h>

#include "models/ConnectionController.h"

namespace fb {
namespace controllers {

class FBLIBSHARED_EXPORT CommandController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<fb::framework::Command>
        ui_createClientViewContextCommands READ
        ui_createClientViewContextCommands CONSTANT)
public:
    explicit CommandController(QObject* _parent = nullptr);
    ~CommandController();
    QQmlListProperty<framework::Command>
    ui_createClientViewContextCommands();
public slots:
    void onCreateClientSaveExecuted();
private:
    class Implementation;
    QScopedPointer<Implementation> implementation;
};

} // namespace controllers
} // namespace fb
#endif
