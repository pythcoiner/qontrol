#include "Screen.h"
#include "Controller.h"
#include "widgets/ComboBox.h"
#include <memory>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qwidget.h>

namespace qontrol {

Screen::Screen() {
    this->setParent(Controller::window());
}

auto Screen::className() const -> const char * {
    return this->metaObject()->className();
}

#define GETTER(member, typeName)                                                                                       \
    if (!this->member.contains(name)) {                                                                                \
        qDebug() << this->className() << " does not contain " << typeName << " with name " << name;                    \
        return nullptr;                                                                                                \
    }                                                                                                                  \
    return this->member.value(name);

auto Screen::button(const QString &name) -> QPushButton * {
    GETTER(m_buttons, "button")
}

auto Screen::input(const QString &name) -> QLineEdit * {
    GETTER(m_inputs, "input")
}

auto Screen::checkbox(const QString &name) -> QCheckBox * {
    GETTER(m_checkboxes, "checkbox")
}

auto Screen::label(const QString &name) -> QLabel * {
    GETTER(m_labels, "label")
}

auto Screen::slider(const QString &name) -> widgets::Slider * {
    GETTER(m_sliders, "slider")
}

auto Screen::combobox(const QString &name) -> widgets::ComboBox *{GETTER(m_comboboxes, "combobox")}

#define INSERT(member)                                                                                                 \
    widget->setParent(this);                                                                                           \
    auto *replaced = this->member.take(name);                                                                          \
    this->member.insert(name, widget);                                                                                 \
    delete replaced;

void Screen::insert(const QString &name, QPushButton *widget) {
    INSERT(m_buttons)
}

void Screen::insert(const QString &name, QLineEdit *widget) {
    INSERT(m_inputs)
}

void Screen::insert(const QString &name, QCheckBox *widget) {
    INSERT(m_checkboxes)
}

void Screen::insert(const QString &name, QLabel *widget) {
    INSERT(m_labels)
}

void Screen::insert(const QString &name, widgets::Slider *widget) {
    INSERT(m_sliders)
}

void Screen::insert(const QString &name, widgets::ComboBox *widget) {
    INSERT(m_comboboxes)
}

} // namespace qontrol
