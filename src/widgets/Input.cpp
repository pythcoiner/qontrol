#include "Input.h"
#include <qlogging.h>

namespace qontrol::widgets {

Input::Input(InputType input_type, QWidget *parent) : QLineEdit(parent), m_type(input_type) {
}

Input::Input(const QString &name, InputType input_type, QWidget *parent) : QLineEdit(parent), m_type(input_type) {
}

void Input::onUpdate() {
    // TODO:
}

void Input::onValueChanged(int _v) { // NOLINT
    emit this->valueChanged();
}

void Input::setType(InputType input_type) {
    m_type = input_type;
}

} // namespace qontrol::widgets
