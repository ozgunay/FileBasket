#ifndef INTDECORATOR_H
#define INTDECORATOR_H

#include <QJsonObject>
#include <QJsonValue>
#include <QObject>
#include <QScopedPointer>
#include <QString>

#include <fb-lib_global.h>
#include <data/DataDecorator.h>


namespace fb {
namespace data {

class FBLIBSHARED_EXPORT IntDecorator : public DataDecorator
{
    Q_OBJECT
    Q_PROPERTY( int ui_value READ value WRITE setValue NOTIFY
        valueChanged )
public:
     IntDecorator(Entity* parentEntity = nullptr, const QString& key =
        "SomeItemKey", const QString& label = "",  int value = 0);
    ~IntDecorator() override;
    IntDecorator& setValue(int value);
    int value() const;
    QJsonValue toJson() const override;
    void fromJson(const QJsonObject& jsonObject) override;

signals:
    void valueChanged();

private:
    class Implementation;
    QScopedPointer<Implementation> implementation;
};

} // namespace data
} // namespace fb

#endif // INTDECORATOR_H
