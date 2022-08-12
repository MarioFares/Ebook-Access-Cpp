#include "include/widgets/ratingcombobox.h"

RatingComboBox::RatingComboBox(QWidget* parent)
{
	setParent(parent);
	addItems(m_ratings);
}

QString RatingComboBox::convertToStars(quint32 index)
{
	return (index > 5 ? "" : m_ratings[index]);
}

QString RatingComboBox::convertToIndex(QString value)
{
	return QString::number(m_ratings.indexOf(value));
}
