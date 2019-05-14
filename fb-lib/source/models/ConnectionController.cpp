

namespace fb {
namespace models {



ConnectionController::ConnectionController()
     {
}

void ConnectionController::clientSettingsSaved(const QJsonObject& connectionSettingsIn) const {

}

void ConnectionController::connectToClient() {
    std::cout << "connectToClient\n";
    m_connection = std::make_unique<Connection>();
    m_connection->ConnectSession();
}

void ConnectionController::connectedToClient() const {
    std::cout << "connectedToClient\n";
}

void ConnectionController::disconnectedFromClient() const {
    std::cout << "clientNotConnected\n";
}

} //namespace models
} //namespace fb

