#pragma once

#include "../Row.h"
#include "ComboBox.h"
#include "Widget.h"
#include <qlabel.h>

namespace qontrol::widgets {

class ComboLine : public Row, public Widget {
    Q_OBJECT
public:
    ComboLine(const QString &name);
    auto label(const QString &label) -> ComboLine *;
    auto combo(const QString &enum_name) -> ComboLine *;
    auto comboWidth(float factor) -> ComboLine *;
    auto unit(const QString &label) -> ComboLine *;
    auto setEnabled(bool enabled) -> ComboLine *;
    auto setNoFill(bool enabled) -> ComboLine *;

    [[nodiscard]] auto hasValue() const -> bool override;
    [[nodiscard]] auto value() const -> QJsonValue override;
    [[nodiscard]] auto isIterable() const -> bool override;
    auto combo() -> ComboBox *;
    void setCurrentIndex(int index);
    void loadValue(const QJsonValue &value, int depth = 0) override;

private:
    void update();

    QLabel *m_label = nullptr;
    ComboBox *m_combo = nullptr;
    QLabel *m_unit = nullptr;
    bool m_no_fill = false;
};

} // namespace qontrol::widgets
