#include "include/yesnodialog.h"
#include "ui_yesnodialog.h"

YesNoDialog::YesNoDialog(QWidget *parent, QString windowTitle, QString title, QString prompt):
    QDialog(parent), ui(new Ui::yesNoDialog)
{
    ui->setupUi(this);
    setWindowTitle(windowTitle);
    ui->labelTitle->setText(title);
    ui->labelPrompt->setText(prompt);

    result = false;
    setupConnections();
}

YesNoDialog::~YesNoDialog()
{
    delete ui;
}

void YesNoDialog::setupConnections()
{
    connect(ui->buttonYes, &QPushButton::clicked, this, &YesNoDialog::setResult);
    connect(ui->buttonNo, &QPushButton::clicked, this, &YesNoDialog::setResult);
}

void YesNoDialog::setResult()
{
    result = sender() == ui->buttonYes;
    close();
}

bool YesNoDialog::getResult()
{
    return result;
}
