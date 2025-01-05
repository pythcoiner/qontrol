#include "Controller.h"
#include "Screen.h"
#include "common.h"
#include <QtWebSockets/qwebsocket.h>
#include <optional>
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

void Controller::init(Controller *controller) {
    if (s_instance != nullptr) {
        qFatal() << "Controller have already been initiated!";
    }
    s_instance = controller;
}

auto Controller::isInit() -> bool {
    return s_instance != nullptr;
}

auto Controller::get() -> Controller* {
    if (s_instance == nullptr) {
        qFatal() << "Controller have not been initiated!";
    }
    return s_instance;
}

auto Controller::screen(const QString &screen) -> std::optional<Screen*> {
    auto *panel = m_panels.value(screen);
    if (panel != nullptr) {
        return std::optional(panel->screen());
    }
    return std::nullopt;
}

void Controller::loadPanels() {}

Controller::~Controller() = default;

void Controller::loadPanel(const QString &name) {
    qDebug() << "loadPanel(" << name <<")";
    auto *panel = m_panels.value(name);

    if (panel == nullptr) qFatal() << "Controller::loadPanel(): Panel with name " 
        << name << " does not exists!";
    if (m_current_panel != nullptr) {
        // setCentralWidget() delete the replaced widget, so we had to take it first
        auto *previousScreen = dynamic_cast<Screen*>(m_window->takePanel());
        if (previousScreen != nullptr) {
            m_current_panel->setScreen(previousScreen);
        }
        m_current_panel->widget()->setVisible(false);
    }
    m_current_panel = panel;
    m_current_panel->connectScreen();
    m_window->loadPanel(m_current_panel->widget());
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
    emit this->closed();
}

auto Controller::window() -> QWidget* {
    return Controller::get()->getWindow();
} 

auto Controller::getWindow() -> QWidget* {
    return m_window;
}

void Controller::start(Window *window) {
    m_window = window;
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
