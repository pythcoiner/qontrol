#include "Controller.h"
#include "Screen.h"
#include <optional>
#include <qdialog.h>
#include <qlogging.h>
#include <qobject.h>
#include <qpointer.h>
#include <qqueue.h>
#include <qscopedpointer.h>
#include <qtmetamacros.h>
#include <qwidget.h>
#include <unistd.h>
#include <utility>

namespace qontrol {

Controller::Controller() : QObject(nullptr), m_window(nullptr) {
}

void Controller::init(Controller *controller) {
    if (s_instance != nullptr) {
        qDebug() << "Controller have already been initiated!";
    }
    s_instance = controller;
}

auto Controller::isInit() -> bool {
    return s_instance != nullptr;
}

auto Controller::get() -> Controller * {
    if (s_instance == nullptr) {
        qDebug() << "Controller have not been initiated!";
    }
    return s_instance;
}

auto Controller::screen(const QString &screen) -> std::optional<Screen *> {
    auto *panel = m_panels.value(screen);
    if (panel != nullptr) {
        return std::optional(panel->screen());
    }
    return std::nullopt;
}

void Controller::loadPanels() {
}

Controller::~Controller() = default;

void Controller::loadPanel(const QString &name) {
    auto *panel = m_panels.value(name);

    if (panel == nullptr)
        qDebug() << "Controller::loadPanel(): Panel with name " << name << " does not exists!";
    if (m_current_panel != nullptr) {
        // setCentralWidget() delete the replaced widget, so we had to take it first
        auto *previousScreen = dynamic_cast<Screen *>(m_window->takePanel());
        if (previousScreen != nullptr) {
            m_current_panel->setScreen(previousScreen);
            previousScreen->onUnload();
        }
        m_current_panel->widget()->setVisible(false);
    }
    m_current_panel = panel;
    m_window->loadPanel(m_current_panel->widget());
    m_current_panel->widget()->setVisible(true);
}

void Controller::insertPanel(Panel *panel) {
    m_panels.insert(panel->name(), panel);
}

void Controller::onClose() {
    emit this->closed();
}

auto Controller::window() -> QWidget * {
    return Controller::get()->getWindow();
}

void Controller::execModal(QDialog *modal) {
    modal->exec();
    delete modal;
}

auto Controller::getWindow() -> QWidget * {
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

} // namespace qontrol
