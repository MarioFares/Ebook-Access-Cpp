#include "ui_bulkdetailsdialog.h"

#include "include/bulkdetailsdialog.h"

BulkDetailsDialog::BulkDetailsDialog(QWidget *parent):
    QDialog(parent), ui(new Ui::bulkDetailsDialog)
{
    author = "";
    genre = "";
    tags = "";
    ui->setupUi(this);
    setupConnections();
}

BulkDetailsDialog::~BulkDetailsDialog()
{
    delete ui;
}

void BulkDetailsDialog::setupConnections()
{
    connect(ui->buttonCancel, &QPushButton::clicked, this, &BulkDetailsDialog::close);
    connect(ui->buttonApply, &QPushButton::clicked, this, &BulkDetailsDialog::applyFields);
    connect(ui->checkBoxAuthor, &QCheckBox::stateChanged, ui->textAuthor, &QLineEdit::setEnabled);
    connect(ui->checkBoxGenre, &QCheckBox::stateChanged, ui->textGenre, &QLineEdit::setEnabled);
    connect(ui->checkBoxTags, &QCheckBox::stateChanged, ui->textTags, &QLineEdit::setEnabled);
}

void BulkDetailsDialog::applyFields()
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
