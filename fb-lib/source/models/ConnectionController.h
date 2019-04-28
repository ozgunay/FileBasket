#ifndef CONNECTIONCONTROLLER_H
#define CONNECTIONCONTROLLER_H

#include <memory>
#include <fb-lib_global.h>

namespace fb {
namespace models {

class FBLIBSHARED_EXPORT ConnectionController
{
public:
    ConnectionController();
    void startup();
    void shutdown();
    void do_connect() const;
    void do_start() const;
    void do_stop() const;

private:
    // Forward declaration of fsm
    // See http://stackoverflow.com/questions/10899641/boost-msm-class/10922799#10922799
    struct Fsm;
    std::shared_ptr<Fsm> m_fsm;
};

} //namespace models
} //namespace fb

#endif // CONNECTIONCONTROLLER_H
