#pragma once

#include "Screen.h"
#include "./common.h"
#include <QPointer>
#include <QWidget>
#include <qjsonobject.h>
#include <qobject.h>

class Controller;

class Panel : public QObject {
    Q_OBJECT
public:
    Panel(Screen *widget, const char *name);
    ~Panel() override;
    auto widget() -> Screen*;
    void setScreen(Screen* screen);
    auto name() -> const char*;
    void updateState(Json update);
    void connectScreen();
    void disconnectScreen();
    auto screen() -> Screen*;

private:
    QPointer<Screen> m_screen;
    const char* m_name;
};
