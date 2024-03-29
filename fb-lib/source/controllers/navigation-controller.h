#ifndef NAVIGATIONCONTROLLER_H
#define NAVIGATIONCONTROLLER_H

#include <QObject>

#include <fb-lib_global.h>

namespace fb {
namespace controllers {

class FBLIBSHARED_EXPORT NavigationController : public QObject
{
    Q_OBJECT
public:
    explicit NavigationController(QObject* _parent = nullptr)
        : QObject(_parent)
    {}
signals:
    void goCreateConnectionView();
    void goDashboardView();
    void goEditConnectionView();
    void goFindConnectionView();
};

} // namespace controllers
} // namespace fb

#endif // NAVIGATIONCONTROLLER_H
