#pragma once

#include <Panel.h>
#include <QObject>
#include <QWidget>

class MainWindow;

class Controller : public QObject {
    Q_OBJECT
public:
    Controller();
    Controller(const Controller&) = delete;
    auto operator=(const Controller&) -> Controller& = delete;
    ~Controller() override;

    static void init();
    static auto get() -> Controller*;
    static auto window() -> QWidget*;

    auto getWindow() -> QWidget*;


    auto getEnum(const QString &name) -> std::optional<QList<QPair<QString, QString>>>;
    auto enumExists(const QString &name) -> bool;
    auto isEnumsEmpty() -> bool;

    void loadPanel(const QString &name);
    void insertPanel(Panel *panel);
    void updateState(Json state);

    virtual void start(MainWindow *window);
    virtual void loadPanels();

signals:
    void update(SharedJson state);
    void externalUpdate(SharedJson update);
    void closed();

public slots:
    void sendUpdate(SharedJson payload);
    void receiveUpdate(SharedJson payload);
    void onClose();

private:

    QPointer<Panel> m_current_panel;
    QHash<QString, Panel*> m_panels;
    MainWindow* m_window;
    QHash<QString, QList<QPair<QString, QString>>> m_enums;
    inline static Controller *s_instance = nullptr ; //NOLINT(readability-identifier-naming)
};
