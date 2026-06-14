#pragma once

#include "Item.h"
#include <QHBoxLayout>
#include <QLayoutItem>
#include <QLoggingCategory>
#include <QWidget>

namespace qontrol {
class Column;

class Row : public QWidget {
    Q_OBJECT

public:
    explicit Row(QWidget *parent = nullptr);
    ~Row() override;
    auto layout() -> QHBoxLayout *;
    auto push(QWidgetItem *item) -> Row *;
    auto push(QWidget *widget) -> Row *;
    // Push a widget with a stretch factor and optional cross-axis alignment
    // (e.g. Qt::AlignVCenter / AlignBottom). To align without a stretch, pass
    // stretch 0 or use pushAligned().
    auto push(QWidget *widget, int stretch, Qt::Alignment alignment = Qt::Alignment()) -> Row *;
    // Push a widget aligned on the cross axis (stretch 0). Avoids the
    // int-vs-Qt::Alignment overload ambiguity of a two-argument push.
    auto pushAligned(QWidget *widget, Qt::Alignment alignment) -> Row *;
    auto push(std::optional<QWidget *> opt_widget) -> Row *;
    auto push(QLayout *layout) -> Row *;
    auto push(QLayout *layout, int stretch) -> Row *;
    // Set the alignment of an already-pushed widget.
    auto align(QWidget *widget, Qt::Alignment alignment) -> Row *;
    // Fluent layout spacing. Unlike pushSpacer(), layout spacing collapses
    // around hidden widgets, so prefer it for inter-item gaps that should
    // disappear when a neighbor is hidden.
    auto spacing(int px) -> Row *;
    auto margins(int px) -> Row *;
    auto margins(int left, int top, int right, int bottom) -> Row *;
    // Host this Row inside `target` (sets target's layout). Lets a plain QWidget
    // use the fluent builder without a bare layout in consumer code.
    auto into(QWidget *target) -> Row *;
    auto widget() -> QWidget *;
    auto pushSpacer() -> Row *;
    auto pushSpacer(int width) -> Row *;
    auto pushStretch(int factor) -> Row *;
    auto merge(const QList<Item *> &items) -> Row *;
    auto merge(Column *column) -> Row *;
    auto merge(Row *row) -> Row *;
    void setLayout(QLayout *layout);
    auto toItemList() -> QList<Item *>;
    void clear();

private:
    QList<Item *> m_items;
};

} // namespace qontrol
