#ifndef QUOTEDIALOG_H
#define QUOTEDIALOG_H

#include <QLabel>
#include <QDialog>
#include <QBoxLayout>
#include <QPushButton>

class QuoteDialog : public QDialog
{
Q_OBJECT

public:
	explicit QuoteDialog(QWidget* parent = nullptr);

private slots:

	void setupInterface();

	void setupConnections();

	void nextQuote();

	void prevQuote();

private:
	QStringList m_quotesList;
	quint32 m_currIndex;

	// Widgets
	QLabel* m_labelQuote;
	QPushButton* m_buttonPrevious;
	QPushButton* m_buttonNext;

	// Layouts
	QHBoxLayout* m_horLayQuote;
	QHBoxLayout* m_horLayButtons;

	// Spacers
	QSpacerItem* m_horSpacerQuoteLeft;
	QSpacerItem* m_horSpacerQuoteRight;
	QSpacerItem* m_horSpacerButtonsLeft;
	QSpacerItem* m_horSpacerButtonsRight;
	QVBoxLayout* m_vertLayMain;
};

#endif // QUOTEDIALOG_H
