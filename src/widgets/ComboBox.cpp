#include "ComboBox.h"
#include "../common.h"
#include <QLoggingCategory>

namespace qontrol::widgets {

ComboBox::ComboBox(const QString &enum_name, QWidget *parent) : QComboBox(parent), m_enum_name(enum_name) {
    m_values = new QList<QPair<QString, QString>>;
    this->onUpdate();
    connect(this, &QComboBox::currentIndexChanged, this, &ComboBox::onValueChanged, UNIQUE);
}

void ComboBox::onUpdate() {
    // Enum system has been removed - populate items directly via addItem/addItems
    qWarning() << "ComboBox::onUpdate() enum system is no longer available, use addItem/addItems instead";
}

void ComboBox::onValueChanged(int _v) { // NOLINT
    emit this->valueChanged();
}

} // namespace qontrol::widgets
