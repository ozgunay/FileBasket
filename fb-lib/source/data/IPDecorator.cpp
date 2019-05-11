#include "IPDecorator.h"

#include <QVariant>

namespace fb {
namespace data {


class IpDecorator::Implementation
{
public:
    Implementation(IpDecorator* _stringDecorator, const QString& _value)
        : stringDecorator(_stringDecorator)
        , value(_value) {
    }

    IpDecorator* stringDecorator{nullptr};
    QString value;
};

IpDecorator::IpDecorator(Entity* parentEntity, const QString& key,
    const QString& label, const QString& value)
    : DataDecorator(parentEntity, key, label) {
        implementation.reset(new Implementation(this, value));
}

IpDecorator::~IpDecorator() {
}

const QString& IpDecorator::value() const {
    return implementation->value;
}

IpDecorator& IpDecorator::setValue(const QString& value) {
    if(value != implementation->value) {
        // ...Validation here if required...
        boost::system::error_code ec;
        boost::asio::ip::address::from_string( value.toStdString(), ec );
        boost::asio::io_service io;
        if ( ec ) {
            LOG_WARNING << ec.message( );
        }
        implementation->value = value;
        emit valueChanged();
    }
    return *this;
}

QJsonValue IpDecorator::toJson() const {
    return QJsonValue::fromVariant(QVariant(implementation->value));
}

void IpDecorator::fromJson(const QJsonObject& _jsonObject) {
    if (_jsonObject.contains(key())) {
        setValue(_jsonObject.value(key()).toString());
    } else {
        setValue("");
    }
}

} // namespace data
} // namespace fb
