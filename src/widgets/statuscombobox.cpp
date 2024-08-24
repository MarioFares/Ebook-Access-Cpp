#include "include/widgets/statuscombobox.h"

StatusComboBox::StatusComboBox(QWidget *parent) {
    setParent(parent);
    addItems(m_status);
}

QString StatusComboBox::convertToStatus(quint32 index) {
    return (index > 3 ? "No Status" : m_status[index]);
}

QString StatusComboBox::convertToIndex(QString value) {
    return QString::number(m_status.indexOf(value));
}
