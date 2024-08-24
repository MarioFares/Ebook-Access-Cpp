#ifndef RATINGCOMBOBOX_H
#define RATINGCOMBOBOX_H

#include <QWidget>
#include <QComboBox>
#include <QStringList>

class RatingComboBox : public QComboBox {
    Q_OBJECT

private:
    static inline const QStringList m_ratings = {
        "Unrated",
        "\360\237\237\211",
        "\360\237\237\211\360\237\237\211",
        "\360\237\237\211\360\237\237\211\360\237\237\211",
        "\360\237\237\211\360\237\237\211\360\237\237\211\360\237\237\211",
        "\360\237\237\211\360\237\237\211\360\237\237\211\360\237\237\211\360\237\237\211"
    };

public:
    explicit RatingComboBox(QWidget *parent = nullptr);

    static QString convertToStars(quint32 index);

    static QString convertToIndex(QString value);
};

#endif // RATINGCOMBOBOX_H
