#pragma once

#include "Screen.h"
#include <QObject>
#include <QPointer>

namespace qontrol {

class Controller;

class Panel : public QObject {
    Q_OBJECT
public:
    Panel(Screen *widget, const char *name);
    ~Panel() override;
    auto widget() -> Screen *;
    void setScreen(Screen *screen);
    auto name() -> const char *;
    auto screen() -> Screen *;

private:
    QPointer<Screen> m_screen;
    const char *m_name;
};

} // namespace qontrol
