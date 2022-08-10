#include "include/ratingcombobox.h"

RatingComboBox::RatingComboBox(QWidget* parent)
{
	setParent(parent);
	addItem(QString());
	addItem(QString("\360\237\237\211"));
	addItem(QString("\360\237\237\211\360\237\237\211"));
	addItem(QString("\360\237\237\211\360\237\237\211\360\237\237\211"));
	addItem(QString("\360\237\237\211\360\237\237\211\360\237\237\211\360\237\237\211"));
	addItem(QString("\360\237\237\211\360\237\237\211\360\237\237\211\360\237\237\211\360\237\237\211"));
}
