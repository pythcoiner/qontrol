#pragma once

#include "Widget.h"
#include <QString>
#include <QWidget>
#include <qcombobox.h>
#include <qjsonvalue.h>
#include <qobject.h>

namespace qontrol::widgets {

class ComboBox : public QComboBox, Widget {
  Q_OBJECT

public:
  explicit ComboBox(const QString &enum_name ,QWidget *parent = nullptr);
  [[nodiscard]] auto value() const -> QJsonValue override;
  [[nodiscard]] auto hasValue() const -> bool override {
    return true;
  };
  [[nodiscard]] auto isIterable() const -> bool override {
    return false;
  };
  void loadValue(const QJsonValue &value, int depth = 0) override;

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
