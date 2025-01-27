#pragma once

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

class Row : public QWidget {
    Q_OBJECT

public:
    explicit Row(QWidget *parent = nullptr);
    auto layout() -> QHBoxLayout*;
    auto push(QWidgetItem *item) -> Row*;
    auto push(QWidget *widget) -> Row*;
    auto push(std::optional<QWidget*> opt_widget) -> Row*;
    auto push(QLayout *layout) -> Row*;
    auto widget() -> QWidget*;
    auto pushSpacer() -> Row*;
    auto pushSpacer(int width) -> Row*;
    auto pushStretch(int factor) -> Row*;
    void setLayout(QLayout *layout);
    void clear();


private:

};

} // namespace qontrol
