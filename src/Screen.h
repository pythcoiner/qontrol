#pragma once

#include "widgets/Widget.h"
#include <QWidget>

namespace qontrol {

class Screen : public QWidget, public qontrol::widgets::Widget {
    Q_OBJECT
public:
    explicit Screen();
    ~Screen() override = default;

public slots:
    virtual void onUnload() {};

protected:
    [[nodiscard]] auto className() const -> const char *;

    // initialisation of contained widgets, called only once
    virtual void init() = 0;
    // layout of the view, called only once
    virtual void view() = 0;
    // connection that should be done on screen loading
    virtual void doConnect() = 0;
};

} // namespace qontrol
