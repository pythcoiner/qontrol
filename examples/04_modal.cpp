/**
 * Example 04: Modal Dialogs
 *
 * Demonstrates using Modal for popup dialogs.
 */

#include <Column.h>
#include <Controller.h>
#include <Modal.h>
#include <Row.h>
#include <QApplication>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

auto main(int argc, char *argv[]) -> int {
    QApplication app(argc, argv);

    auto *window = new QWidget();
    window->setWindowTitle("Modal Example");
    window->setFixedSize(300, 200);

    // Button to show simple message modal
    auto *msgBtn = new QPushButton("Show Message");
    QObject::connect(msgBtn, &QPushButton::clicked, [window]() {
        auto *modal = new qontrol::Modal("Information", "This is a simple message dialog.");
        modal->exec();
        delete modal;
    });

    // Button to show custom modal
    auto *customBtn = new QPushButton("Show Custom Dialog");
    QObject::connect(customBtn, &QPushButton::clicked, [window]() {
        auto *modal = new qontrol::Modal();
        modal->setWindowTitle("Enter Details");

        auto *content = (new qontrol::Column())
            ->pushSpacer()
            ->push(new QLabel("Enter your name:"))
            ->pushSpacer(10)
            ->push(new QLineEdit())
            ->pushSpacer(20)
            ->push((new qontrol::Row())
                ->pushSpacer()
                ->push(new QPushButton("OK"))
                ->pushSpacer());

        modal->setMainWidget(content);
        modal->exec();
        delete modal;
    });

    // Build layout
    auto *layout = (new qontrol::Column())
        ->pushSpacer()
        ->push(new QLabel("Modal Dialog Examples"))
        ->pushSpacer(20)
        ->push((new qontrol::Row())
            ->pushSpacer()
            ->push(msgBtn)
            ->pushSpacer())
        ->pushSpacer(10)
        ->push((new qontrol::Row())
            ->pushSpacer()
            ->push(customBtn)
            ->pushSpacer())
        ->pushSpacer();

    window->setLayout(layout->layout());
    window->show();

    return QApplication::exec();
}
