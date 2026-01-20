#pragma once

#include "../Column.h"
#include <QList>
#include <QPushButton>

namespace qontrol::widgets {

class Collapsible : public Column {
    Q_OBJECT

public:
    explicit Collapsible(const QString &name, QWidget *parent = nullptr);
    ~Collapsible() override { delete m_items; }
    auto pushInner(QWidget *widget) -> Collapsible *;
    void setCollapsed(bool collapsed);

public slots:
    void onClicked();

protected:
private:
    QString m_title;
    QPushButton *m_bar = nullptr;
    Column *m_column = nullptr;
    QList<QWidget *> *m_items = nullptr;
    bool m_collapsed = true;
};

} // namespace qontrol::widgets
