#include "ui_insertlinkdialog.h"

#include "include/insertlinkdialog.h"

insertLinkDialog::insertLinkDialog(QWidget *parent):
    QDialog(parent), ui(new Ui::insertLinkDialog)
{
    title = "";
    link = "";
    ui->setupUi(this);
}

insertLinkDialog::~insertLinkDialog()
{
    delete ui;
}

void insertLinkDialog::on_buttonCancel_clicked()
{
    close();
}

void insertLinkDialog::on_buttonOk_clicked()
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
