#ifndef CONNECTIONSMIMPL_H
#define CONNECTIONSMIMPL_H

#include <boost/msm/front/functor_row.hpp>
#include <boost/msm/front/state_machine_def.hpp>

#include "ConnectionController.h"

namespace fb {
namespace models {

struct ev_connect {};
struct ev_connected {};
struct ev_disconnect {};

struct ac_connecting {
    template <class FSM, class EVT, class SourceState, class TargetState>
    void operator()(EVT const&, FSM& fsm, SourceState&, TargetState&) {
        fsm.connectionCtrl->do_connect();
    }
};
struct ac_disconnect{
    template <class FSM, class EVT, class SourceState, class TargetState>
    void operator()(EVT const&, FSM& fsm, SourceState&, TargetState&) {
        fsm.connectionCtrl->do_stop();
    }
};

struct ac_connected{
    template <class FSM, class EVT, class SourceState, class TargetState>
    void operator()(EVT const&, FSM& fsm, SourceState&, TargetState&) {
        fsm.connectionCtrl->do_stop();
    }
};

struct Empty : public boost::msm::front::state<> {};
struct Connecting : public boost::msm::front::state<> {};
struct NotConnected : public boost::msm::front::state<> {};
struct Connected : public boost::msm::front::state<> {};

// front-end: define the FSM structure
struct FsmImpl : public boost::msm::front::state_machine_def<FsmImpl> {
    explicit FsmImpl(ConnectionController* connectionCtrlIn)
        : connectionCtrl(connectionCtrlIn) {}

    typedef Empty initial_state;

    struct transition_table
        : boost::mpl::vector<
            //                     Source           Event               Target          Action              Guard
            boost::msm::front::Row<Empty,           ev_connect,         Connecting,     ac_connecting,      boost::msm::front::none>,
            boost::msm::front::Row<Connecting,      ev_connected,       Connected,      ac_connected,       boost::msm::front::none>,
            boost::msm::front::Row<Connecting,      ev_disconnect,      NotConnected,   ac_disconnect,      boost::msm::front::none>
            > {};
    ConnectionController* connectionCtrl;
};

} // namespace models
} // namespace fb

#endif // CONNECTIONSMIMPL_H
