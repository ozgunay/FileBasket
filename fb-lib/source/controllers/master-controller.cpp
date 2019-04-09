#include "master-controller.h"

namespace fb {
namespace controllers {

class MasterController::Implementation
{
public:
    Implementation(MasterController* _masterController)
        : masterController(_masterController) {
        navigationController = new NavigationController(masterController);
    }
    MasterController* masterController;
    NavigationController* navigationController;
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

const QString& MasterController::welcomeMessage() const {
    return implementation->welcomeMessage;
}


} // namespace controllers
} // namesapce fb
