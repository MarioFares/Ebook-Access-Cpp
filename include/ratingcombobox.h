#ifndef RATINGCOMBOBOX_H
#define RATINGCOMBOBOX_H

#include <QWidget>
#include <QComboBox>

class RatingComboBox : public QComboBox
{
Q_OBJECT

public:
	explicit RatingComboBox(QWidget* parent = nullptr);

};

#endif // RATINGCOMBOBOX_H
