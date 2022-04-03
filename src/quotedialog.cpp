#include "include/quotedialog.h"
#include "ui_quotedialog.h"

#include <QFile>
#include <QRandomGenerator>
#include <QRegularExpression>

QuoteDialog::QuoteDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::quoteDialog)
{
    ui->setupUi(this);

    QFile file(":/txt/quotes.txt");
    file.open(QFile::ReadOnly);
    QString quotes = file.readAll();
    this->quotesList = quotes.split(QRegularExpression("\n|\r\n|\r"));
    this->quotesList.removeAll(QString());

    std::shuffle(quotesList.begin(), quotesList.end(), std::random_device());
    ui->label->setText(this->quotesList[0]);

    this->currIndex = 0;
    setupConnections();
}

QuoteDialog::~QuoteDialog()
{
    delete ui;
}

void QuoteDialog::setupConnections()
{
    connect(ui->buttonNext, &QPushButton::clicked, this, &QuoteDialog::nextQuote);
    connect(ui->buttonPrevious, &QPushButton::clicked, this, &QuoteDialog::prevQuote);
}

void QuoteDialog::nextQuote()
{
    this->currIndex < this->quotesList.count() - 1 ? this->currIndex++ : this->quotesList.count();
    ui->label->setText(this->quotesList[this->currIndex]);
}

void QuoteDialog::prevQuote()
{
    this->currIndex > 0 ? this->currIndex-- : 0;
    ui->label->setText(this->quotesList[this->currIndex]);
}

