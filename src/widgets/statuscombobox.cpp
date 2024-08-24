#include "include/widgets/statuscombobox.h"

StatusComboBox::StatusComboBox(QWidget *parent) {
    setParent(parent);
    addItems(_status);
}

QString StatusComboBox::convertToStatus(quint32 index) {
    return (index > 3 ? "No Status" : _status[index]);
}

QString StatusComboBox::convertToIndex(QString value) {
    return QString::number(_status.indexOf(value));
}
