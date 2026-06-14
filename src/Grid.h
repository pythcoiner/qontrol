#pragma once

#include <QGridLayout>
#include <QLoggingCategory>
#include <QWidget>

namespace qontrol {

// A fluent wrapper over QGridLayout for two-dimensional layouts (no Row/Column
// equivalent). Items are placed by (row, column) with optional spans/alignment.
class Grid : public QWidget {
    Q_OBJECT

public:
    explicit Grid(QWidget *parent = nullptr);

    auto layout() -> QGridLayout *;
    auto push(QWidget *widget, int row, int col, int row_span = 1, int col_span = 1,
              Qt::Alignment alignment = Qt::Alignment()) -> Grid *;
    auto spacing(int px) -> Grid *;
    auto spacing(int horizontal, int vertical) -> Grid *;
    auto margins(int px) -> Grid *;
    auto margins(int left, int top, int right, int bottom) -> Grid *;
    auto columnStretch(int column, int stretch) -> Grid *;
    auto rowStretch(int row, int stretch) -> Grid *;
    // Host this Grid inside `target` (sets target's layout).
    auto into(QWidget *target) -> Grid *;
    auto widget() -> QWidget *;
    void setLayout(QLayout *layout);
};

} // namespace qontrol
