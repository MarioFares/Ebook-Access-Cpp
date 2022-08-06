#ifndef CLEANEBOOKSDIALOG_H
#define CLEANEBOOKSDIALOG_H

#include <QLabel>
#include <QDialog>
#include <QBoxLayout>
#include <QSpacerItem>
#include <QListWidget>

class CleanBooksDialog : public QDialog
{
Q_OBJECT

public:
	explicit CleanBooksDialog(QWidget* parent = nullptr);

private slots:

	void setupInterface();

	void setupConnections();

	void setupTabOrder();

	void beginClean();

private:
	// Widgets
	QLabel* m_labelTitle;
	QPushButton* m_buttonBegin;
	QListWidget* m_listWidget;

	// Layouts
	QHBoxLayout* m_horLayTitle;
	QHBoxLayout* m_horLayButton;
	QVBoxLayout* m_vertLayMain;

	// Spacers
	QSpacerItem* m_horSpacerTitleLeft;
	QSpacerItem* m_horSpacerTitleRight;
	QSpacerItem* m_horSpacerButtonLeft;
	QSpacerItem* m_horSpacerButtonRight;
};

#endif // CLEANEBOOKSDIALOG_H
