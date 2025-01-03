#include "ToggleSwitch.h"
#include <qlogging.h>

ToggleSwitch::ToggleSwitch(QWidget *parent) :
    QCheckBox(parent),
    m_margin(3),
    m_body_brush(Qt::lightGray)
{
    this->setChecked(false);
    this->setBrush(QColor(Qt::blue));
    this->setFixedHeight(25);
    this->setFixedWidth(25 * 15 / 10);
}

ToggleSwitch::ToggleSwitch(const QBrush &brush, QWidget *parent) :
    QCheckBox(parent),
    m_margin(3),
    m_body_brush(Qt::lightGray)
{
    this->setChecked(false);
    this->setBrush(brush);
    this->setFixedHeight(25);
    this->setFixedWidth(25 * 15 / 10);
}


void ToggleSwitch::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setRenderHint(QPainter::Antialiasing, true);
    int travel = this->width() - this->height();
    int outerRadius = this->height() / 2;
    int handleRadius = (this->height() - (2 * m_margin)) / 2;

    if (this->isEnabled()) {
        painter.setBrush(this->isChecked() ? this->brush() : Qt::black);
        painter.setOpacity(this->isChecked() ? 0.3 : 0.40);
    } else {
        painter.setBrush(Qt::black);
        painter.setOpacity(0.20);
    }

    painter.drawRoundedRect(
        QRect(
            0, 
            0, 
            this->width() ,
            this->height()
        ), 
        outerRadius,
        outerRadius
    );

    painter.setOpacity(1.0);
    if (this->isEnabled()) {
        painter.setBrush(m_body_brush);
    } else {
        painter.setBrush(QColor("#BDBDBD"));
    }

    auto handle = QRect(
            m_margin,
            m_margin,
            this->height() - (2 * m_margin),
            this->height() - (2 * m_margin)
        );

    // if toggled we move to the right
    if (this->isChecked()) handle.moveTo(m_margin + travel, m_margin);
    painter.drawRoundedRect(
        handle,
        handleRadius,
        handleRadius
    );
}

void ToggleSwitch::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() & (Qt::LeftButton != 0U)) // NOLINT
    {
        this->setChecked(!this->isChecked());
    }
}

void ToggleSwitch::resizeEvent(QResizeEvent *e) {
    auto size = e->size();
    int minWidth = size.height() * 13 / 10;
    int maxWidth = size.width() * 15 / 10;
    if (size.width() < minWidth || size.width() > maxWidth) {
        // if height unchanged, adjust height
        if ((this->height() == size.height()) && (this->width() != size.width()) ) {
            size.setHeight(size.width() * 10 / 14);
        // else adjust width
        } else {
            size.setWidth(size.height() * 14 / 10);
        }
    }
    this->resize(size);

}

auto ToggleSwitch::value() const  -> QJsonValue {
    if (this->hasValue()) {
        return QJsonValue(this->isChecked());
    }
    return QJsonValue::Null;
}


