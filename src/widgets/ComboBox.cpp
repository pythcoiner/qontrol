#include "ComboBox.h"
#include "../Controller.h"
#include "../common.h"
#include <qcombobox.h>
#include <qcontainerfwd.h>
#include <qlist.h>

namespace qontrol::widgets {

ComboBox::ComboBox(const QString &enum_name, QWidget *parent) : QComboBox(parent), m_enum_name(enum_name) {
    m_values = new QList<QPair<QString, QString>>;
    this->onUpdate();
    connect(this, &QComboBox::currentIndexChanged, this, &ComboBox::onValueChanged, UNIQUE);
}

void ComboBox::onUpdate() {
    auto values = Controller::get()->getEnum(m_enum_name);
    if (values) {
        // store variants
        m_values->append(*values);
        QStringList list{};
        for (const auto &variant : *m_values) {
            list.push_back(variant.second);
        }
        // reload variants
        this->clear();
        this->addItems(list);
    } else {
        qCritical() << "ComboBox::onUpdate() no values for enum " << m_enum_name;
    }
}

void ComboBox::onValueChanged(int _v) { // NOLINT
    emit this->valueChanged();
}

} // namespace qontrol::widgets
