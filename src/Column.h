#pragma once

#include "Item.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <qboxlayout.h>
#include <qlayout.h>
#include <qlogging.h>
#include <qnamespace.h>

namespace qontrol {

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
    auto merge(const QList<Item *> &items) -> Column*;
    void setLayout(QLayout *layout);
    auto toItemList() -> QList<Item *>;
    void clear();

private:
    QList<Item *> m_items;
};

} // namespace qontrol
