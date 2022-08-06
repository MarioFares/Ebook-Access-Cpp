#ifndef BOOKDETAILSWINDOW_H
#define BOOKDETAILSWINDOW_H

#include "include/booksearchwidget.h"

#include <QLabel>
#include <QComboBox>
#include <QSplitter>
#include <QMainWindow>
#include <QPushButton>
#include <QGraphicsView>
#include <QPlainTextEdit>
#include <QListWidgetItem>

class BookMetadataWindow : public QMainWindow
{
Q_OBJECT

public:
	explicit BookMetadataWindow(QWidget* parent = nullptr);

	void showBookDetails(const QString& name);

private:
	void setupInterface();

	void setupConnections();

	void setupTabOrder();

	QWidget* m_centralwidget;
	BookSearchWidget* m_bookSearchWidget;
	QSplitter* m_rightSplitter;
	QSplitter* m_splitter;
	QSplitter* m_verticalSplitter;
	QLabel* m_labelName;
	QLineEdit* m_textGenre;
	QLabel* m_labelDatePublished;
	QLineEdit* m_textPublisher;
	QLineEdit* m_textDatePublished;
	QLabel* m_labelPath;
	QLineEdit* m_textExt;
	QLabel* m_labelDateAdded;
	QLineEdit* m_textAuthor;
	QComboBox* m_comboBoxRating;
	QLabel* m_labelExt;
	QLabel* m_labelFolder;
	QLineEdit* m_textFolder;
	QLabel* m_labelPages;
	QLineEdit* m_textSeries;
	QLabel* m_labelSeries;
	QLineEdit* m_textDateAdded;
	QLabel* m_labelRating;
	QLineEdit* m_textPages;
	QLabel* m_labelSize;
	QLabel* m_labelAuthor;
	QLabel* m_labelGenre;
	QLabel* m_labelPublisher;
	QLineEdit* m_textName;
	QLabel* m_labelStatus;
	QComboBox* m_comboBoxStatus;
	QLineEdit* m_textPath;
	QLineEdit* m_textSize;
	QLabel* m_labelTags;
	QLineEdit* m_textTags;
	QLabel* m_labelModified;
	QLineEdit* m_textDateModified;
	QFrame* m_frameComments;
	QLabel* m_labelComments;
	QPlainTextEdit* m_textEditComments;
	QGraphicsView* m_graphicsView;
	QFrame* m_frameButtons;
	QPushButton* m_buttonUpdate;
	QPushButton* m_buttonPrev;
	QPushButton* m_buttonNext;
	QPushButton* m_buttonCancel;
	QMenuBar* m_menubar;
	QMenu* m_menuFile;
	QMenu* m_menuEdit;
	QStatusBar* m_statusbar;
	QWidget* m_gridWidget;

	// Layouts
	QHBoxLayout* m_horLayStatus;
	QGridLayout* m_gridLayData;
	QVBoxLayout* m_vertLayComments;
	QHBoxLayout* m_horLayButtons;
	QSpacerItem* m_horSpacerButtonsLeft;
	QSpacerItem* m_horSpacerButtonsRight;
	QVBoxLayout* m_vertLayMain;

};

#endif // BOOKDETAILSWINDOW_H
