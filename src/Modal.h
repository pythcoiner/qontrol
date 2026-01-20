#pragma once

#include <QDialog>

namespace qontrol {

class Modal : public QDialog {
    Q_OBJECT
public:
    Modal();
    Modal(const QString &title, const QString &msg);
    void setMainWidget(QWidget *widget);

private:
    QWidget *m_main_widget = nullptr;
};

} // namespace qontrol
