#include "include/bulkdetailsdialog.h"
#include "ui_bulkdetailsdialog.h"

bulkDetailsDialog::bulkDetailsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::bulkDetailsDialog)
{
    ui->setupUi(this);
}

bulkDetailsDialog::~bulkDetailsDialog()
{
    delete ui;
}

void bulkDetailsDialog::on_checkBoxAuthor_stateChanged(int arg1)
{
    ui->textAuthor->setEnabled(arg1);
}

void bulkDetailsDialog::on_checkBoxGenre_stateChanged(int arg1)
{
    ui->textGenre->setEnabled(arg1);
}

void bulkDetailsDialog::on_checkBoxTags_stateChanged(int arg1)
{
    ui->textTags->setEnabled(arg1);
}

void bulkDetailsDialog::on_buttonCancel_clicked()
{
    close();
}

void bulkDetailsDialog::on_buttonApply_clicked()
{
    if (ui->textAuthor->isEnabled())
    {
        author = ui->textAuthor->text();
    }
    if (ui->textGenre->isEnabled())
    {
        genre = ui->textGenre->text();
    }
    if (ui->textTags->isEnabled())
    {
        tags = ui->textTags->text();
    }
    close();
}
