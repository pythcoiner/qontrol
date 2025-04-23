#include "ComboLine.h"
#include "../common.h"
#include <qlabel.h>
#include <qlogging.h>
#include <qwidget.h>

namespace qontrol::widgets {

ComboLine::ComboLine(const QString &name) {
    this->setKey(name);
}

auto ComboLine::label(const QString &label) -> ComboLine * {
    m_label = new QLabel(label, this);
    m_label->setFixedWidth(QONTROL_LABEL_WIDTH);
    this->update();
    return this;
}

auto ComboLine::combo(const QString &enum_name) -> ComboLine * {
    m_combo = new ComboBox(enum_name, this);
    m_combo->setFixedWidth(QONTROL_INPUT_WIDTH);
    this->update();
    return this;
}

auto ComboLine::comboWidth(float factor) -> ComboLine * {
    if (m_combo != nullptr) {
        auto f = (int)(factor * 1000);
        m_combo->setFixedWidth(QONTROL_INPUT_WIDTH * f / 1000);
    }
    return this;
}

auto ComboLine::unit(const QString &label) -> ComboLine * {
    m_unit = new QLabel(label, this);
    m_unit->setFixedWidth(QONTROL_UNIT_WIDTH);
    this->update();
    return this;
}

void ComboLine::update() {
    this->clear();
    this->pushSpacer(QONTROL_LEFT_MARGIN);
    if (m_label != nullptr) {
        this->push(m_label)->pushSpacer(QONTROL_H_SPACER);
    }
    if (m_combo != nullptr) {
        this->push(m_combo)->pushSpacer(QONTROL_H_SPACER);
    }
    if (m_unit != nullptr) {
        this->push(m_unit);
    }
    if (!m_no_fill) {
        this->pushSpacer();
    }
}

auto ComboLine::hasValue() const -> bool {
    return m_combo != nullptr;
}

auto ComboLine::value() const -> QJsonValue {
    return m_combo->value();
}

auto ComboLine::isIterable() const -> bool {
    return false;
}

auto ComboLine::combo() -> ComboBox * {
    return this->m_combo;
}

auto ComboLine::setEnabled(bool enabled) -> ComboLine * {
    if (m_combo != nullptr) {
        m_combo->setEnabled(enabled);
    }
    return this;
}

auto ComboLine::setNoFill(bool enabled) -> ComboLine * {
    m_no_fill = enabled;
    this->update();
    return this;
}

void ComboLine::setCurrentIndex(int index) {
    if (m_combo != nullptr) {
        if (m_combo->count() > index) {
            m_combo->setCurrentIndex(index);
        } else {
            qCritical() << "ComboLine::setCurrentIndex() index > count-1";
        }
    } else {
        qCritical() << "ComboLine::setCurrentIndex() m_combo is nullptr";
    }
}

void ComboLine::loadValue(const QJsonValue &value, int depth) {
    if (m_combo != nullptr) {
        m_combo->loadValue(value);
    }
}

} // namespace qontrol::widgets
