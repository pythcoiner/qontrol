#pragma once

#include "Widget.h"
#include <QObject>
#include <QWidget>
#include <QtWidgets>
#include <qcheckbox.h>
#include <qevent.h>
#include <qjsonvalue.h>
#include <qsize.h>

class ToggleSwitch : public QCheckBox, Widget
{
Q_OBJECT
Q_PROPERTY(QBrush brush READ brush WRITE setBrush)

public:

    ToggleSwitch(QWidget * = Q_NULLPTR);
    ToggleSwitch(const QBrush &, QWidget * = Q_NULLPTR);

    [[nodiscard]] virtual auto brush(void) const -> QBrush;
    virtual void setBrush(const QBrush &brush);

    [[nodiscard]] auto hasValue() const -> bool override {
        return this->hasKey();
    }
    [[nodiscard]] auto value() const -> QJsonValue override;
    [[nodiscard]] auto isIterable() const -> bool override {
        return false;
    }

protected:

    void paintEvent(QPaintEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void resizeEvent(QResizeEvent *e) override;

private:

    int m_margin;
    QBrush m_body_brush;
    QBrush m_head_brush;
};

inline auto ToggleSwitch::brush(void) const -> QBrush
{
    return this->m_head_brush;
}

inline void ToggleSwitch::setBrush(const QBrush &brush)
{
    this->m_head_brush = brush;
}

