#pragma once

#include <optional>
#include <QHash>
#include <QLoggingCategory>
#include <QString>
#include <QWidget>

namespace qontrol::widgets {

class Widget {

public:
    virtual ~Widget() = default;

    [[nodiscard]] auto key() const -> QString {
        if (m_key.has_value()) {
            return m_key.value();
        }
        return QString();
    };

    [[nodiscard]] auto hasKey() const -> bool {
        return m_key.has_value() && !m_key.value().isEmpty();
    }

    void setKey(const QString &key) {
        m_key = key;
    };

    [[nodiscard]] auto cbegin() const {
        return m_widgets.cbegin();
    }

    [[nodiscard]] auto cend() const {
        return m_widgets.cend();
    }

    virtual auto addWidget(Widget *widget) -> bool {
        if (widget->hasKey()) {
            m_widgets.insert(widget->key(), widget);
            return true;
        }
        qCritical() << "Widget::addWidget() widget don't have key!";
        return false;
    };

    virtual void removeWidget(const QString &name) {
        m_widgets.remove(name);
    }

    virtual auto addWidget(QString &key, Widget *widget) -> bool {
        widget->setKey(key);
        return this->addWidget(widget);
    };

    auto getWidget(const QString &name) -> std::optional<QWidget *> {
        auto *widget = m_widgets.value(name);
        if (widget != nullptr) {
            auto *casted = dynamic_cast<QWidget *>(widget);
            return std::optional<QWidget *>(casted);
        }
        qCritical() << "Widget::getWidget() no widget with name " << name;
        return std::nullopt;
    }

    [[nodiscard]] auto widgets() const -> const QHash<QString, Widget *> * {
        return &m_widgets;
    }

    [[nodiscard]] auto widgetsMut() -> QHash<QString, Widget *> * {
        return &m_widgets;
    }

private:
    std::optional<QString> m_key = std::nullopt;
    QHash<QString, Widget *> m_widgets;
};

} // namespace qontrol::widgets
