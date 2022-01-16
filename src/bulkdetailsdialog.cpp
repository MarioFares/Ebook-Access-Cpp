#include "ui_bulkdetailsdialog.h"

#include "include/bulkdetailsdialog.h"

bulkDetailsDialog::bulkDetailsDialog(QWidget *parent):
    QDialog(parent), ui(new Ui::bulkDetailsDialog)
{
    author = "";
    genre = "";
    tags = "";
    ui->setupUi(this);
    setupConnections();
}

bulkDetailsDialog::~bulkDetailsDialog()
{
    delete ui;
}

void bulkDetailsDialog::setupConnections()
{
    connect(ui->buttonCancel, &QPushButton::clicked, this, &bulkDetailsDialog::close);
    connect(ui->buttonApply, &QPushButton::clicked, this, &bulkDetailsDialog::applyFields);
    connect(ui->checkBoxAuthor, &QCheckBox::stateChanged, ui->textAuthor, &QLineEdit::setEnabled);
    connect(ui->checkBoxGenre, &QCheckBox::stateChanged, ui->textGenre, &QLineEdit::setEnabled);
    connect(ui->checkBoxTags, &QCheckBox::stateChanged, ui->textTags, &QLineEdit::setEnabled);
}

void bulkDetailsDialog::applyFields()
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
