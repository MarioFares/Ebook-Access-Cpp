#ifndef YESNODIALOG_H
#define YESNODIALOG_H

#include <QLabel>
#include <QDialog>
#include <QBoxLayout>
#include <QPushButton>

class YesNoDialog : public QDialog
{
Q_OBJECT

public:
	explicit YesNoDialog(QWidget* parent = nullptr, QString windowTitle = "Select",
						 QString title = "Select", QString prompt = "Select");

	bool getResult() const;

private slots:

	void setupInterface();

	void setupConnections();

	void setupTabOrder();

	void setResult();

private:
	QString m_windowTitle;
	QString m_title;
	QString m_prompt;
	bool m_result;

	// Widgets
	QLabel* m_labelTitle;
	QLabel* m_labelPrompt;
	QPushButton* m_buttonYes;
	QPushButton* m_buttonNo;

	// Layouts
	QHBoxLayout* m_horLayTitle;
	QHBoxLayout* m_horLayButtons;
	QVBoxLayout* m_vertLayMain;

	// Spacers
	QSpacerItem* m_horSpacerTitleLeft;
	QSpacerItem* m_horSpacerTitleRight;
	QSpacerItem* m_horSpacerButtonsLeft;
};

#endif // YESNODIALOG_H
