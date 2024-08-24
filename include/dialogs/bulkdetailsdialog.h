#ifndef BULKDETAILSDIALOG_H
#define BULKDETAILSDIALOG_H

#include "include/widgets/ratingcombobox.h"
#include "include/widgets/statuscombobox.h"

#include <QDialog>
#include <QBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QLineEdit>

class BulkDetailsDialog : public QDialog {
    Q_OBJECT

public:
    explicit BulkDetailsDialog(QWidget *parent = nullptr);

    QString _author;
    QString _genre;
    QString _tags;
    QString _publisher;
    QString _datePublished;
    QString _series;
    quint32 _rating;
    quint32 _status;

private slots:
    void setupInterface();

    void setupConnections();

    void setupTabOrder();

    void applyFields();

private:
    // Widgets
    QLabel *_labelPrompt;
    QLabel *_labelGenre;
    QCheckBox *_checkBoxGenre;
    QLineEdit *_textGenre;
    QLabel *_labelTags;
    QCheckBox *_checkBoxTags;
    QLineEdit *_textTags;
    QLabel *_labelAuthor;
    QCheckBox *_checkBoxAuthor;
    QLineEdit *_textAuthor;
    QLabel *_labelPublisher;
    QCheckBox *_checkBoxPublisher;
    QLineEdit *_textPublisher;
    QLabel *_labelDatePublished;
    QCheckBox *_checkBoxDatePublished;
    QLineEdit *_textDatePublished;
    QLabel *_labelSeries;
    QCheckBox *_checkBoxSeries;
    QLineEdit *_textSeries;
    QLabel *_labelStatus;
    QCheckBox *_checkBoxStatus;
    StatusComboBox *_comboBoxStatus;
    QLabel *_labelRating;
    QCheckBox *_checkBoxRating;
    RatingComboBox *_comboBoxRating;
    QPushButton *_buttonApply;
    QPushButton *_buttonCancel;

    // Layouts
    QHBoxLayout *_horLayButtons;
    QGridLayout *_gridLay;
    QVBoxLayout *_vertLayMain;

    // Spacers
    QSpacerItem *_horSpacerButtonsLeft;
};

#endif // BULKDETAILSDIALOG_H
