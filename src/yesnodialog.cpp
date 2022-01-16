#include "include/yesnodialog.h"
#include "ui_yesnodialog.h"

yesNoDialog::yesNoDialog(QWidget *parent, QString windowTitle, QString title, QString prompt):
    QDialog(parent), ui(new Ui::yesNoDialog)
{
    ui->setupUi(this);
    setWindowTitle(windowTitle);
    ui->labelTitle->setText(title);
    ui->labelPrompt->setText(prompt);

    result = false;
    setupConnections();
}

yesNoDialog::~yesNoDialog()
{
    delete ui;
}

void yesNoDialog::setupConnections()
{
    connect(ui->buttonYes, &QPushButton::clicked, this, &yesNoDialog::setResult);
    connect(ui->buttonNo, &QPushButton::clicked, this, &yesNoDialog::setResult);
}

void yesNoDialog::setResult()
{
    result = sender() == ui->buttonYes;
    close();
}

bool yesNoDialog::getResult()
{
    return result;
}
