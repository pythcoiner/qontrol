#include "InputLine.h"
#include "../common.h"
#include "Input.h"
#include <QLoggingCategory>

namespace qontrol::widgets {

InputLine::InputLine(const QString &name) {
}

auto InputLine::label(const QString &label) -> InputLine * {
    m_label = new QLabel(label, this);
    m_label->setFixedWidth(QONTROL_LABEL_WIDTH);
    this->update();
    return this;
}

auto InputLine::input(InputType input_type) -> InputLine * {
    m_input = new Input(input_type, this);
    m_input->setFixedWidth(QONTROL_INPUT_WIDTH);
    this->update();
    return this;
}

auto InputLine::validator(QValidator *validator) -> InputLine * {
    m_input->setValidator(validator);
    return this;
}

auto InputLine::inputWidth(float factor) -> InputLine * {
    if (m_input != nullptr) {
        auto f = (int)(factor * 1000);
        m_input->setFixedWidth(QONTROL_INPUT_WIDTH * f / 1000);
    }
    return this;
}

auto InputLine::unit(const QString &label) -> InputLine * {
    m_unit = new QLabel(label, this);
    m_unit->setFixedWidth(QONTROL_UNIT_WIDTH);
    this->update();
    return this;
}

void InputLine::update() {
    this->clear();
    this->pushSpacer(QONTROL_LEFT_MARGIN);
    if (m_label != nullptr) {
        this->push(m_label)->pushSpacer(QONTROL_H_SPACER);
    }
    if (m_input != nullptr) {
        this->push(m_input)->pushSpacer(QONTROL_H_SPACER);
    } else {
    }
    if (m_unit != nullptr) {
        this->push(m_unit);
    }
    if (!m_no_fill) {
        this->pushSpacer();
    }
}

auto InputLine::input() -> Input * {
    return this->m_input;
}

auto InputLine::setEnabled(bool enabled) -> InputLine * {
    if (m_input != nullptr) {
        m_input->setEnabled(enabled);
    }
    return this;
}

auto InputLine::setNoFill(bool enabled) -> InputLine * {
    m_no_fill = enabled;
    this->update();
    return this;
}

void InputLine::setValue(const QString &value) {
    if (m_input != nullptr) {
        m_input->setText(value);
    } else {
        qCritical() << "InputLine::setValue() m_input is nullptr";
    }
}

void InputLine::setValue(int value) {
    this->setValue(QString().setNum(value));
}

void InputLine::setValue(double value) {
    this->setValue(QString().setNum(value));
}

} // namespace qontrol::widgets
