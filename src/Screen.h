#pragma once

#include "widgets/Slider.h"
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QWidget>
#include <qcheckbox.h>
#include <qhash.h>
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
    virtual void onUnload() {};

protected:
    [[nodiscard]] auto className() const -> const char *;

    // initialisation of contained widgets, called only once
    virtual void init() = 0;
    // layout of the view, called only once
    virtual void view() = 0;
    // connection that should be done on screen loading
    virtual void doConnect() = 0;

private:
    QHash<QString, QPushButton *> m_buttons;
    QHash<QString, QLineEdit *> m_inputs;
    QHash<QString, QCheckBox *> m_checkboxes;
    QHash<QString, QLabel *> m_labels;
    QHash<QString, widgets::Slider *> m_sliders;
    QHash<QString, widgets::ComboBox *> m_comboboxes;
};

} // namespace qontrol
