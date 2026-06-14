#pragma once

#include <cstdint>
#include <optional>
#include <QLayoutItem>
#include <QWidget>

enum class Orientation : uint8_t {
    Horizontal,
    Vertical,
};

class Spacer {
public:
    Spacer(Orientation orient, std::optional<int> value);

    static auto withWidth(int value) -> Spacer;
    static auto withWidth() -> Spacer;
    static auto withHeight(int value) -> Spacer;
    static auto withHeight() -> Spacer;
    auto orientation() -> std::optional<Orientation>;
    auto value() -> std::optional<int>;

private:
    std::optional<int> m_value = std::nullopt; // no value == stretch
    Orientation m_orientation;
};

class Item {
public:
    explicit Item(QWidget *widget);
    Item(QWidget *widget, int stretch, Qt::Alignment alignment);
    explicit Item(Orientation orient, std::optional<int> value);

    [[nodiscard]] auto isWidget() const -> bool;
    [[nodiscard]] auto isSpacer() const -> bool;

    auto widget() -> QWidget *;
    // Stretch factor and alignment recorded for a widget item, so merge() /
    // toItemList() rebuilds reproduce it exactly.
    [[nodiscard]] auto stretch() const -> int;
    [[nodiscard]] auto alignment() const -> Qt::Alignment;
    auto spacer() -> std::optional<Spacer>;

private:
    std::optional<QWidget *> m_widget = std::nullopt;
    int m_stretch = 0;
    Qt::Alignment m_alignment = Qt::Alignment();
    std::optional<Spacer> m_spacer = std::nullopt;
};
