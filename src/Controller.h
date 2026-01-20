#pragma once

#include "Window.h"
#include <Panel.h>
#include <QMainWindow>
#include <QObject>
#include <QPointer>
#include <QWidget>

namespace qontrol {

class Controller : public QObject {
    Q_OBJECT
public:
    Controller();
    Controller(const Controller &) = delete;
    auto operator=(const Controller &) -> Controller & = delete;
    ~Controller() override;

    static void init(Controller *controller);
    static auto isInit() -> bool;
    static auto get() -> Controller *;
    static auto window() -> QWidget *;
    static void execModal(QDialog *modal);

    auto getWindow() -> QWidget *;
    auto screen(const QString &screen) -> std::optional<Screen *>;

    void loadPanel(const QString &name);
    void insertPanel(Panel *panel);

    virtual void start(Window *window);
    virtual void loadPanels();

signals:
    void closed();

public slots:
    virtual void onClose();

private:
    QPointer<Panel> m_current_panel;
    QHash<QString, Panel *> m_panels;
    QPointer<Window> m_window;
    inline static Controller *s_instance = nullptr; // NOLINT(readability-identifier-naming)
};

} // namespace qontrol
