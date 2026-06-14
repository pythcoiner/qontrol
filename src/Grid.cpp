#include "Grid.h"

#include <QHBoxLayout>

namespace qontrol {

Grid::Grid(QWidget *parent) : QWidget(parent) {
    auto *layout = new QGridLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    QWidget::setLayout(layout);
}

auto Grid::layout() -> QGridLayout * {
    QLayout *currentLayout = QWidget::layout();
    auto *gridLayout = dynamic_cast<QGridLayout *>(currentLayout);
    if (gridLayout == nullptr) {
        qDebug() << "layout is not of type QGridLayout!";
    }
    return gridLayout;
}

auto Grid::push(QWidget *widget, int row, int col, int row_span, int col_span,
                Qt::Alignment alignment) -> Grid * {
    widget->setParent(this);
    this->layout()->addWidget(widget, row, col, row_span, col_span, alignment);
    return this;
}

auto Grid::spacing(int px) -> Grid * {
    this->layout()->setSpacing(px);
    return this;
}

auto Grid::spacing(int horizontal, int vertical) -> Grid * {
    this->layout()->setHorizontalSpacing(horizontal);
    this->layout()->setVerticalSpacing(vertical);
    return this;
}

auto Grid::margins(int px) -> Grid * {
    this->layout()->setContentsMargins(px, px, px, px);
    return this;
}

auto Grid::margins(int left, int top, int right, int bottom) -> Grid * {
    this->layout()->setContentsMargins(left, top, right, bottom);
    return this;
}

auto Grid::columnStretch(int column, int stretch) -> Grid * {
    this->layout()->setColumnStretch(column, stretch);
    return this;
}

auto Grid::rowStretch(int row, int stretch) -> Grid * {
    this->layout()->setRowStretch(row, stretch);
    return this;
}

auto Grid::into(QWidget *target) -> Grid * {
    auto *host = new QHBoxLayout(target);
    host->setContentsMargins(0, 0, 0, 0);
    host->setSpacing(0);
    host->addWidget(this);
    return this;
}

auto Grid::widget() -> QWidget * {
    return this;
}

// NOTE: override to prevent consumers from changing the layout type.
void Grid::setLayout(QLayout *layout) { // NOLINT
    qCritical() << "Layout of a Grid cannot be changed";
}

} // namespace qontrol
