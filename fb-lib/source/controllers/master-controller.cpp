#include "master-controller.h"

using namespace fb::models;

namespace fb {
namespace controllers {

class MasterController::Implementation
{
public:
    Implementation(MasterController* _masterController)
        : masterController(_masterController) {
        navigationController = new NavigationController(masterController);
        commandController = new CommandController(masterController);
        newConnection = new Connection(masterController);
    }
    MasterController* masterController{nullptr};
    NavigationController* navigationController{nullptr};
    CommandController* commandController{nullptr};
    Connection* newConnection{nullptr};
    QString welcomeMessage = "File Basket is a ssh file transfer software.";
};

MasterController::MasterController(QObject *parent)
    : QObject(parent) {
    implementation.reset(new Implementation(this));
}

MasterController::~MasterController() {

}

NavigationController* MasterController::navigationController() {
    return implementation->navigationController;
}

CommandController* MasterController::commandController() {
    return implementation->commandController;
}

const QString& MasterController::welcomeMessage() const {
    return implementation->welcomeMessage;
}

Connection* MasterController::newConnection() {
    return implementation->newConnection;
}


} // namespace controllers
} // namesapce fb
