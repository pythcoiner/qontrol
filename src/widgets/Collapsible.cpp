#include "Collapsible.h"
#include "../Column.h"
#include <qlist.h>
#include <qpushbutton.h>

const QString PLUS = ":/icons/plus.png";
const QString MINUS = ":/icons/minus.png";

Collapsible::Collapsible(const QString &name, QWidget *parent) : Column(parent), m_title(name) {
    m_bar = new QPushButton(QIcon(PLUS),m_title);
    m_bar->setStyleSheet("text-align: left; padding-left: 30px; padding-top: 10px; padding-bottom: 10px;");
    m_bar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    m_column = new Column();
    m_items = new QList<QWidget*>();

    m_column->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    this->setCollapsed(false);
    connect(m_bar, &QPushButton::clicked, this, &Collapsible::onClicked);

    this->push(m_bar);
    this->push(m_column);
}

auto Collapsible::pushInner(QWidget *widget) -> Collapsible* {
    m_column->push(widget);
    m_items->push_back(widget);
    this->setCollapsed(m_collapsed);
    return this;
}

void Collapsible::onClicked() {
    this->setCollapsed(!m_collapsed);
}

void Collapsible::setCollapsed(bool collapsed) {
    m_collapsed = collapsed;
    if (!collapsed) {
        m_bar->setIcon(QIcon(PLUS));
        m_column->setVisible(false);
    } else {
        m_bar->setIcon(QIcon(MINUS));
        m_column->setVisible(true);
    }
}

