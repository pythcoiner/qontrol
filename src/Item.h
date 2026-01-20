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
    explicit Item(Orientation orient, std::optional<int> value);

    [[nodiscard]] auto isWidget() const -> bool;
    [[nodiscard]] auto isSpacer() const -> bool;

    auto widget() -> QWidget *;
    auto spacer() -> std::optional<Spacer>;

private:
    std::optional<QWidget *> m_widget = std::nullopt;
    std::optional<Spacer> m_spacer = std::nullopt;
};
