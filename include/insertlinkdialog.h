#ifndef INSERTLINKDIALOG_H
#define INSERTLINKDIALOG_H

#include <QLabel>
#include <QDialog>
#include <QLineEdit>
#include <QBoxLayout>
#include <QPushButton>

class InsertLinkDialog : public QDialog
{
Q_OBJECT

public:
	explicit InsertLinkDialog(QWidget* parent = nullptr);

	QString m_title;
	QString m_link;

	void setDataOnOpen();

private slots:

	void setupInterface();

	void setupConnections();

	void setupTabOrder();

	void getInput();

private:
	// Widgets
	QLabel* m_labelLink;
	QLabel* m_labelTitle;
	QLineEdit* m_textLink;
	QLineEdit* m_textTitle;
	QPushButton* m_buttonOk;
	QPushButton* m_buttonCancel;

	// Layouts
	QHBoxLayout* m_horLayButtons;
	QGridLayout* m_gridLay;

	// Spacers
	QSpacerItem* m_horSpacerButtonsLeft;
};

#endif // INSERTLINKDIALOG_H
