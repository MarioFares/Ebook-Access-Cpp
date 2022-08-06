#ifndef ADDBOOKDIALOG_H
#define ADDBOOKDIALOG_H

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

	// Layouts
	QGridLayout* m_gridLay;
	QHBoxLayout* m_horLayButtons;
	QVBoxLayout* m_vertLayMain;

	// Spacers
	QSpacerItem* m_horSpacerButtonsLeft;
	QSpacerItem* m_horSpacerButtonsRight;
};

#endif // ADDBOOKDIALOG_H
