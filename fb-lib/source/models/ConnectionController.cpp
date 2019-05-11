#include <boost/msm/back/state_machine.hpp>
#include <iostream>
#include "ConnectionController.h"
#include "ConnectionSMImpl.h"

namespace fb {
namespace models {

// provide complete type definition
struct ConnectionController::Fsm : public boost::msm::back::state_machine<FsmImpl> {
    explicit Fsm(ConnectionController* ctrl)
        : boost::msm::back::state_machine<FsmImpl>(ctrl) {}
};

ConnectionController::ConnectionController()
    : m_fsm(std::make_shared<Fsm>(this)) {
}

void ConnectionController::startup() {
    m_fsm->process_event(ev_connect{});
}

void ConnectionController::shutdown() {
    m_fsm->process_event(ev_disconnect{});
}

void ConnectionController::do_connect() const {
    std::cout << "do_connect\n";
}

void ConnectionController::do_start() const {
    std::cout << "do_start\n";
}

void ConnectionController::do_stop() const {
    std::cout << "do_stop\n";
}

} //namespace models
} //namespace fb

