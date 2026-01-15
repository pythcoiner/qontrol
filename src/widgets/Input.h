#pragma once

#include "Widget.h"
#include <QLineEdit>
#include <QString>
#include <QWidget>
#include <qobject.h>

namespace qontrol::widgets {

enum class InputType : uint8_t {
    String,
    Int,
    Float,
};

class Input : public QLineEdit, Widget {
    Q_OBJECT

public:
    explicit Input(InputType input_type, QWidget *parent = nullptr);
    Input(const QString &name, InputType input_type, QWidget *parent = nullptr);

    void setType(InputType input_type);

signals:
    void valueChanged();

public slots:
    void onUpdate();
    void onValueChanged(int _v); // NOLINT

private:
    InputType m_type;
};

} // namespace qontrol::widgets
