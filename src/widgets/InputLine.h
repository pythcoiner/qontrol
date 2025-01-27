#pragma once

#include "../Row.h"
#include "Input.h"
#include "Widget.h"
#include <qlabel.h>
#include <qvalidator.h>

namespace qontrol::widgets {

class InputLine: public Row, public Widget {
    Q_OBJECT
public:
    InputLine(const QString& name);
    auto label(const QString &label) -> InputLine*;
    auto input(InputType input_type) -> InputLine*;
    auto validator(QValidator *validator) -> InputLine*;
    auto inputWidth(float factor) -> InputLine*;
    auto unit(const QString &label) -> InputLine*;
    auto setEnabled(bool enabled) -> InputLine*;
    auto setNoFill(bool enabled) -> InputLine*;

    [[nodiscard]] auto hasValue() const -> bool override;
    [[nodiscard]] auto value() const -> QJsonValue override;
    [[nodiscard]] auto isIterable() const -> bool override;
    auto input() -> Input*;
    void setValue(int value);
    void setValue(const QString &value);
    void setValue(double value);
    void loadValue(const QJsonValue &value, int depth = 0) override;


private:
    void update();

    QLabel *m_label = nullptr;
    Input *m_input = nullptr;
    QLabel *m_unit = nullptr;
    bool m_no_fill = false;
};

} // namespace qontrol::widgets
