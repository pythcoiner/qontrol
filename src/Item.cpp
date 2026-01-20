#include "Item.h"
#include <optional>

Item::Item(QWidget *widget) {
    m_widget = std::make_optional(widget);
}

auto Item::isWidget() const -> bool {
    return m_widget.has_value();
}

auto Item::isSpacer() const -> bool {
    return m_spacer.has_value();
}

auto Item::widget() -> QWidget * {
    return m_widget.has_value() ? m_widget.value() : nullptr;
}

auto Item::spacer() -> std::optional<Spacer> {
    return m_spacer;
}

Item::Item(Orientation orient, std::optional<int> value) {
    if (value.has_value() && orient == Orientation::Horizontal) {
        m_spacer = Spacer::withWidth(value.value());
    } else if (!value.has_value() && orient == Orientation::Horizontal) {
        m_spacer = Spacer::withWidth();
    } else if (value.has_value() && orient == Orientation::Vertical) {
        m_spacer = Spacer::withHeight(value.value());
    } else {
        m_spacer = Spacer::withHeight();
    }
}

Spacer::Spacer(Orientation orient, std::optional<int> value) {
    m_orientation = orient;
    m_value = value;
}

auto Spacer::withWidth(int value) -> Spacer {
    return Spacer(Orientation::Horizontal, value);
}

auto Spacer::withHeight(int value) -> Spacer {
    return Spacer(Orientation::Vertical, value);
}

auto Spacer::withWidth() -> Spacer {
    return Spacer(Orientation::Horizontal, std::nullopt);
}

auto Spacer::withHeight() -> Spacer {
    return Spacer(Orientation::Vertical, std::nullopt);
}

auto Spacer::orientation() -> std::optional<Orientation> {
    return m_orientation;
}

auto Spacer::value() -> std::optional<int> {
    return m_value;
}
