#include "include/widgets/ratingcombobox.h"

RatingComboBox::RatingComboBox(QWidget *parent) {
    setParent(parent);
    addItems(_ratings);
}

QString RatingComboBox::convertToStars(quint32 index) {
    return (index > 5 ? "" : _ratings[index]);
}

QString RatingComboBox::convertToIndex(QString value) {
    return QString::number(_ratings.indexOf(value));
}
