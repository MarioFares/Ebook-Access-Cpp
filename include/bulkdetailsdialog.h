#ifndef BULKDETAILSDIALOG_H
#define BULKDETAILSDIALOG_H

#include <QDialog>
#include <QBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QLineEdit>

class BulkDetailsDialog : public QDialog
{
Q_OBJECT

public:
	explicit BulkDetailsDialog(QWidget* parent = nullptr);

	QString m_author;
	QString m_genre;
	QString m_tags;

private slots:

	void setupInterface();

	void setupConnections();

	void applyFields();

private:
	// Widgets
	QLabel* m_labelPrompt;
	QCheckBox* m_checkBoxGenre;
	QLabel* m_labelTags;
	QCheckBox* m_checkBoxAuthor;
	QCheckBox* m_checkBoxTags;
	QLabel* m_labelAuthor;
	QLabel* m_labelGenre;
	QLineEdit* m_textAuthor;
	QLineEdit* m_textGenre;
	QLineEdit* m_textTags;
	QPushButton* m_buttonApply;
	QPushButton* m_buttonCancel;

	// Layouts
	QHBoxLayout* m_horLayButtons;
	QGridLayout* m_gridLay;
	QVBoxLayout* m_vertLayMain;

	// Spacers
	QSpacerItem* m_horSpacerButtonsLeft;
};

#endif // BULKDETAILSDIALOG_H
