#include "ComboBox.h"
#include "../Controller.h" 
#include "../common.h"
#include <qassert.h>
#include <qcombobox.h>
#include <qcontainerfwd.h>
#include <qjsonvalue.h>
#include <qlist.h>

ComboBox::ComboBox( const QString &enum_name ,QWidget *parent) : 
    QComboBox(parent), 
    m_enum_name(enum_name)
{
    m_values = new QList<QPair<QString, QString>>;
    this->setKey(enum_name);
    this->onUpdate();
    connect(this, &QComboBox::currentIndexChanged, this, &ComboBox::onValueChanged, UNIQUE);

}

void ComboBox::onUpdate() {
    auto values = Controller::get()->getEnum(m_enum_name);
    if (values) {
        // store variants
        m_values->append(*values);
        QStringList list{};
        for (const auto& variant : *m_values) {
            list.push_back(variant.second);
        }
        // reload variants
        this->clear();
        this->addItems(list);
    } else {
        qCritical() << "ComboBox::onUpdate() no values for enum " << m_enum_name;
    }
}

auto ComboBox::value() const -> QJsonValue {
    Q_ASSERT(this->currentIndex() <= m_values->size());
    int index = 0;
    if (this->currentIndex() > -1) {
        index = this->currentIndex();
        if (!m_values->isEmpty()) {
            return QJsonValue(m_values->at(index).first);
        }
    }
    return QJsonValue::Null;
}

void ComboBox::onValueChanged(int _v) { // NOLINT
    emit this->valueChanged();
}

void ComboBox::loadValue(const QJsonValue &value, int depth) {
    if (value.isString()) {
        auto str = value.toString();
        for (const auto& kv : *m_values) {
            if (kv.first == str) {
                auto index = m_values->indexOf(kv);
                Q_ASSERT(index > -1);
                this->setCurrentIndex(index);
                return;
            }
        }
        qCritical() << "ComboBox::loadValue() value is not a valid variant";
    } else {
        qCritical() << "ComboBox::loadValue() value is not of type String";
    }
}
