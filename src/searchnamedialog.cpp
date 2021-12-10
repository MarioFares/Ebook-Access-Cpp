#include "ui_searchnamedialog.h"

#include "include/searchnamedialog.h"
#include "include/common.h"

searchNameDialog::searchNameDialog(QWidget *parent):
    QDialog(parent), ui(new Ui::searchNameDialog)
{
    ui->setupUi(this);
}

searchNameDialog::~searchNameDialog()
{
    delete ui;
}

void searchNameDialog::on_buttonCancel_clicked()
{
    this->close();
}

void searchNameDialog::on_buttonOk_clicked()
{
    searchName = ui->lineEdit->text();
    if (searchName.isEmpty())
    {
        common::showMsgBox("Name Error!", "Name cannot be empty.", ":/styles/summarystyle.qss", QMessageBox::Warning, ":/icons/books_icon.png");
    }
    else
    {
        this->close();
    }
}
