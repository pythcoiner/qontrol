#include "Screen.h"
#include "Controller.h"
#include "common.h"
#include "widgets/ComboBox.h"
#include <memory>
#include <qassert.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qjsonobject.h>
#include <qlineedit.h>
#include <qwidget.h>


Screen::Screen() {
    this->setParent(Controller::window());
}

auto Screen::className() const -> const char* {
    return this->metaObject()->className();
}

void Screen::updateIsNotNull(const char* function) {
    if (m_update == nullptr) qFatal() 
        << this->className() << "::" << function << "(): member _update is a nullptr!";
}

auto Screen::keyDoesNotExists(const char* function, QJsonValue* value, const QString &name) -> bool {
    auto verbose = false;
    if (value->isUndefined()) {
        if (verbose) {
            qDebug() 
            << this->className() << "::" << function <<"(): key " << name << " does not exists!";
        }
        return true; 
    }
    return false;
}

void Screen::valueIsNotOfType(const char* function, const QString &name, const QString &t) {
    qDebug() 
        << this->className() << "::" << function << "(): value at key " 
        << name << " is not of type " << t << "!";
}

#define UPDATE(attrName, method, valueType, typeCheck, typeConvert) \
    auto value = map->value(attrName); \
    if (this->keyDoesNotExists(__func__, &value, attrName)) return; \
    qDebug() << "updating attribute " << attrName;\
    if (value.typeCheck()) { \
        auto converted = value.typeConvert();\
        qDebug() << "with value: " << converted;\
        widget->method(converted); \
    } else { \
        this->valueIsNotOfType(__func__, attrName, valueType); \
    }

void Screen::updateChecked(const SharedJson &map, QAbstractButton *widget) {
    UPDATE("checked", setChecked, "Bool", isBool, toBool)
}

void Screen::updateEnabled(const SharedJson &map, QWidget *widget) {
    UPDATE("enabled", setEnabled, "Bool", isBool, toBool)
}

void Screen::updateVisible(const SharedJson &map, QWidget *widget) {
    UPDATE("visible", setVisible, "Bool", isBool, toBool)
}

void Screen::updateText(const SharedJson &map, QAbstractButton *widget) {
    UPDATE("text", setText, "String", isString, toString)
}

void Screen::updateText(const SharedJson &map, QLineEdit *widget) {
    UPDATE("text", setText, "String", isString, toString)
}

void Screen::updateText(const SharedJson &map, QLabel *widget) {
    UPDATE("text", setText, "String", isString, toString)
}

void Screen::updateValue(const SharedJson &map, Slider *widget) {
    UPDATE("value", setValue, "Int", isDouble, toInt)
}

#define GETTER(member, typeName) \
    if (!this->member.contains(name)) { \
        qFatal() << this->className() << " does not contain " << typeName << " with name " << name; \
        return nullptr; \
    } \
    return this->member.value(name);

auto Screen::button(const QString &name) -> QPushButton* {
    GETTER(m_buttons, "button")
}

auto Screen::input(const QString &name) -> QLineEdit* {
    GETTER(m_inputs, "input")
}

auto Screen::checkbox(const QString &name) -> QCheckBox* {
    GETTER(m_checkboxes, "checkbox")
}
auto Screen::label(const QString &name) -> QLabel* {
    GETTER(m_labels, "label")
}
auto Screen::slider(const QString &name) -> Slider* {
    GETTER(m_sliders, "slider")
}
auto Screen::combobox(const QString &name) -> ComboBox* {
    GETTER(m_comboboxes, "combobox")
}

#define INSERT(member) \
    widget->setParent(this); \
    auto *replaced = this->member.take(name); \
    this->member.insert(name, widget); \
    delete replaced;

void Screen::insert(const QString &name, QPushButton* widget) {
    INSERT(m_buttons)
}
void Screen::insert(const QString &name, QLineEdit* widget) {
    INSERT(m_inputs)
}
void Screen::insert(const QString &name, QCheckBox* widget) {
    INSERT(m_checkboxes)
}
void Screen::insert(const QString &name, QLabel* widget) {
    INSERT(m_labels)
}
void Screen::insert(const QString &name, Slider* widget) {
    INSERT(m_sliders)
}
void Screen::insert(const QString &name, ComboBox* widget) {
    INSERT(m_comboboxes)
}

