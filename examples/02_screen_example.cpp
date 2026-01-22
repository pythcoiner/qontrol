/**
 * Example 02: Screen Pattern
 *
 * Demonstrates creating a Screen subclass with init(), view(), and doConnect().
 */

#include <Column.h>
#include <Controller.h>
#include <Row.h>
#include <Screen.h>
#include <Window.h>
#include <QApplication>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

// Custom screen implementation
class LoginScreen : public qontrol::Screen {
    Q_OBJECT

public:
    LoginScreen() : Screen() {
        init();
        view();
        doConnect();
    }

protected:
    void init() override {
        m_title = new QLabel("Login");
        m_title->setStyleSheet("font-size: 24px; font-weight: bold;");

        m_username_label = new QLabel("Username:");
        m_username_input = new QLineEdit();
        m_username_input->setPlaceholderText("Enter username");

        m_password_label = new QLabel("Password:");
        m_password_input = new QLineEdit();
        m_password_input->setEchoMode(QLineEdit::Password);
        m_password_input->setPlaceholderText("Enter password");

        m_login_btn = new QPushButton("Login");
        m_cancel_btn = new QPushButton("Cancel");
        m_status = new QLabel("");
    }

    void view() override {
        auto *col = (new qontrol::Column())
            ->pushSpacer()
            ->push(m_title)
            ->pushSpacer(30)
            ->push((new qontrol::Row())
                ->push(m_username_label)
                ->pushSpacer(10)
                ->push(m_username_input))
            ->pushSpacer(10)
            ->push((new qontrol::Row())
                ->push(m_password_label)
                ->pushSpacer(10)
                ->push(m_password_input))
            ->pushSpacer(20)
            ->push((new qontrol::Row())
                ->pushSpacer()
                ->push(m_cancel_btn)
                ->pushSpacer(10)
                ->push(m_login_btn)
                ->pushSpacer())
            ->pushSpacer(10)
            ->push(m_status)
            ->pushSpacer();

        auto *row = (new qontrol::Row())
            ->pushSpacer()
            ->push(col)
            ->pushSpacer();

        this->setLayout(row->layout());
    }

    void doConnect() override {
        connect(m_login_btn, &QPushButton::clicked,
                this, &LoginScreen::onLogin, qontrol::UNIQUE);
        connect(m_cancel_btn, &QPushButton::clicked,
                this, &LoginScreen::onCancel, qontrol::UNIQUE);
    }

public slots:
    void onUnload() override {
        // Clean up when navigating away
        m_username_input->clear();
        m_password_input->clear();
        m_status->clear();
    }

private slots:
    void onLogin() {
        auto username = m_username_input->text();
        auto password = m_password_input->text();

        if (username.isEmpty() || password.isEmpty()) {
            m_status->setText("Please fill in all fields");
            m_status->setStyleSheet("color: red;");
            return;
        }

        m_status->setText("Login successful!");
        m_status->setStyleSheet("color: green;");
    }

    void onCancel() {
        m_username_input->clear();
        m_password_input->clear();
        m_status->clear();
    }

private:
    QLabel *m_title;
    QLabel *m_username_label;
    QLineEdit *m_username_input;
    QLabel *m_password_label;
    QLineEdit *m_password_input;
    QPushButton *m_login_btn;
    QPushButton *m_cancel_btn;
    QLabel *m_status;
};

// Simple controller
class AppController : public qontrol::Controller {
    Q_OBJECT

public:
    static void init() {
        if (!Controller::isInit()) {
            Controller::init(new AppController());
        }
    }

    void loadPanels() override {
        insertPanel(new qontrol::Panel(new LoginScreen(), "login"));
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    AppController::init();

    auto *window = new qontrol::Window(nullptr);
    window->setWindowTitle("Screen Example");
    window->setFixedSize(400, 350);

    auto *ctrl = qontrol::Controller::get();
    ctrl->start(window);
    ctrl->loadPanel("login");

    window->show();

    return app.exec();
}

#include "02_screen_example.moc"
