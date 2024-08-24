#ifndef ADDBOOKDIALOG_H
#define ADDBOOKDIALOG_H

#include "include/widgets/ratingcombobox.h"
#include "include/widgets/statuscombobox.h"

#include <QDialog>
#include <QLabel>
#include <QFrame>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>

class AddBookDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddBookDialog(QWidget *parent = nullptr);

private slots:
    void setupInterface();

    void setupConnections();

    void setupTabOrder();

    void clearInputs();

    void browseFile();

    void addFile();

private:
    // Widgets
    QLabel *_labelTitle;
    QFrame *_frame;
    QLineEdit *_textSize;
    QLineEdit *_textTags;
    QLineEdit *_textExtension;
    QLineEdit *_textFolder;
    QLineEdit *_textGenre;
    QLabel *_labelAuthor;
    QLabel *_labelPages;
    QLineEdit *_textPath;
    QLineEdit *_textName;
    QLabel *_labelPath;
    QLabel *_labelSize;
    QLabel *_labelTags;
    QLabel *_labelGenre;
    QLineEdit *_textPages;
    QPushButton *_buttonBrowse;
    QLabel *_labelFolder;
    QLineEdit *_textAuthor;
    QLabel *_labelName;
    QLabel *_labelExtension;
    QPushButton *_buttonAdd;
    QPushButton *_buttonClear;
    QPushButton *_buttonCancel;
    QLabel *_labelPublisher;
    QLineEdit *_textPublisher;
    QLabel *_labelDatePublished;
    QLineEdit *_textDatePublished;
    QLabel *_labelSeries;
    QLineEdit *_textSeries;
    QLabel *_labelRating;
    RatingComboBox *_comboBoxRating;
    QLabel *_labelStatus;
    StatusComboBox *_comboBoxStatus;

    // Layouts
    QGridLayout *_gridLay;
    QHBoxLayout *_horLayButtons;
    QHBoxLayout *_horLayStatus;
    QVBoxLayout *_vertLayMain;

    // Spacers
    QSpacerItem *_horSpacerButtonsLeft;
    QSpacerItem *_horSpacerButtonsRight;
};

#endif // ADDBOOKDIALOG_H
