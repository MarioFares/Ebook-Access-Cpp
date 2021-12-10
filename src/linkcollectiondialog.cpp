#include "ui_linkcollectiondialog.h"

#include "include/linkcollectiondialog.h"
#include "include/common.h"

linkCollectionDialog::linkCollectionDialog(QWidget *parent):
    QDialog(parent), ui(new Ui::linkCollectionDialog)
{
    ui->setupUi(this);
}

linkCollectionDialog::~linkCollectionDialog()
{
    delete ui;
}

void linkCollectionDialog::on_buttonCancel_clicked()
{
    close();
}

void linkCollectionDialog::on_buttonOk_clicked()
{
    collectionName = ui->lineEdit->text();
    if (collectionName.isEmpty())
    {
        common::showMsgBox("Name Error!", "Name cannot be empty.", ":/styles/summarystyle.qss", QMessageBox::Warning, ":/icons/books_icon.png");
    }
    else
    {
        this->close();
    }
}

void linkCollectionDialog::setDataOnOpen()
{
    ui->lineEdit->setText(collectionName);
}