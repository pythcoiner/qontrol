#pragma once

#include <qdialog.h>
#include <qtmetamacros.h>

namespace qontrol {

class Modal : public QDialog {
    Q_OBJECT
public:
    Modal();
    Modal(const QString& msg, const QString& title);
    void setMainWidget(QWidget *widget);
private:
    QWidget *m_main_widget = nullptr;
};

} // namespace qontrol 
