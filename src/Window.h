#pragma once

#include <QMainWindow>
#include <qmainwindow.h>
#include <qtmetamacros.h>

namespace qontrol {

class Window : public QMainWindow {
    Q_OBJECT
public:
    Window(QWidget *parent);
    virtual void loadPanel(QWidget *widget);
    virtual auto takePanel() -> QWidget *;
};

} // namespace qontrol
