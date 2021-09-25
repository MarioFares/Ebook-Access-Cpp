#include "cleanebooksdialog.h"
#include "ui_cleanebooksdialog.h"

#include "queries.h"

#include <filesystem>

namespace fs = std::filesystem;

cleanEbooksDialog::cleanEbooksDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cleanEbooksDialog)
{
    ui->setupUi(this);
}

cleanEbooksDialog::~cleanEbooksDialog()
{
    delete ui;
}

void cleanEbooksDialog::on_buttonBegin_clicked()
{
    queries::selectPath();
    while(queries::query.next())
    {
        QString path = queries::query.value(0).toString();
        if(!fs::exists(fs::path(path.toStdString())))
        {
            ui->listWidget->addItem(path);
        }
    }
}

