#ifndef STRINGDECORATOR_H
#define STRINGDECORATOR_H

#include <QJsonObject>
#include <QJsonValue>
#include <QObject>
#include <QScopedPointer>
#include <QString>

#include <fb-lib_global.h>
#include <data/DataDecorator.h>

namespace fb {
namespace data {

class FBLIBSHARED_EXPORT StringDecorator : public DataDecorator
{
    Q_OBJECT
    Q_PROPERTY( QString ui_value READ value WRITE setValue NOTIFY
        valueChanged )
public:
     StringDecorator(Entity* parentEntity = nullptr, const QString& key =
        "SomeItemKey", const QString& label = "", const QString& value = "");
    ~StringDecorator() override;
    StringDecorator& setValue(const QString& value);
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

#endif // STRINGDECORATOR_H
