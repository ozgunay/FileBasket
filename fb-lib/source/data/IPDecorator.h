#ifndef IPDECORATOR_H
#define IPDECORATOR_H

#include <QJsonObject>
#include <QJsonValue>
#include <QObject>
#include <QScopedPointer>
#include <QString>

#include <fb-lib_global.h>
#include <data/DataDecorator.h>
#include <boost/asio.hpp>
#include <logger/fb-logger.h>

namespace fb {
namespace data {

class FBLIBSHARED_EXPORT IpDecorator : public DataDecorator
{
    Q_OBJECT
    Q_PROPERTY( QString ui_value READ value WRITE setValue NOTIFY
        valueChanged )
public:
     IpDecorator(Entity* parentEntity = nullptr, const QString& key =
        "SomeItemKey", const QString& label = "", const QString& value = "");
    ~IpDecorator() override;
    void setValue(const QString& value);
    const QString& value() const;
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

#endif // IPDECORATOR_H
