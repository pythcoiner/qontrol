#include "Panel.h"
#include "Controller.h"
#include "Screen.h"
#include <QLoggingCategory>

namespace qontrol {

Panel::Panel(Screen *widget, const char *name) {
    if (widget == nullptr) {
        qDebug() << "Panel::Panel(widget): widget should not be a nullptr!";
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

auto Panel::screen() -> Screen * {
    return this->m_screen;
}

} // namespace qontrol