void Screen::onUpdate(SharedJson state) { // NOLINT
    // if there is only one ref it's safe to cast to an unique_ptr
    Q_ASSERT(state.use_count() == 1);

    m_update = newJson(state);
    this->updateStatePre();

    #define UPDATES(attr, handlerFunction) \
        if (m_update->contains(attr)) { \
            auto value = m_update->take(attr); \
            if (value.isObject()) { \
                auto key = value.toObject(); \
                handlerFunction(std::make_unique<QJsonObject>(key)); \
            } else { \
                qCritical() << this->className() << "::" << __func__ << "(): " \
                    << attr << " map is not of type QJsonObject!"; \
            } \
        }

    UPDATES("buttons", updateButtons)
    UPDATES("inputs", updateInputs)
    UPDATES("checkboxes", updateCheckBoxes)
    UPDATES("labels", updateLabels)
    UPDATES("sliders", updateSliders)

    this->updateStatePost();
    m_update = nullptr;
}

void Screen::updateStatePre() {}

#define UPDATE_WIDGETS(method) \
    for (auto wdg = widgets->begin(); wdg != widgets->end(); ++wdg) { \
        QString name = wdg.key(); \
        auto value = wdg.value(); \
        if (value.isObject()) { \
            auto widget = value.toObject(); \
            widget["name"] = name; \
            this->method(std::make_unique<QJsonObject>(widget)); \
        } else { \
            qCritical() << this->className() << "::" << __func__ << ": " << name \
                        << "is not of type QJsonObject!"; \
            continue; \
        } \
    } \

void Screen::updateButtons(Json widgets) {
    UPDATE_WIDGETS(updateButton)
}
void Screen::updateInputs(Json widgets) {
    UPDATE_WIDGETS(updateInput)
}
void Screen::updateCheckBoxes(Json widgets) {
    UPDATE_WIDGETS(updateCheckBox)
}
void Screen::updateLabels(Json widgets) {
    UPDATE_WIDGETS(updateLabel)
}
void Screen::updateSliders(Json widgets) {
    UPDATE_WIDGETS(updateSlider)
}
void Screen::updateComboBoxes(Json widgets) {
    UPDATE_WIDGETS(updateComboBox)
}

#define UPDATE_WIDGET(member, typeName, innerMethod) \
    if (map->isEmpty()) return;\
    QString name;\
    if (!map->contains("name")) {\
        qFatal() << this->className() << "::" << __func__ << ": name is missing!";\
        return;\
    } \
    name = map->value("name").toString();\
    if (name.isEmpty()) {\
        qFatal() << this->className() << "::" << __func__ << ": name is empty!";\
        return;\
    }\
    qDebug() << "Updating " << typeName << " " << name;\
    if (this->member.contains(name)) {\
        auto edited = this->member.value(name);\
        if (edited == nullptr) {\
            qFatal() << this->className() << "::" << __func__ << ": " << name \
                << typeName << " missing from the internal state!";\
            return;\
        }\
        innerMethod(std::move(map), edited);\
    }

void Screen::updateButton(Json map) {
    UPDATE_WIDGET(m_buttons, "button", updateButtonInner)
}
void Screen::updateButtonInner(Json map, QPushButton *widget) {
    std::shared_ptr<QJsonObject> sharedMap = std::move(map);
    this->updateChecked(sharedMap, widget);
    this->updateEnabled(sharedMap, widget);
    this->updateText(sharedMap, widget);
    this->updateVisible(sharedMap, widget);
}

auto Screen::stateInner(QPushButton *widget) -> Json {
    Json json = newJson();
    json->insert("checked", widget->isChecked());
    json->insert("enabled", widget->isEnabled());
    json->insert("text", widget->text());
    json->insert("visible", widget->isVisible());
    return std::move(json);
}

