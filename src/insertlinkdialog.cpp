#include "ui_insertlinkdialog.h"

#include "include/insertlinkdialog.h"

InsertLinkDialog::InsertLinkDialog(QWidget *parent):
    QDialog(parent), ui(new Ui::insertLinkDialog)
{
    title = "";
    link = "";
    ui->setupUi(this);
    setupConnections();
}

InsertLinkDialog::~InsertLinkDialog()
{
    delete ui;
}

void InsertLinkDialog::setupConnections()
{
    connect(ui->buttonCancel, &QPushButton::clicked, this, &InsertLinkDialog::close);
    connect(ui->buttonOk, &QPushButton::clicked, this, &InsertLinkDialog::getInput);
}

void InsertLinkDialog::getInput()
{
    title = ui->textTitle->text();
    link = ui->textLink->text();
    close();
}

void InsertLinkDialog::setDataOnOpen()
{
    if (!title.isEmpty() && !link.isEmpty())
    {
        ui->textLink->setText(link);
        ui->textTitle->setText(title);
    }
}
