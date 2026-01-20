#include "Column.h"
#include "Item.h"
#include "Row.h"
#include <optional>

namespace qontrol {

Column::Column(QWidget *parent) : QWidget(parent) {
    auto *layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    QWidget::setLayout(layout);
}

Column::~Column() {
    for (auto *item : m_items) {
        delete item;
    }
}

auto Column::push(QWidgetItem *item) -> Column * {
    this->layout()->addItem(item);
    return this;
}

auto Column::push(QWidget *widget) -> Column * {
    widget->setParent(this);
    this->layout()->addWidget(widget);
    m_items.append(new Item(widget));
    return this;
}

auto Column::push(std::optional<QWidget *> opt_widget) -> Column * {
    if (opt_widget.has_value()) {
        this->push(opt_widget.value());
    }
    return this;
}

auto Column::push(QLayout *layout) -> Column * {
    this->layout()->addLayout(layout);
    // Set parent on all widgets in the layout so they get deleted with this Column
    for (int i = 0; i < layout->count(); ++i) {
        if (auto *widget = layout->itemAt(i)->widget()) {
            widget->setParent(this);
        }
    }
    return this;
}

auto Column::widget() -> QWidget * {
    return this;
}

auto Column::pushSpacer() -> Column * {
    this->layout()->addStretch();
    m_items.append(new Item(Orientation::Vertical, std::nullopt));
    return this;
}

auto Column::pushSpacer(int height) -> Column * {
    this->layout()->addSpacing(height);
    m_items.append(new Item(Orientation::Vertical, height));
    return this;
}

auto Column::pushStretch(int factor) -> Column * {
    this->layout()->addStretch(factor);
    return this;
}

// NOTE:we need to override this method in order to impeach consumer to change layout type
void Column::setLayout(QLayout *layout) { // NOLINT
    qCritical() << "Layout of a Column cannot be changed";
}

// we need override the method in order to return a QHBoxLayout* because
// addLayout() is is not implemented on QLayout
auto Column::layout() -> QVBoxLayout * {
    QLayout *currentLayout = QWidget::layout();
    auto *vboxLayout = dynamic_cast<QVBoxLayout *>(currentLayout);
    if (vboxLayout == nullptr) {
        qDebug() << "layout is not of type QVBoxLayout!";
    }
    return vboxLayout;
}

auto Column::merge(const QList<Item *> &items) -> Column * {
    for (auto *item : items) {
        if (item->isWidget()) {
            push(item->widget());
        } else if (item->isSpacer()) {
            if (item->spacer()->orientation() == Orientation::Vertical) {
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

void Column::clear() {
    auto *layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    delete this->layout();
    // Qt layouts don't own widgets, so deleting the layout doesn't delete
    // the widgets. We must delete them explicitly here.
    for (auto *item : m_items) {
        if (item->isWidget()) {
            delete item->widget();
        }
        delete item;
    }
    m_items.clear();
    QWidget::setLayout(layout);
}

auto Column::toItemList() -> QList<Item *> {
    return m_items;
}

auto Column::merge(Column *column) -> Column * {
    merge(column->toItemList());
    return this;
}

auto Column::merge(Row *row) -> Column * {
    merge(row->toItemList());
    return this;
}
} // namespace qontrol
