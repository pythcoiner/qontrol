#pragma once

#include <QJsonObject>
#include <QJsonValue>
#include <QString>
#include <QWidget>
#include <optional>
#include <qhash.h>
#include <qjsonvalue.h>
#include <qlist.h>
#include <qlogging.h>
#include <qobject.h>

class Widget {

public:
    [[nodiscard]] virtual auto hasValue() const -> bool {
        return false;
    }
    [[nodiscard]] virtual auto isIterable() const -> bool {
        return !m_widgets.isEmpty();
    };

    [[nodiscard]] auto key() const -> QString {
        if (m_key.has_value()) {
            return m_key.value();
        }
        return QString();
    };

    static void loadValue(Widget *widget, const QJsonValue &value, int depth) { //NOLINT(misc-no-recursion)
        if (value.isObject()) {
            auto map = value.toObject();
            for (const auto& key : map.keys()) {
                if (widget->widgets()->contains(key)) {
                    widget->widgetsMut()->value(key)->loadValue(map.value(key), depth);
                } else {
                    qCritical() << "Widget::loadValue() m_widgets do not contains key" << key;
                }
            }
            return;
        }
        qCritical() << "Widget::loadValue() value is not an Object";
    }

    virtual void loadValue(const QJsonValue &value, int depth = 0) { //NOLINT(misc-no-recursion)
        depth++;
        if (depth > MAX_RECURSION) return;
        Widget::loadValue(this, value, depth);
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
        if (widget->hasKey() and widget->key() != "") {
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

    auto getWidget(const QString& name) -> std::optional<QWidget*> {
        auto *widget = m_widgets.value(name);
        if (widget != nullptr) {
            auto *casted = dynamic_cast<QWidget*>(widget);
            return std::optional<QWidget*>(casted);
        }
        qCritical() << "Widget::getWidget() no widget with name " << name;
        return std::nullopt;
    }

    [[nodiscard]] auto widgets() const -> const QHash<QString, Widget*>* {
        return &m_widgets;
    }
    [[nodiscard]] auto widgetsMut() -> QHash<QString, Widget*>* {
        return &m_widgets;
    }

    static void getValue(Widget *widget, QJsonObject *map, int depth = 0) { //NOLINT(misc-no-recursion)
        if (widget->hasValue()) {
            map->insert(widget->key(), widget->value());
        }
        if (widget->isIterable()) {
            depth++;
            if (depth > MAX_RECURSION) return;
            getValue(widget, map, depth);
        }
        if (!widget->hasValue() && !widget->isIterable()) {
            qCritical() << "widget " << widget->key() << "has no value and is not iterable!";
        }
    }

    [[nodiscard]] virtual auto value() const -> QJsonValue { //NOLINT(misc-no-recursion)
        auto map = QJsonObject();
        for (auto *widget : *this->widgets()) {
            getValue(widget, &map);
        }
        return QJsonValue(map);
    }

private:
    static const int MAX_RECURSION = 10;
    std::optional<QString> m_key =  std::nullopt;
    QHash<QString, Widget*> m_widgets;
};

