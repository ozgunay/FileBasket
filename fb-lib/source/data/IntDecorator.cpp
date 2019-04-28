#include "IntDecorator.h"

#include <QVariant>

namespace fb {
namespace data {


class IntDecorator::Implementation
{
public:
    Implementation(IntDecorator* _intDecorator, int _value)
        : intDecorator(_intDecorator)
        , value(_value) {
    }

    IntDecorator* intDecorator{nullptr};
    int value;
};

IntDecorator::IntDecorator(Entity* parentEntity, const QString& key,
    const QString& label, int value)
    : DataDecorator(parentEntity, key, label) {
        implementation.reset(new Implementation(this, value));
}

IntDecorator::~IntDecorator() {
}

int IntDecorator::value() const {
    return implementation->value;
}

void IntDecorator::setValue(int value) {
    if(value != implementation->value) {
        // ...Validation here if required...
        implementation->value = value;
        emit valueChanged();
    }
}

QJsonValue IntDecorator::toJson() const {
    return QJsonValue::fromVariant(QVariant(implementation->value));
}

void IntDecorator::fromJson(const QJsonObject& _jsonObject) {
    if (_jsonObject.contains(key())) {
        setValue(_jsonObject.value(key()).toInt());
    } else {
        setValue(0);
    }
}

} // namespace data
} // namespace fb
