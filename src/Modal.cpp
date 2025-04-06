#include "Modal.h"
#include "Column.h"
#include "Row.h"

#include <Qontrol>
#include <qboxlayout.h>
#include <qdialog.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qstringliteral.h>
#include <qwidget.h>

namespace qontrol {

Modal::Modal() {
    this->setModal(true);
    this->setLayout(new QHBoxLayout);

}

Modal::Modal(const QString& title, const QString& msg) {
    this->setModal(true);
    this->setWindowTitle(title);
    this->setLayout(new QHBoxLayout);
    this->setFixedSize(300,160);
    auto *col = new qontrol::Column();
    auto *label = new QLabel(msg);
    label->setWordWrap(true);
    auto  *btn = new QPushButton("Close");
    connect(btn, &QPushButton::clicked, this, &QDialog::close);
    auto *btnRow = (new qontrol::Row)
        ->pushSpacer()
        ->push(btn)
        ->pushSpacer()
        ;

    auto *labelRow = (new qontrol::Row)
        ->pushSpacer()
        ->push(label)
        ->pushSpacer()
        ;
    col->pushSpacer()
        ->push(labelRow)
        ->pushSpacer()
        ->push(btnRow)
        ->pushSpacer(10)
        ;
    this->setMainWidget(col);
}

void Modal::setMainWidget(QWidget *widget) {
    widget->setParent(this);
    this->layout()->addWidget(widget);
}

} // namespace modal
