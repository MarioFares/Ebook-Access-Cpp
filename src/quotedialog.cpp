#include "include/quotedialog.h"

#include <QFile>
#include <QRandomGenerator>
#include <QRegularExpression>

QuoteDialog::QuoteDialog(QWidget* parent) : QDialog(parent)
{
	QFile file(":/txt/quotes.txt");
	file.open(QFile::ReadOnly);
	QString quotes = file.readAll();
	m_quotesList = quotes.split(QRegularExpression("\n|\r\n|\r"));
	m_quotesList.removeAll(QString());

	std::shuffle(m_quotesList.begin(), m_quotesList.end(), std::random_device());
	m_currIndex = 0;

	setupInterface();
	setupConnections();
	setupTabOrder();
}

void QuoteDialog::setupInterface()
{
	// Window
	setWindowTitle("Quote of the Day");
	setFixedSize(475, 210);

	// Widgets
	m_labelQuote = new QLabel(m_quotesList[0]);
	m_labelQuote->setAlignment(Qt::AlignCenter);
	m_labelQuote->setWordWrap(true);
	QFont font;
	font.setBold(true);
	font.setItalic(true);
	font.setPointSize(10);
	m_labelQuote->setFont(font);

	m_buttonPrevious = new QPushButton("Previous");
	m_buttonPrevious->setFlat(true);
	m_buttonPrevious->setCursor(Qt::PointingHandCursor);
	m_buttonPrevious->setMinimumSize(80, 25);

	m_buttonNext = new QPushButton("Next");
	m_buttonNext->setFlat(true);
	m_buttonNext->setCursor(Qt::PointingHandCursor);
	m_buttonNext->setMinimumSize(80, 25);

	// Layouts
	m_horLayQuote = new QHBoxLayout();
	m_horSpacerQuoteLeft = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_horSpacerQuoteRight = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_horLayQuote->addSpacerItem(m_horSpacerQuoteLeft);
	m_horLayQuote->addWidget(m_labelQuote);
	m_horLayQuote->addSpacerItem(m_horSpacerQuoteRight);

	m_horLayButtons = new QHBoxLayout();
	m_horSpacerButtonsLeft = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_horSpacerButtonsRight = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_horLayButtons->addSpacerItem(m_horSpacerButtonsLeft);
	m_horLayButtons->addWidget(m_buttonPrevious);
	m_horLayButtons->addWidget(m_buttonNext);
	m_horLayButtons->addSpacerItem(m_horSpacerButtonsRight);

	m_vertLayMain = new QVBoxLayout();
	m_vertLayMain->addLayout(m_horLayQuote);
	m_vertLayMain->addLayout(m_horLayButtons);
	m_vertLayMain->setSpacing(10);

	setLayout(m_vertLayMain);
}

void QuoteDialog::setupConnections()
{
	connect(m_buttonNext, &QPushButton::clicked, this, &QuoteDialog::nextQuote);
	connect(m_buttonPrevious, &QPushButton::clicked, this, &QuoteDialog::prevQuote);
}

void QuoteDialog::setupTabOrder()
{
	QWidget::setTabOrder(m_buttonPrevious, m_buttonNext);
}

void QuoteDialog::nextQuote()
{
	m_currIndex < m_quotesList.count() - 1 ? m_currIndex++ : m_quotesList.count();
	m_labelQuote->setText(m_quotesList[m_currIndex]);
}

void QuoteDialog::prevQuote()
{
	m_currIndex > 0 ? m_currIndex-- : 0;
	m_labelQuote->setText(m_quotesList[m_currIndex]);
}