void Screen::updateInput(Json map) {
    UPDATE_WIDGET(m_inputs, "input", updateInputInner)
}
void Screen::updateInputInner(Json map, QLineEdit *widget) {
    std::shared_ptr<QJsonObject> sharedMap = std::move(map);
    this->updateEnabled(sharedMap, widget);
    this->updateText(sharedMap, widget);
    this->updateVisible(sharedMap, widget);
}

auto Screen::stateInner(QLineEdit *widget) -> Json {
    Json json = newJson();
    json->insert("enabled", widget->isEnabled());
    json->insert("text", widget->text());
    json->insert("visible", widget->isVisible());
    return std::move(json);
}

void Screen::updateCheckBox(Json map) {
    UPDATE_WIDGET(m_checkboxes, "checkbox", updateCheckBoxInner)
}
void Screen::updateCheckBoxInner(Json map, QCheckBox *widget) {
    std::shared_ptr<QJsonObject> sharedMap = std::move(map);
    this->updateChecked(sharedMap, widget);
    this->updateEnabled(sharedMap, widget);
    this->updateText(sharedMap, widget);
    this->updateVisible(sharedMap, widget);
}

auto Screen::stateInner(QCheckBox *widget) -> Json {
    Json json = newJson();
    json->insert("checked", widget->isChecked());
    json->insert("enabled", widget->isEnabled());
    json->insert("text", widget->text());
    json->insert("visible", widget->isVisible());
    return std::move(json);
}

void Screen::updateLabel(Json map) {
    UPDATE_WIDGET(m_labels, "label", updateLabelInner)
}
void Screen::updateLabelInner(Json map, QLabel *widget) {
    std::shared_ptr<QJsonObject> sharedMap = std::move(map);
    this->updateEnabled(sharedMap, widget);
    this->updateText(sharedMap, widget);
    this->updateVisible(sharedMap, widget);
}

auto Screen::stateInner(QLabel *widget) -> Json {
    Json json = newJson();
    json->insert("enabled", widget->isEnabled());
    json->insert("text", widget->text());
    json->insert("visible", widget->isVisible());
    return std::move(json);
}

void Screen::updateSlider(Json map) {
    UPDATE_WIDGET(m_sliders, "slider", updateSliderInner)
}
void Screen::updateSliderInner(Json map, Slider *widget) {
    std::shared_ptr<QJsonObject> sharedMap = std::move(map);
    this->updateEnabled(sharedMap, widget);
    this->updateVisible(sharedMap, widget);
    this->updateValue(sharedMap, widget);
}

auto Screen::stateInner(Slider *widget) -> Json {
    Json json = newJson();
    json->insert("enabled", widget->isEnabled());
    json->insert("visible", widget->isVisible());
    json->insert("value", widget->value());
    return std::move(json);
}

void Screen::updateComboBox(Json map) {
    UPDATE_WIDGET(m_comboboxes, "combobox", updateComboBoxInner)
}
void Screen::updateComboBoxInner(Json map, ComboBox *widget) {
    std::shared_ptr<QJsonObject> sharedMap = std::move(map);
    this->updateEnabled(sharedMap, widget);
    this->updateVisible(sharedMap, widget);
}

auto Screen::stateInner(ComboBox *widget) -> Json {
    Json json = newJson();
    json->insert("enabled", widget->isEnabled());
    json->insert("visible", widget->isVisible());
    return std::move(json);
}

void Screen::updateStatePost() {}

#define STATE(key, member) \
    { \
        Json widgets = newJson(); \
        for (auto [name, widget] : this->member.asKeyValueRange()) { \
            Json map = this->stateInner(widget); \
            widgets->insert(name, *map); \
        } \
        state->insert(key, *widgets); \
    }

auto Screen::state() -> Json {
    Json state = newJson();
    STATE("buttons", m_buttons)
    STATE("inputs", m_inputs)
    STATE("checkboxes", m_checkboxes)
    STATE("labels", m_labels)
    STATE("sliders", m_sliders)

    return std::move(state);
}



