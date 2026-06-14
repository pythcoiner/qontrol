#pragma once

#include "Item.h"
#include <QLoggingCategory>
#include <QVBoxLayout>
#include <QWidget>

namespace qontrol {

class Row;

class Column : public QWidget {
    Q_OBJECT
public:
    explicit Column(QWidget *parent = nullptr);
    ~Column() override;

    auto layout() -> QVBoxLayout *;
    auto push(QWidgetItem *item) -> Column *;
    auto push(QWidget *widget) -> Column *;
    // Push a widget with a stretch factor and optional cross-axis alignment
    // (e.g. Qt::AlignHCenter / AlignRight). To align without a stretch, pass
    // stretch 0 or use pushAligned().
    auto push(QWidget *widget, int stretch, Qt::Alignment alignment = Qt::Alignment()) -> Column *;
    // Push a widget aligned on the cross axis (stretch 0). Avoids the
    // int-vs-Qt::Alignment overload ambiguity of a two-argument push.
    auto pushAligned(QWidget *widget, Qt::Alignment alignment) -> Column *;
    auto push(std::optional<QWidget *> opt_widget) -> Column *;
    auto push(QLayout *layout) -> Column *;
    auto push(QLayout *layout, int stretch) -> Column *;
    // Set the alignment of an already-pushed widget.
    auto align(QWidget *widget, Qt::Alignment alignment) -> Column *;
    // Fluent layout spacing. Unlike pushSpacer(), layout spacing collapses
    // around hidden widgets, so prefer it for inter-item gaps that should
    // disappear when a neighbor is hidden.
    auto spacing(int px) -> Column *;
    auto margins(int px) -> Column *;
    auto margins(int left, int top, int right, int bottom) -> Column *;
    // Host this Column inside `target` (sets target's layout). Lets a plain
    // QWidget use the fluent builder without a bare layout in consumer code.
    auto into(QWidget *target) -> Column *;
    auto widget() -> QWidget *;
    auto pushSpacer() -> Column *;
    auto pushSpacer(int height) -> Column *;
    auto pushStretch(int factor) -> Column *;
    // Positional insert.
    auto insert(int index, QWidget *widget, int stretch = 0,
                Qt::Alignment alignment = Qt::Alignment()) -> Column *;
    auto insertSpacer(int index) -> Column *;
    auto insertSpacer(int index, int height) -> Column *;
    // Remove a widget: remove() deletes it (deleteLater); take() detaches and
    // returns it without deleting; replace() swaps it preserving stretch/alignment.
    auto remove(QWidget *widget) -> Column *;
    auto take(QWidget *widget) -> QWidget *;
    auto replace(QWidget *old_widget, QWidget *new_widget) -> Column *;
    auto merge(const QList<Item *> &items) -> Column *;
    auto merge(Column *column) -> Column *;
    auto merge(Row *row) -> Column *;
    void setLayout(QLayout *layout);
    auto toItemList() -> QList<Item *>;
    void clear();

private:
    QList<Item *> m_items;
};

} // namespace qontrol
