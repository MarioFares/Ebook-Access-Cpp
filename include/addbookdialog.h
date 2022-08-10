#ifndef ADDBOOKDIALOG_H
#define ADDBOOKDIALOG_H

#include "include/ratingcombobox.h"
#include "include/statuscombobox.h"

#include <QDialog>
#include <QLabel>
#include <QFrame>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>

class AddBookDialog : public QDialog
{
Q_OBJECT

public:
	explicit AddBookDialog(QWidget* parent = nullptr);

private slots:

	void setupInterface();

	void setupConnections();

	void setupTabOrder();

	void clearInputs();

	void browseFile();

	void addFile();

private:
	// Widgets
	QLabel* m_labelTitle;
	QFrame* m_frame;
	QLineEdit* m_textSize;
	QLineEdit* m_textTags;
	QLineEdit* m_textExtension;
	QLineEdit* m_textFolder;
	QLineEdit* m_textGenre;
	QLabel* m_labelAuthor;
	QLabel* m_labelPages;
	QLineEdit* m_textPath;
	QLineEdit* m_textName;
	QLabel* m_labelPath;
	QLabel* m_labelSize;
	QLabel* m_labelTags;
	QLabel* m_labelGenre;
	QLineEdit* m_textPages;
	QPushButton* m_buttonBrowse;
	QLabel* m_labelFolder;
	QLineEdit* m_textAuthor;
	QLabel* m_labelName;
	QLabel* m_labelExtension;
	QPushButton* m_buttonAdd;
	QPushButton* m_buttonClear;
	QPushButton* m_buttonCancel;
	QLabel* m_labelPublisher;
	QLineEdit* m_textPublisher;
	QLabel* m_labelDatePublished;
	QLineEdit* m_textDatePublished;
	QLabel* m_labelSeries;
	QLineEdit* m_textSeries;
	QLabel* m_labelRating;
	RatingComboBox* m_comboBoxRating;
	QLabel* m_labelStatus;
	StatusComboBox* m_comboBoxStatus;

	// Layouts
	QGridLayout* m_gridLay;
	QHBoxLayout* m_horLayButtons;
	QHBoxLayout* m_horLayStatus;
	QVBoxLayout* m_vertLayMain;

	// Spacers
	QSpacerItem* m_horSpacerButtonsLeft;
	QSpacerItem* m_horSpacerButtonsRight;
};

#endif // ADDBOOKDIALOG_H
