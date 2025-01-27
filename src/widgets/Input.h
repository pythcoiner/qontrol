#pragma once

#include "Widget.h"
#include <QLineEdit>
#include <QString>
#include <QWidget>
#include <qjsonvalue.h>
#include <qobject.h>

namespace qontrol::widgets {

enum class InputType : uint8_t {
    String,
    Int,
    Float,
};

class Input : public QLineEdit, Widget {
  Q_OBJECT

public:
  explicit Input(InputType input_type, QWidget *parent = nullptr);
  Input(const QString &name, InputType input_type, QWidget *parent = nullptr);
  [[nodiscard]] auto value() const -> QJsonValue override;
  [[nodiscard]] auto hasValue() const -> bool override {
    return true;
  };
  [[nodiscard]] auto isIterable() const -> bool override {
    return false;
  };

  [[nodiscard]] auto intValue() const -> QJsonValue;
  [[nodiscard]] auto floatValue() const -> QJsonValue;
  [[nodiscard]] auto stringValue() const -> QJsonValue;

  void setType(InputType input_type);
  void loadValue(const QJsonValue &value, int depth = 0) override;

signals:
  void valueChanged();

public slots:
  void onUpdate();
  void onValueChanged(int _v); // NOLINT

private:
  InputType m_type;
};

} // namespace qontrol::widgets
