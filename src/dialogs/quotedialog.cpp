#include "include/dialogs/quotedialog.h"

#include <QFile>
#include <QRandomGenerator>
#include <QRegularExpression>

QuoteDialog::QuoteDialog(QWidget *parent) : QDialog(parent) {
    QFile file(":/txt/quotes.txt");
    file.open(QFile::ReadOnly);
    QString quotes = file.readAll();
    _quotesList = quotes.split(QRegularExpression("\n|\r\n|\r"));
    _quotesList.removeAll(QString());

    std::shuffle(_quotesList.begin(), _quotesList.end(), std::random_device());
    _currIndex = 0;

    setupInterface();
    setupConnections();
    setupTabOrder();
}

void QuoteDialog::setupInterface() {
    // Window
    setWindowTitle("Quote of the Day");
    setFixedSize(475, 210);

    // Widgets
    _labelQuote = new QLabel(_quotesList[0]);
    _labelQuote->setAlignment(Qt::AlignCenter);
    _labelQuote->setWordWrap(true);
    QFont font;
    font.setBold(true);
    font.setItalic(true);
    font.setPointSize(10);
    _labelQuote->setFont(font);

    _buttonPrevious = new QPushButton("Previous");
    _buttonPrevious->setFlat(true);
    _buttonPrevious->setCursor(Qt::PointingHandCursor);
    _buttonPrevious->setMinimumSize(80, 25);

    _buttonNext = new QPushButton("Next");
    _buttonNext->setFlat(true);
    _buttonNext->setCursor(Qt::PointingHandCursor);
    _buttonNext->setMinimumSize(80, 25);

    // Layouts
    _horLayQuote = new QHBoxLayout();
    _horSpacerQuoteLeft = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
    _horSpacerQuoteRight = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
    _horLayQuote->addSpacerItem(_horSpacerQuoteLeft);
    _horLayQuote->addWidget(_labelQuote);
    _horLayQuote->addSpacerItem(_horSpacerQuoteRight);

    _horLayButtons = new QHBoxLayout();
    _horSpacerButtonsLeft = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
    _horSpacerButtonsRight = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
    _horLayButtons->addSpacerItem(_horSpacerButtonsLeft);
    _horLayButtons->addWidget(_buttonPrevious);
    _horLayButtons->addWidget(_buttonNext);
    _horLayButtons->addSpacerItem(_horSpacerButtonsRight);

    _vertLayMain = new QVBoxLayout();
    _vertLayMain->addLayout(_horLayQuote);
    _vertLayMain->addLayout(_horLayButtons);
    _vertLayMain->setSpacing(10);

    setLayout(_vertLayMain);
}

void QuoteDialog::setupConnections() {
    connect(_buttonNext, &QPushButton::clicked, this, &QuoteDialog::nextQuote);
    connect(_buttonPrevious, &QPushButton::clicked, this, &QuoteDialog::prevQuote);
}

void QuoteDialog::setupTabOrder() {
    QWidget::setTabOrder(_buttonPrevious, _buttonNext);
}

void QuoteDialog::nextQuote() {
    _currIndex < _quotesList.count() - 1 ? _currIndex++ : _quotesList.count();
    _labelQuote->setText(_quotesList[_currIndex]);
}

void QuoteDialog::prevQuote() {
    _currIndex > 0 ? _currIndex-- : 0;
    _labelQuote->setText(_quotesList[_currIndex]);
}
