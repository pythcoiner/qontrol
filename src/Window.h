#pragma once

#include <QMainWindow>

namespace qontrol {

class Window : public QMainWindow {
    Q_OBJECT
public:
    Window(QWidget *parent);
    virtual void loadPanel(QWidget *widget);
    virtual auto takePanel() -> QWidget *;
};

} // namespace qontrol
