#include "command-controller.h"

#include <QList>
#include <QDebug>

using namespace fb::framework;
using namespace fb::models;

namespace fb {
namespace controllers {

class CommandController::Implementation
{
public:
    Implementation(CommandController* _commandController, Connection* _newConnection)
        : commandController(_commandController),
        newConnection(_newConnection) {
        Command* createClientSaveCommand = new Command(
            commandController, QChar( 0xf1e6 ), "Connect" );
        QObject::connect( createClientSaveCommand, &Command::executed,
            commandController, &CommandController::onCreateClientSaveExecuted );
        createClientViewContextCommands.append( createClientSaveCommand );
    }
    CommandController* commandController{nullptr};
    QList<Command*> createClientViewContextCommands{};
    Connection* newConnection{nullptr};
};

CommandController::CommandController(QObject* parent, Connection* newConnection)
    : QObject(parent) {
    implementation.reset(new Implementation(this, newConnection));
}

CommandController::~CommandController() {
}

QQmlListProperty<Command>
CommandController::ui_createConnectionViewContextCommands() {
    return QQmlListProperty<Command>(this,
        implementation->createClientViewContextCommands);
}

void CommandController::onCreateClientSaveExecuted() {
    qDebug() << "You executed the Save command!";
    implementation->newConnection->connectionSettingsSaved();
}

} // namespace controllers
} // namespace fb
