#include "Window.h"
#include <qmainwindow.h>

Window::Window(QWidget *parent) : QMainWindow(parent) {};

void Window::loadPanel(QWidget *widget) {
    this->setCentralWidget(widget);
}
auto Window::takePanel() -> QWidget* {
    return this->takeCentralWidget();
}
