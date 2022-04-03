#include "ui_cleanebooksdialog.h"

#include "include/cleanebooksdialog.h"
#include "include/queries.h"

#include <QFileInfo>

CleanEbooksDialog::CleanEbooksDialog(QWidget *parent):
    QDialog(parent), ui(new Ui::cleanEbooksDialog)
{
    ui->setupUi(this);
}

CleanEbooksDialog::~CleanEbooksDialog()
{
    delete ui;
}

void CleanEbooksDialog::setupConnections()
{
    connect(ui->buttonBegin, &QPushButton::clicked, this, &CleanEbooksDialog::beginClean);
}

void CleanEbooksDialog::beginClean()
{
    queries::selectPath();
    while (queries::query.next())
    {
        QString path = queries::query.value(0).toString();
        if (!QFileInfo::exists(path))
        {
            ui->listWidget->addItem(path);
        }
    }
}
