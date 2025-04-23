#pragma once

#include "common.h"
#include "widgets/Slider.h"
#include <QCheckBox>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QSlider>
#include <QWidget>
#include <qabstractbutton.h>
#include <qcheckbox.h>
#include <qhash.h>
#include <qjsonobject.h>
#include <qjsonvalue.h>
#include <qlineedit.h>
#include <qlogging.h>
#include <qobject.h>
#include <qpushbutton.h>

namespace qontrol {

class Controller;

namespace widgets {

class ComboBox;

} // namespace widgets

class Screen : public QWidget, public qontrol::widgets::Widget {
    Q_OBJECT
public:
    explicit Screen();
    ~Screen() override = default;

    auto button(const QString &name) -> QPushButton *;
    auto input(const QString &name) -> QLineEdit *;
    auto checkbox(const QString &name) -> QCheckBox *;
    auto label(const QString &name) -> QLabel *;
    auto slider(const QString &name) -> widgets::Slider *;
    auto combobox(const QString &name) -> widgets::ComboBox *;

    void insert(const QString &name, QPushButton *widget);
    void insert(const QString &name, QLineEdit *widget);
    void insert(const QString &name, QCheckBox *widget);
    void insert(const QString &name, QLabel *widget);
    void insert(const QString &name, widgets::Slider *widget);
    void insert(const QString &name, widgets::ComboBox *widget);

public slots:
    void onUpdate(SharedJson state);
    virtual void onUnload() {};

signals:
    void update(const SharedJson &payload);

protected:
    [[nodiscard]] auto className() const -> const char *;

    void updateChecked(const SharedJson &map, QAbstractButton *widget);
    void updateEnabled(const SharedJson &map, QWidget *widget);
    void updateVisible(const SharedJson &map, QWidget *widget);
    void updateText(const SharedJson &map, QAbstractButton *widget);
    void updateText(const SharedJson &map, QLineEdit *widget);
    void updateText(const SharedJson &map, QLabel *widget);
    void updateValue(const SharedJson &map, widgets::Slider *widget);

    void updateIsNotNull(const char *function);
    auto keyDoesNotExists(const char *function, QJsonValue *value, const QString &name) -> bool;
    void valueIsNotOfType(const char *function, const QString &name, const QString &t);

    void updateStatePre();

    void updateButtons(Json widgets);
    void updateInputs(Json widgets);
    void updateCheckBoxes(Json widgets);
    void updateLabels(Json widgets);
    void updateSliders(Json widgets);
    void updateComboBoxes(Json widgets);

    void updateButton(Json map);
    void updateInput(Json map);
    void updateCheckBox(Json map);
    void updateLabel(Json map);
    void updateSlider(Json map);
    void updateComboBox(Json map);

    void updateButtonInner(Json map, QPushButton *widget);
    void updateInputInner(Json map, QLineEdit *widget);
    void updateCheckBoxInner(Json map, QCheckBox *widget);
    void updateLabelInner(Json map, QLabel *widget);
    void updateSliderInner(Json map, widgets::Slider *widget);
    void updateComboBoxInner(Json map, widgets::ComboBox *widget);

    static auto stateInner(QPushButton *widget) -> Json;
    static auto stateInner(QLineEdit *widget) -> Json;
    static auto stateInner(QCheckBox *widget) -> Json;
    static auto stateInner(QLabel *widget) -> Json;
    static auto stateInner(widgets::Slider *widget) -> Json;
    static auto stateInner(widgets::ComboBox *widget) -> Json;

    void updateStatePost();

    // initialisation of contained widgets, called only once
    virtual void init() = 0;
    // layout of the view, called only once
    virtual void view() = 0;
    // connection that should be done on screen loading
    virtual void doConnect() = 0;
    // return the actual state aka states of all contained widgets
    virtual auto state() -> Json;

private:
    Json m_update;
    QHash<QString, QPushButton *> m_buttons;
    QHash<QString, QLineEdit *> m_inputs;
    QHash<QString, QCheckBox *> m_checkboxes;
    QHash<QString, QLabel *> m_labels;
    QHash<QString, widgets::Slider *> m_sliders;
    QHash<QString, widgets::ComboBox *> m_comboboxes;
};

} // namespace qontrol
