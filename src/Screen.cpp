#include "Screen.h"
#include "Controller.h"

namespace qontrol {

Screen::Screen() {
    this->setParent(Controller::window());
}

auto Screen::className() const -> const char * {
    return this->metaObject()->className();
}

} // namespace qontrol
