#include "Controller.h"
#include "MainWindow.h"
#include "Screen.h"
#include "common.h"
#include <QtWebSockets/qwebsocket.h>
#include <qassert.h>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qlogging.h>
#include <qobject.h>
#include <qpointer.h>
#include <qqueue.h>
#include <qscopedpointer.h>
#include <qtmetamacros.h>
#include <qwidget.h>
#include <unistd.h>
#include <utility>

Controller::Controller() : 
    QObject(nullptr) ,
    m_window(nullptr)
{}

void Controller::init() {
    if (s_instance != nullptr) {
        qFatal() << "Controller have already been initiated!";
    }
    s_instance = new Controller();
}
auto Controller::get() -> Controller* {
    if (s_instance == nullptr) {
        qFatal() << "Controller have not been initiated!";
    }
    return s_instance;
}

void Controller::loadPanels() {}

Controller::~Controller() = default;

void Controller::loadPanel(const QString &name) {
    qDebug() << "loadPanel(" << name <<")";
    if (m_panels.size() < 1) this->loadPanels();
    auto *panel = m_panels.value(name);

    if (panel == nullptr) qFatal() << "Controller::loadPanel(): Panel with name " << name << " does not exists!";
    if (m_current_panel != nullptr) {
        // setCentralWidget() delete the replaced widget, so we had to take it first
        auto *previousScreen = dynamic_cast<Screen*>(m_window->takeCentralWidget());
        if (previousScreen != nullptr) {
            m_current_panel->setScreen(previousScreen);
        }
        m_current_panel->widget()->setVisible(false);
    }
    m_current_panel = panel;
    m_current_panel->connectScreen();
    m_window->setCentralWidget(m_current_panel->widget());
    m_current_panel->widget()->setVisible(true);
}

void Controller::insertPanel(Panel *panel) {
    m_panels.insert(panel->name(), panel);
}

void Controller::updateState(Json state) {
    m_current_panel->updateState(std::move(state));
}

void Controller::sendUpdate(SharedJson payload) {} // NOLINT

void Controller::receiveUpdate(SharedJson payload) {} // NOLINT

auto jsonFromString(const QString &msg) -> Json {
    QJsonDocument payload = QJsonDocument::fromJson(msg.toUtf8());
    if (payload.isNull() || !payload.isObject()) {
        qCritical() << "invalid json payload:" << msg;
        return newJson();
    }
    Json json = newJson(payload.object());
    return std::move(json);
}

void Controller::onClose() {
    connect(this, &Controller::closed, m_window, &MainWindow::onClose);
    emit this->closed();
}

auto Controller::window() -> QWidget* {
    return Controller::get()->getWindow();
} 

auto Controller::getWindow() -> QWidget* {
    return m_window;
}

void Controller::start(MainWindow *window) {
    m_window = window;
    connect(m_window, &MainWindow::askClose, this, &Controller::onClose);
}

auto Controller::getEnum(const QString &name) -> std::optional<QList<QPair<QString, QString>>> {
    if (m_enums.contains(name)) {
        return std::optional(m_enums.value(name));
    }
    return std::nullopt;
}

auto Controller::enumExists(const QString &name) -> bool {
    return m_enums.contains(name);
}

auto Controller::isEnumsEmpty() -> bool {
    return m_enums.isEmpty();
}
