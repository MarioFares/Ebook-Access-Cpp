#include "include/getnamedialog.h"
#include "ui_getnamedialog.h"

#include "include/common.h"

GetNameDialog::GetNameDialog(QWidget *parent, QString title, QString prompt) :
    QDialog(parent),
    ui(new Ui::getNameDialog)
{
    ui->setupUi(this);
    ui->labelTitle->setText(title);
    ui->labelPrompt->setText(prompt);

    setupConnections();
}

GetNameDialog::~GetNameDialog()
{
    delete ui;
}

void GetNameDialog::setupConnections()
{
    connect(ui->buttonCancel, &QPushButton::clicked, this, &GetNameDialog::close);
    connect(ui->buttonOk, &QPushButton::clicked, this, &GetNameDialog::setName);
}

void GetNameDialog::setName()
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

void GetNameDialog::setDataOnOpen()
{
    ui->textName->setText(name);
}
