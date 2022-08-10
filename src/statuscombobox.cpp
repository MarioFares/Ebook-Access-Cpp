#include "include/statuscombobox.h"

StatusComboBox::StatusComboBox(QWidget* parent)
{
	setParent(parent);
	addItem(QString(""));
	addItem(QString("Prospective"));
	addItem(QString("Pending"));
	addItem(QString("Complete"));
}
