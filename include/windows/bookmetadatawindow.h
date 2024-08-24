#ifndef BOOKDETAILSWINDOW_H
#define BOOKDETAILSWINDOW_H

#include "include/widgets/statuscombobox.h"
#include "include/widgets/ratingcombobox.h"
#include "include/widgets/booksearchwidget.h"

#include <QLabel>
#include <QComboBox>
#include <QSplitter>
#include <QMainWindow>
#include <QPushButton>
#include <QTableWidget>
#include <QPlainTextEdit>
#include <QListWidgetItem>

class BookMetadataWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit BookMetadataWindow(QWidget *parent = nullptr);

    void showBookDetails(const QString &name);

private:
    void setupInterface();

    void setupConnections();

    void setupTabOrder();

    void clearDetails();

    void restoreDetails();

    void updateDetails();

    void nextBook();

    void prevBook();

    QWidget *_centralwidget;
    BookSearchWidget *_bookSearchWidget;
    QSplitter *_rightSplitter;
    QSplitter *_splitter;
    QSplitter *_verticalSplitter;
    QLabel *_labelName;
    QLineEdit *_textGenre;
    QLabel *_labelDatePublished;
    QLineEdit *_textPublisher;
    QLineEdit *_textDatePublished;
    QLabel *_labelPath;
    QLineEdit *_textExt;
    QLabel *_labelDateAdded;
    QLineEdit *_textAuthor;
    RatingComboBox *_comboBoxRating;
    QLabel *_labelExt;
    QLabel *_labelFolder;
    QLineEdit *_textFolder;
    QLabel *_labelPages;
    QLineEdit *_textSeries;
    QLabel *_labelSeries;
    QLineEdit *_textDateAdded;
    QLabel *_labelRating;
    QLineEdit *_textPages;
    QLabel *_labelSize;
    QLabel *_labelAuthor;
    QLabel *_labelGenre;
    QLabel *_labelPublisher;
    QLineEdit *_textName;
    QLabel *_labelStatus;
    StatusComboBox *_comboBoxStatus;
    QLineEdit *_textPath;
    QLineEdit *_textSize;
    QLabel *_labelTags;
    QLineEdit *_textTags;
    QLabel *_labelModified;
    QLineEdit *_textDateModified;
    QFrame *_frameComments;
    QLabel *_labelComments;
    QPlainTextEdit *_textEditComments;
    QFrame *_frameButtons;
    QPushButton *_buttonRestore;
    QPushButton *_buttonClear;
    QPushButton *_buttonUpdate;
    QPushButton *_buttonPrev;
    QPushButton *_buttonNext;
    QFrame *_vertLineButtons;
    QMenuBar *_menubar;
    QMenu *_menuFile;
    QMenu *_menuEdit;
    QStatusBar *_statusbar;
    QWidget *_gridWidget;
    QTableWidget *_tableWidgetBookmarks;
    QFrame *_frameBookmarks;

    // Layouts
    QHBoxLayout *_horLayStatus;
    QGridLayout *_gridLayData;
    QVBoxLayout *_vertLayComments;
    QHBoxLayout *_horLayButtons;
    QSpacerItem *_horSpacerButtonsLeft;
    QSpacerItem *_horSpacerButtonsRight;
    QVBoxLayout *_vertLayMain;
    QVBoxLayout *_vertLayBookmarks;
};

#endif // BOOKDETAILSWINDOW_H
