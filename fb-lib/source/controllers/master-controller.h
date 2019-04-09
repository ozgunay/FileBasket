#ifndef MASTERCONTROLLER_H
#define MASTERCONTROLLER_H

#include <QObject>
#include <QScopedPointer>
#include <QString>

#include <fb-lib_global.h>
#include <controllers/navigation-controller.h>

namespace fb {
namespace controllers {

class FBLIBSHARED_EXPORT MasterController : public QObject
{
    Q_OBJECT
    Q_PROPERTY( QString ui_welcomeMessage READ welcomeMessage CONSTANT )
    Q_PROPERTY( fb::controllers::NavigationController*
        ui_navigationController READ navigationController CONSTANT )
public:
    explicit MasterController(QObject *parent = nullptr);
    ~MasterController();
    NavigationController* navigationController();
    const QString& welcomeMessage() const;

private:
    class Implementation;
    QScopedPointer<Implementation> implementation;
};

} //namespace controllers
} //namespace fb

#endif // MASTERCONTROLLER_H
