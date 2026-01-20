#include "Slider.h"
#include "../Column.h"
#include "../Row.h"
#include "../common.h"
#include <algorithm>
#include <cstdarg>
#include <qcheckbox.h>
#include <qcontainerfwd.h>
#include <qcoreevent.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlist.h>
#include <qlogging.h>
#include <qmap.h>
#include <qnamespace.h>
#include <qpen.h>
#include <qslider.h>
#include <qwidget.h>

namespace qontrol::widgets {

SliderHeader::SliderHeader(Slider *parent) : QWidget(parent) {
    m_pen = new QPen(Qt::white);
    m_parent = parent;
    m_labels = new QMap<int, QPair<QRect, QString>>();
}

SliderHeader::~SliderHeader() {
    delete m_pen;
    delete m_labels;
}

void SliderHeader::setPen(QPen *pen) {
    delete (m_pen);
    m_pen = pen;
}

void filter(QMap<int, QPair<QRect, QString>> *map) {
    if ((map == nullptr) || map->size() < 2)
        return;

    auto *out = new QMap<int, QRect>;

    for (auto iter = map->begin(); iter != map->end(); ++iter) {
        bool insert{false};

        // we always insert firt element
        if (iter == map->begin()) {
            insert = true;
        } else {
            bool intersectLastOut = iter.value().first.intersects((--out->end()).value());
            bool intersectLastMap =
                iter.value().first.intersects((--map->end()).value().first) && iter != (--map->end());
            if (!intersectLastOut && !intersectLastMap) {
                insert = true;
            }
        }

        if (insert) {
            out->insert(iter.key(), iter.value().first);
        }
    }

    for (auto k : map->keys()) {
        if (!out->contains(k)) {
            map->remove(k);
        }
    }
    delete (out);
}

void SliderHeader::paintEvent(QPaintEvent *ev) {
    const auto *map = m_parent->ticksMap();
    if ((map == nullptr) || map->isEmpty())
        return;
    QPainter painter(this);
    painter.setPen(*m_pen);

    QFontMetrics fontMetrics(this->font());

    const int c_handle_width = m_parent->handleWidth();

    auto tickStart = map->firstKey();
    auto tickRange = map->lastKey() - tickStart;
    // NOTE: it should be safer to call parent.load() but can be ressource intensive

    m_labels->clear();
    for (const auto c_tick : *map) {
        auto label = QString::number(c_tick);
        auto tickPosition =
            (((c_tick - tickStart) * (m_parent->slider()->geometry().width() - c_handle_width)) / tickRange);
        int tickX = tickPosition + m_parent->leftSpace();

        // define the bounding rectangle of the text
        QRect position = fontMetrics.boundingRect(label);
        int x(tickX - (position.width() / 2) + (c_handle_width / 2));
        int y((this->height() - position.height()) / 2);
        position.moveTo(x, y);

        m_labels->insert(x, QPair<QRect, QString>(position, label));
    }

    filter(m_labels);

    for (const auto &pair : *m_labels) {
        painter.drawText(pair.first, pair.second);
    }
}

Slider::Slider(int padding, QWidget *parent) : Row(parent) {
    m_pen = nullptr;
    // this->setStyleSheet("background-color: red;");
    m_header = new SliderHeader(this);
    m_header->setFixedHeight(20);
    // this->header->setStyleSheet("background-color: yellow;");
    m_slider = new QSlider(this);
    m_slider->setMinimum(0);
    m_slider->setMaximum(1000);
    m_slider->setOrientation(Qt::Orientation::Horizontal);
    // this->slider->setStyleSheet("background-color: pink;");
    m_ticks = new QMap<int, int>;
    m_spacer = 5;

    m_pre_spacer = padding;
    m_post_spacer = padding;
    m_checkbox = nullptr;
    m_pre_label = nullptr;
    m_post_label = nullptr;
    m_input = nullptr;

    this->load();
}

auto Slider::percent(const QString &label, int label_width, QWidget *parent) -> Slider * {
    auto *slider = new Slider(10, parent);
    slider->m_pre_label = new QLabel(label, slider);
    slider->m_pre_label->setFixedWidth(label_width);
    slider->m_input = new QLineEdit(slider);
    slider->m_input->setFixedWidth(50);
    slider->m_post_label = new QLabel("%");
    QList<int> ticks{0, 100};
    slider->addTicks(&ticks);
    slider->load();
    return slider;
}

auto Slider::cursor(const QString &left_label, int left_label_width, const QString &right_label, int right_label_width,
                    QWidget *parent) -> Slider * {
    auto *slider = new Slider(10, parent);
    slider->m_pre_label = new QLabel(left_label, slider);
    slider->m_pre_label->setFixedWidth(left_label_width);
    slider->m_pre_label->setAlignment(Qt::AlignCenter);
    slider->m_post_label = new QLabel(right_label, slider);
    slider->m_post_label->setFixedWidth(right_label_width);
    slider->m_post_label->setAlignment(Qt::AlignCenter);
    QList<int> ticks{0, 50, 100};
    slider->addTicks(&ticks);
    slider->load();
    slider->m_slider->setValue(50);
    return slider;
}

auto Slider::axis(const QString &axis_name, int label_width, QList<int> ticks, bool optional, bool linear,
                  QWidget *parent) -> Slider * {
    auto *slider = new Slider(10, parent);
    if (optional) {
        slider->m_checkbox = new QCheckBox(slider);
    } else {
        slider->setPreSpacer(slider->getPreSpacer() + 25);
    }
    slider->m_pre_label = new QLabel(axis_name, slider);
    slider->m_pre_label->setFixedWidth(label_width);
    slider->m_input = new QLineEdit(slider);
    slider->m_input->setFixedWidth(50);
    slider->m_post_label = new QLabel(linear ? " mm" : " °", slider);
    slider->m_post_label->setFixedWidth(35);
    slider->addTicks(&ticks);
    slider->load();
    if (optional) {
        slider->m_input->setEnabled(false);
        slider->m_slider->setEnabled(false);
    }
    return slider;
}

auto Slider::simple(QString name, // NOLINT
                    int label_width, QList<int> ticks,
                    QString unit, // NOLINT
                    QWidget *parent) -> Slider * {
    auto *slider = new Slider(10, parent);
    slider->m_pre_label = new QLabel(name, slider);
    slider->m_pre_label->setFixedWidth(label_width);
    slider->m_input = new QLineEdit(slider);
    slider->m_input->setFixedWidth(50);
    slider->m_post_label = new QLabel(unit);
    slider->addTicks(&ticks);
    slider->load();
    return slider;
}

void Slider::changeEvent(QEvent *e) {
    // propagate the enabled state to checkbox
    if ((e->type() == QEvent::EnabledChange) && (m_checkbox != nullptr)) {
        m_checkbox->setEnabled(this->isEnabled());
    }
    QWidget::changeEvent(e);
}

void Slider::setValue(int v) {
    m_slider->setValue(v);
    if (m_input != nullptr) {
        m_input->setText(QString::number(v));
    }
}

auto Slider::value() const -> int {
    return m_slider->value();
}

void Slider::setPreSpacer(int v) {
    m_pre_spacer = v;
}

auto Slider::getPreSpacer() const -> int {
    return m_pre_spacer;
}

auto Slider::ticksMap() -> const QMap<int, int> * {
    return m_ticks;
}

auto Slider::leftSpace() -> int {
    int left = 0;

    if (m_pre_spacer != 0) {
        left += m_pre_spacer;
    }
    if (m_checkbox != nullptr) {
        left += m_checkbox->width();
        left += m_spacer;
    }
    if (m_pre_label != nullptr) {
        left += m_pre_label->width();
        left += m_spacer;
    }
    return left;
}

auto Slider::rightSpace() -> int {
    int right = 0;

    if (m_input != nullptr) {
        right += m_input->width();
        right += m_spacer;
    }
    if (m_post_label != nullptr) {
        right += m_post_label->width();
        right += m_spacer;
    }
    if (m_post_spacer != 0) {
        right += m_post_spacer;
    }
    return right;
}

auto Slider::handleWidth() const -> int {
    return m_slider->height();
}

void Slider::load() {

    auto *row = new Row;
    if (m_pre_spacer != 0) {
        row->pushSpacer(m_pre_spacer);
    }
    if (m_checkbox != nullptr) {
        row->push(m_checkbox)->pushSpacer(m_spacer);
        this->connectCheckbox();
    }
    if (m_pre_label != nullptr) {
        row->push(m_pre_label)->pushSpacer(m_spacer);
    }
    row->push(m_slider);
    if (m_input != nullptr) {
        row->pushSpacer(m_spacer)->push(m_input);
        this->connectInput();
    }
    if (m_post_label != nullptr) {
        row->pushSpacer(m_spacer)->push(m_post_label);
    }
    if (m_post_spacer != 0) {
        row->pushSpacer(m_post_spacer);
    }

    auto *col = new Column;
    col->push(m_header)->push(row);

    this->push(col);
}

void Slider::connectInput() {
    if (m_input != nullptr) {
        connect(m_slider, &QSlider::valueChanged, this, &Slider::sliderValueUpdated, UNIQUE);
        connect(m_input, &QLineEdit::editingFinished, this, &Slider::inputValueUpdated, UNIQUE);
    }
}

void Slider::connectCheckbox() {
    if (m_checkbox != nullptr) {
        if (m_input != nullptr) {
            connect(m_checkbox, &QCheckBox::toggled, m_input, &QLineEdit::setEnabled, UNIQUE);
        }
        connect(m_checkbox, &QCheckBox::toggled, m_slider, &QSlider::setEnabled, UNIQUE);
    }
}

void Slider::sliderValueUpdated(int v) {
    if (m_input != nullptr) {
        m_input->setText(QString::number(v));
    }
    emit this->valueUpdated(v);
}

void Slider::inputValueUpdated() {
    if (m_input != nullptr) {
        auto text = m_input->text();
        bool success = false;
        int value = text.toUInt(&success);
        if (success) {
            auto maxSlider = m_slider->maximum();
            auto minslider = m_slider->minimum();
            value = std::min(maxSlider, value);
            value = std::max(minslider, value);
            m_slider->setValue(value);
            emit this->valueUpdated(value);
        } else {
            qCritical() << "Slider.inputValueUpdated(): input value is not of type UInt!";
        }
    } else {
        qCritical() << "Slider.inputValueUpdated(): input is a nullptr!";
    }
}

void Slider::addTicks(QList<int> *ticks) {
    for (auto tick : *ticks) {
        m_ticks->insert(tick, tick);
    }
    this->updateMinMax();
}

void Slider::updateMinMax() {
    if (m_ticks != nullptr) {
        auto min = m_ticks->firstKey();
        auto max = m_ticks->lastKey();
        m_slider->setRange(min, max);
    }
}

auto Slider::slider() -> QSlider * {
    return m_slider;
}

Slider::~Slider() {
    delete m_ticks;
    delete m_pen;
}

} // namespace qontrol::widgets
