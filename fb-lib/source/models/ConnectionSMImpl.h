#ifndef CONNECTIONSMIMPL_H
#define CONNECTIONSMIMPL_H

#include <boost/msm/front/functor_row.hpp>
#include <boost/msm/front/state_machine_def.hpp>

#include "Connection.h"

namespace fb {
namespace models {

struct ev_connect {};
struct ev_connected {};
struct ev_disconnect {};
struct ev_notReachable {};

struct ac_connecting {
    template <class FSM, class EVT, class SourceState, class TargetState>
    void operator()(EVT const&, FSM& fsm, SourceState&, TargetState&) {
        LOG_INFO << "Could not allocate a session.";
        fsm.m_connection->InitSession();
        fsm.m_connection->ConnectSession();
    }
};
struct ac_notConnected{
    template <class FSM, class EVT, class SourceState, class TargetState>
    void operator()(EVT const&, FSM& fsm, SourceState&, TargetState&) {
    }
};

struct ac_connected{
    template <class FSM, class EVT, class SourceState, class TargetState>
    void operator()(EVT const&, FSM& fsm, SourceState&, TargetState&) {
    }
};

struct Empty : public boost::msm::front::state<> {};
struct Connecting : public boost::msm::front::state<> {};
struct NotConnected : public boost::msm::front::state<> {};
struct Connected : public boost::msm::front::state<> {};

// front-end: define the FSM structure
struct FsmImpl : public boost::msm::front::state_machine_def<FsmImpl> {
    explicit FsmImpl(Connection* connectionIn)
        : m_connection(connectionIn) {}

    typedef Empty initial_state;

    struct transition_table
        : boost::mpl::vector<
            //                     Source           Event               Target          Action              Guard
            boost::msm::front::Row<Empty,           ev_connect,         Connecting,     ac_connecting,      boost::msm::front::none>,
            boost::msm::front::Row<Connecting,      ev_connected,       Connected,      ac_connected,       boost::msm::front::none>,
            boost::msm::front::Row<Connecting,      ev_notReachable,    NotConnected,   ac_notConnected,    boost::msm::front::none>,
            boost::msm::front::Row<Connected,       ev_disconnect,      NotConnected,   ac_notConnected,    boost::msm::front::none>
            > {};
    Connection* m_connection;
};

} // namespace models
} // namespace fb

#endif // CONNECTIONSMIMPL_H
