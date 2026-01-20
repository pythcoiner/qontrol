#pragma once

#include "Widget.h"
#include <QComboBox>

namespace qontrol::widgets {

class ComboBox : public QComboBox, Widget {
    Q_OBJECT

public:
    explicit ComboBox(const QString &enum_name, QWidget *parent = nullptr);
    ~ComboBox() override { delete m_values; }

signals:
    void valueChanged();

public slots:
    void onUpdate();
    void onValueChanged(int _v); // NOLINT

private:
    QString m_enum_name;
    QList<QPair<QString, QString>> *m_values;
};

} // namespace qontrol::widgets
