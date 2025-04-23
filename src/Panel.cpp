#include "Panel.h"
#include "Controller.h"
#include "Screen.h"
#include "common.h"
#include <qlogging.h>
#include <qnamespace.h>

namespace qontrol {

Panel::Panel(Screen *widget, const char *name) {
    if (widget == nullptr) {
        qFatal() << "Panel::Panel(widget): widget should not be a nullptr!";
    }
    m_name = name;
    m_screen = widget;
    m_screen->setVisible(false);
}

Panel::~Panel() = default;

auto Panel::widget() -> Screen * {
    return m_screen;
}

void Panel::setScreen(Screen *screen) {
    m_screen = screen;
}

auto Panel::name() -> const char * {
    return m_name;
}

void Panel::updateState(Json update) {
    auto *widget = this->widget();
    SharedJson ptr = std::move(update);
    widget->onUpdate(ptr);
}

void Panel::connectScreen() {
    auto *controller = Controller::get();
    if (controller == nullptr) {
        qFatal() << "Panel.connectScreen() pointer to controller is a nullptr!";
    } else if (m_screen == nullptr) {
        qFatal() << "Panel.connectScreen() pointer to m_screen is a nullptr!";
    }
    connect(m_screen, &Screen::update, controller, &Controller::sendUpdate, UNIQUE);
    connect(controller, &Controller::update, m_screen, &Screen::onUpdate, UNIQUE);
}

auto Panel::screen() -> Screen * {
    return this->m_screen;
}

} // namespace qontrol
