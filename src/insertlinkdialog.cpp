#include "ui_insertlinkdialog.h"

#include "include/insertlinkdialog.h"

insertLinkDialog::insertLinkDialog(QWidget *parent):
    QDialog(parent), ui(new Ui::insertLinkDialog)
{
    title = "";
    link = "";
    ui->setupUi(this);
    setupConnections();
}

insertLinkDialog::~insertLinkDialog()
{
    delete ui;
}

void insertLinkDialog::setupConnections()
{
    connect(ui->buttonCancel, &QPushButton::clicked, this, &insertLinkDialog::close);
    connect(ui->buttonOk, &QPushButton::clicked, this, &insertLinkDialog::getInput);
}

void insertLinkDialog::getInput()
{
    title = ui->textTitle->text();
    link = ui->textLink->text();
    close();
}

void insertLinkDialog::setDataOnOpen()
{
    if (!title.isEmpty() && !link.isEmpty())
    {
        ui->textLink->setText(link);
        ui->textTitle->setText(title);
    }
}
