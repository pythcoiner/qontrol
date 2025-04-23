#pragma once

#include "Item.h"
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <qboxlayout.h>
#include <qlayout.h>
#include <qlayoutitem.h>
#include <qlogging.h>
#include <qnamespace.h>

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
    auto push(std::optional<QWidget *> opt_widget) -> Row *;
    auto push(QLayout *layout) -> Row *;
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
