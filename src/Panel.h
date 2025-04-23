#pragma once

#include "./common.h"
#include "Screen.h"
#include <QPointer>
#include <QWidget>
#include <qjsonobject.h>
#include <qobject.h>

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
    void updateState(Json update);
    void connectScreen();
    void disconnectScreen();
    auto screen() -> Screen *;

private:
    QPointer<Screen> m_screen;
    const char *m_name;
};

} // namespace qontrol
