#include "Window.h"
#include <qmainwindow.h>

namespace qontrol {

Window::Window(QWidget *parent) : QMainWindow(parent) {};

void Window::loadPanel(QWidget *widget) {
    this->setCentralWidget(widget);
}
auto Window::takePanel() -> QWidget* {
    return this->takeCentralWidget();
}

} // namespace qontrol
