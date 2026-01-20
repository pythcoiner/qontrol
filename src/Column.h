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
    auto push(std::optional<QWidget *> opt_widget) -> Column *;
    auto push(QLayout *layout) -> Column *;
    auto widget() -> QWidget *;
    auto pushSpacer() -> Column *;
    auto pushSpacer(int height) -> Column *;
    auto pushStretch(int factor) -> Column *;
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
