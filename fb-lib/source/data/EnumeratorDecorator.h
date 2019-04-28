#ifndef ENUMERATORDECORATOR_H
#define ENUMERATORDECORATOR_H

#include <map>
#include <QJsonObject>
#include <QJsonValue>
#include <QObject>
#include <QScopedPointer>

#include <fb-lib_global.h>
#include <data/DataDecorator.h>

namespace fb {
namespace data {

class FBLIBSHARED_EXPORT EnumeratorDecorator : public DataDecorator
{

    Q_OBJECT
    Q_PROPERTY( int ui_value READ value WRITE setValue NOTIFY
        valueChanged )
    Q_PROPERTY( QString ui_valueDescription READ valueDescription
        NOTIFY valueChanged )

public:
    EnumeratorDecorator(Entity* parentEntity = nullptr, const QString&
        key = "SomeItemKey", const QString& label = "", int value = 0,
        const std::map<int, QString>& descriptionMapper = std::map<int,
        QString>());
    ~EnumeratorDecorator() override;
    void setValue(int value);
    int value() const;
    QString valueDescription() const;
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

#endif
