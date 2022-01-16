#include "include/quotedialog.h"
#include "ui_quotedialog.h"

#include <QFile>
#include <QRandomGenerator>
#include <QRegularExpression>

quoteDialog::quoteDialog(QWidget *parent) :
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

quoteDialog::~quoteDialog()
{
    delete ui;
}

void quoteDialog::setupConnections()
{
    connect(ui->buttonNext, &QPushButton::clicked, this, &quoteDialog::nextQuote);
    connect(ui->buttonPrevious, &QPushButton::clicked, this, &quoteDialog::prevQuote);
}

void quoteDialog::nextQuote()
{
    this->currIndex < this->quotesList.count() - 1 ? this->currIndex++ : this->quotesList.count();
    ui->label->setText(this->quotesList[this->currIndex]);
}

void quoteDialog::prevQuote()
{
    this->currIndex > 0 ? this->currIndex-- : 0;
    ui->label->setText(this->quotesList[this->currIndex]);
}

