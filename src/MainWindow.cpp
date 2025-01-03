#include "MainWindow.h"
#include "Controller.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    Controller::init();
    Controller::get()->start(this);
    this-> m_central_widget = new QWidget(this);
    this->setCentralWidget(m_central_widget);
}

MainWindow::~MainWindow() = default;

void MainWindow::loadLayout(QLayout *layout) {
    m_central_widget->setLayout(layout);
}

auto MainWindow::centralWidget() -> QWidget* {
    return m_central_widget;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    emit this->askClose();
    if (m_allow_close) {
        event->accept();
    }
    else {
        event->ignore();
    }
}

void MainWindow::onClose() {
    m_allow_close = true;
}

