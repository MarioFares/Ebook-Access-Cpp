#include "include/getnamedialog.h"
#include "ui_getnamedialog.h"

#include "include/common.h"

getNameDialog::getNameDialog(QWidget *parent, QString title, QString prompt) :
    QDialog(parent),
    ui(new Ui::getNameDialog)
{
    ui->setupUi(this);
    ui->labelTitle->setText(title);
    ui->labelPrompt->setText(prompt);

    setupConnections();
}

getNameDialog::~getNameDialog()
{
    delete ui;
}

void getNameDialog::setupConnections()
{
    connect(ui->buttonCancel, &QPushButton::clicked, this, &getNameDialog::close);
    connect(ui->buttonOk, &QPushButton::clicked, this, &getNameDialog::setName);
}

void getNameDialog::setName()
{
    name = ui->textName->text();
    if (name.isEmpty())
    {
        common::showMsgBox("Name Error!", "Name cannot be empty.", ":/styles/summarystyle.qss", QMessageBox::Warning, ":/icons/books_icon.png");
    }
    else
    {
        this->close();
    }
}

void getNameDialog::setDataOnOpen()
{
    ui->textName->setText(name);
}
