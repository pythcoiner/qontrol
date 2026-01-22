/**
 * Example 01: Basic Layout
 *
 * Demonstrates Row and Column containers with the chainable API.
 */

#include <Column.h>
#include <Row.h>
#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QWidget>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Create main widget
    auto *window = new QWidget();
    window->setWindowTitle("Basic Layout Example");
    window->setFixedSize(400, 300);

    // Build layout using chainable API
    auto *layout = (new qontrol::Column())
        ->pushSpacer()
        ->push(new QLabel("Welcome to Qontrol"))
        ->pushSpacer(20)
        ->push((new qontrol::Row())
            ->push(new QLabel("Name:"))
            ->pushSpacer(10)
            ->push(new QLineEdit()))
        ->pushSpacer(10)
        ->push((new qontrol::Row())
            ->push(new QLabel("Email:"))
            ->pushSpacer(10)
            ->push(new QLineEdit()))
        ->pushSpacer(20)
        ->push((new qontrol::Row())
            ->pushSpacer()
            ->push(new QPushButton("Cancel"))
            ->pushSpacer(10)
            ->push(new QPushButton("Submit"))
            ->pushSpacer())
        ->pushSpacer();

    // Set layout on window
    window->setLayout(layout->layout());
    window->show();

    return app.exec();
}
