#pragma once

#include "Row.h"
#include "Widget.h"
#include <QFontMetrics>
#include <QMap>
#include <QPainter>
#include <QSlider>
#include <QString>
#include <QWidget>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qobject.h>
#include <qslider.h>
#include <qtmetamacros.h>
#include <qwidget.h>

namespace qontrol::widgets {

class Slider;

class SliderHeader : public QWidget {
    Q_OBJECT

public:
    explicit SliderHeader(Slider *parent);
    ~SliderHeader() override;
    void addLabel(int tick_position, QString label);
    void clear();
    void setPen(QPen *pen);

protected:
    // we override paintEvent as a QPainter can only be created in
    // paintEvent()
    void paintEvent(QPaintEvent *ev) override;

private:
    Slider *m_parent;
    QPen *m_pen;
    QMap<int, QPair<QRect, QString>> *m_labels;
};

class Slider : public Row, Widget {
    Q_OBJECT

public:
    explicit Slider(int padding = 20, QWidget *parent = nullptr);
    ~Slider() override;


    static auto percent(const QString &label, int label_width, QWidget *parent = nullptr) -> Slider *;
    static auto cursor(const QString &left_label, int left_label_width, const QString &right_label,
                       int right_label_width, QWidget *parent) -> Slider *;
    static auto axis(const QString &axis_name, int label_width, QList<int> ticks, bool optional, bool linear,
                     QWidget *parent) -> Slider *;
    static auto simple(QString name, int label_width, QList<int> ticks, QString unit, QWidget *parent) -> Slider *;

    void addTicks(QList<int> *ticks);
    void updateTicks();
    void updateMinMax();
    auto ticksMap() -> const QMap<int, int> *;
    auto leftSpace() -> int;
    auto rightSpace() -> int;
    [[nodiscard]] auto handleWidth() const -> int;

    [[nodiscard]] auto value() const -> int;

    void setValue(int v);

    [[nodiscard]] auto getPreSpacer() const -> int;
    void setPreSpacer(int v);

    auto slider() -> QSlider *;

signals:
    void valueUpdated(int v);

protected:
    void load();
    void connectInput();
    void connectCheckbox();
    void changeEvent(QEvent *e) override;

private slots:
    void sliderValueUpdated(int v);
    void inputValueUpdated();

private:
    SliderHeader *m_header;
    QMap<int, int> *m_ticks;
    QPen *m_pen;

    int m_spacer;
    int m_pre_spacer;
    int m_post_spacer;
    QCheckBox *m_checkbox;
    QLabel *m_pre_label;
    QLineEdit *m_input;
    QLabel *m_post_label;
    QSlider *m_slider;
};

} // namespace qontrol::widgets
