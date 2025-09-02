#include "Input.h"
#include <limits>
#include <qjsonvalue.h>
#include <qlogging.h>

namespace qontrol::widgets {

Input::Input(InputType input_type, QWidget *parent) : QLineEdit(parent), m_type(input_type) {
}

Input::Input(const QString &name, InputType input_type, QWidget *parent) : QLineEdit(parent), m_type(input_type) {
    this->setKey(name);
}

auto Input::value() const -> QJsonValue {
    switch (m_type) {
    case InputType::Int:
        return this->intValue();
    case InputType::Float:
        return this->floatValue();
    case InputType::String:
        return this->stringValue();
    }
    qDebug() << "Input::value(): switch unhandled case";
    return QJsonValue();
}

void Input::onUpdate() {
    // TODO:
}

void Input::onValueChanged(int _v) { // NOLINT
    emit this->valueChanged();
}

auto Input::intValue() const -> QJsonValue {
    bool ok = false;
    auto v = this->text().toInt(&ok);
    if (ok) {
        return QJsonValue(v);
    }
    return QJsonValue::Null;
}

auto Input::floatValue() const -> QJsonValue {
    bool ok = false;
    auto v = this->text().toFloat(&ok);
    if (ok) {
        return QJsonValue(v);
    }
    return QJsonValue::Null;
}

auto Input::stringValue() const -> QJsonValue {
    return this->text();
}

void Input::setType(InputType input_type) {
    m_type = input_type;
}

void Input::loadValue(const QJsonValue &value, int depth) {
    switch (m_type) {
    case InputType::String:
        if (value.isString()) {
            this->setText(value.toString());
        } else {
            qCritical() << "Input::loadValue() value is not of type string";
        }
        return;
    case InputType::Float: {
        auto floatDefault = std::numeric_limits<double>::lowest();
        auto floatV = value.toDouble(floatDefault);
        if (floatV != floatDefault) {
            this->setText(QString().setNum(floatV));
        } else {
            qCritical() << "Input::loadValue() value is not of type float";
        }
        return;
    }
    case InputType::Int: {
        auto intDefault = std::numeric_limits<int>::min();
        auto intV = value.toInt(intDefault);
        if (intV != intDefault) {
            this->setText(QString().setNum(intV));
        } else {
            qCritical() << "Input::loadValue() value is not of type int";
        }
        return;
    }
    }
}

} // namespace qontrol::widgets
