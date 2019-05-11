#include "EnumeratorDecorator.h"

#include <QVariant>

namespace fb {
namespace data {


class EnumeratorDecorator::Implementation
{
public:
    Implementation(EnumeratorDecorator* _enumeratorDecorator, int _value,
                   const std::map<int, QString>& _descriptionMapper)
        : enumatorDecorator(_enumeratorDecorator)
        , value(_value)
        , descriptionMapper(_descriptionMapper){
    }

    EnumeratorDecorator* enumatorDecorator{nullptr};
    int value;
    std::map<int, QString> descriptionMapper;
};

EnumeratorDecorator::EnumeratorDecorator(Entity* parentEntity, const QString& key,
                        const QString& label, int value, const std::map<int, QString>&
                                         descriptionMapper)
    : DataDecorator(parentEntity, key, label) {
        implementation.reset(new Implementation(this, value, descriptionMapper));
}

EnumeratorDecorator::~EnumeratorDecorator() {
}

int EnumeratorDecorator::value() const {
    return implementation->value;
}

EnumeratorDecorator& EnumeratorDecorator::setValue(int value) {
    if(value != implementation->value) {
        // ...Validation here if required...
        implementation->value = value;
        emit valueChanged();
    }
    return *this;
}

QString EnumeratorDecorator::valueDescription() const {
    if (implementation->descriptionMapper.find(implementation->value)
        != implementation->descriptionMapper.end()) {
        return implementation->descriptionMapper.at(implementation->value);
    } else {
    return {};
    }
}

QJsonValue EnumeratorDecorator::toJson() const {
    return QJsonValue::fromVariant(QVariant(implementation->value));
}

void EnumeratorDecorator::fromJson(const QJsonObject& _jsonObject) {
    if (_jsonObject.contains(key())) {
        setValue(_jsonObject.value(key()).toInt());
    } else {
        setValue(0);
    }
}

} // namespace data
} // namespace fb
