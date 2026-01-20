#include "Row.h"
#include "Column.h"
#include <qwidget.h>

namespace qontrol {

Row::Row(QWidget *parent) : QWidget(parent) {
    auto *layout = new QHBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    QWidget::setLayout(layout);
}

Row::~Row() {
    for (auto *item : m_items) {
        delete item;
    }
}

auto Row::push(QWidgetItem *item) -> Row * {
    this->layout()->addItem(item);
    return this;
}

auto Row::push(QWidget *widget) -> Row * {
    widget->setParent(this);
    this->layout()->addWidget(widget);
    m_items.append(new Item(widget));
    return this;
}

auto Row::push(std::optional<QWidget *> opt_widget) -> Row * {
    if (opt_widget.has_value()) {
        this->push(opt_widget.value());
        m_items.append(new Item(opt_widget.value()));
    }
    return this;
}

auto Row::push(QLayout *layout) -> Row * {
    layout->setParent(this);
    this->layout()->addLayout(layout);
    return this;
}

auto Row::widget() -> QWidget * {
    return this;
}

auto Row::pushSpacer() -> Row * {
    this->layout()->addStretch();
    m_items.append(new Item(Orientation::Horizontal, std::nullopt));
    return this;
}

auto Row::pushSpacer(int width) -> Row * {
    this->layout()->addSpacing(width);
    m_items.append(new Item(Orientation::Horizontal, width));
    return this;
}

auto Row::pushStretch(int factor) -> Row * {
    this->layout()->addStretch(factor);
    return this;
}

// NOTE:we need to override this method in order to prevent consumer to change layout type
void Row::setLayout(QLayout *layout) { // NOLINT
    qCritical() << "Layout of a Row cannot be changed";
}

// we need override the method in order to return a QHBoxLayout* because
// addLayout() is is not implemented on QLayout
auto Row::layout() -> QHBoxLayout * {
    QLayout *currentLayout = QWidget::layout();
    auto *hboxLayout = dynamic_cast<QHBoxLayout *>(currentLayout);
    if (hboxLayout == nullptr) {
        qDebug() << "layout is not of type QHBoxLayout!";
    }
    return hboxLayout;
}

void Row::clear() {
    auto *layout = new QHBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    delete this->layout();
    for (auto *item : m_items) {
        if (item->isWidget()) {
            delete item->widget();
        }
        delete item;
    }
    m_items.clear();
    QWidget::setLayout(layout);
}

auto Row::toItemList() -> QList<Item *> {
    auto list = QList<Item *>();
    for (auto *item : m_items) {
        list.append(item);
    }
    return list;
}

auto Row::merge(const QList<Item *> &items) -> Row * {
    for (auto *item : items) {
        if (item->isWidget()) {
            push(item->widget());
        } else if (item->isSpacer()) {
            if (item->spacer()->orientation() == Orientation::Horizontal) {
                auto value = item->spacer().value().value();
                if (value.has_value()) {
                    pushSpacer(value.value());
                } else {
                    pushSpacer();
                }
            }
        }
    }
    return this;
}

auto Row::merge(Row *row) -> Row * {
    merge(row->toItemList());
    return this;
}

auto Row::merge(Column *column) -> Row * {
    merge(column->toItemList());
    return this;
}
} // namespace qontrol
