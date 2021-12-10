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
}

yesNoDialog::~yesNoDialog()
{
    delete ui;
}

void yesNoDialog::on_buttonYes_clicked()
{
    result = true;
    close();
}

void yesNoDialog::on_buttonNo_clicked()
{
    result = false;
    close();
}

bool yesNoDialog::getResult()
{
    return result;
}
