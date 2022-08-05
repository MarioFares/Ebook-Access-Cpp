#ifndef EXTSELECTIONDIALOG_H
#define EXTSELECTIONDIALOG_H

#include <QDialog>
#include <QVector>
#include <QListWidgetItem>
#include <QBoxLayout>
#include <QLabel>

class ExtSelectionDialog : public QDialog
{
Q_OBJECT

public:
	explicit ExtSelectionDialog(QWidget* parent = nullptr, QVector<QString> extVector = {},
								QString title = "Preferences", QString label = "Select Preferences");

	QVector<QString> getExtVector();

private slots:

	void setupInterface();

	void setupConnections();

	void fillListWidget(QVector<QString> extVector);

	static void setItemState(QListWidgetItem* item);

	void searchText(const QString& arg1);

	void setItemsCheckState(Qt::CheckState state);

private:
	QVector<QString> m_outputVector;
	QVector<QString> m_inputVector;

	void setExtVector();

	QString m_title;
	QString m_label;

	// Widgets
	QLabel* m_labelTitle;
	QLineEdit* m_textSearch;
	QListWidget* m_listWidget;
	QPushButton* m_buttonSelectAll;
	QPushButton* m_buttonDeselectAll;
	QPushButton* m_buttonContinue;

	// Layouts
	QHBoxLayout* m_horLayTitle;
	QHBoxLayout* m_horLayButtons;
	QVBoxLayout* m_vertLayMain;

	// Spacers
	QSpacerItem* m_horSpacerTitleLeft;
	QSpacerItem* m_horSpacerTitleRight;
	QSpacerItem* m_horSpacerButtonsLeft;
	QSpacerItem* m_horSpacerButtonsRight;
};

#endif // EXTSELECTIONDIALOG_H
