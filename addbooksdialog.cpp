#include "addbooksdialog.h"
#include "ui_addbooksdialog.h"

#include "common.h"
#include "queries.h"
#include "bulkdetailsdialog.h"

#include <QFileInfo>
#include <QFileDialog>
#include <QProgressBar>
#include <QDirIterator>

addBooksDialog::addBooksDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addBooksDialog)
{
    ui->setupUi(this);
}

addBooksDialog::~addBooksDialog()
{
    delete ui;
}


void addBooksDialog::on_buttonClose_clicked()
{
    close();
}

void addBooksDialog::on_buttonBrowseFolders_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose Folder"),
                                                    "/",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    ui->textFolderPath->setText(dir);

}

void addBooksDialog::insertBooks(QString entry, QString tags, QString genre, QString author)
{

    QFileInfo file(entry);

    QString name = file.completeBaseName();
    QString path = file.absoluteFilePath();
    QString folder = file.dir().dirName();
    QString ext = "." + file.suffix();
    long long size = file.size();
    int pages = common::getPageCount(path);
    queries::insertBooksQuery(name, path, folder, ext, size, pages, tags, genre, author);
}

void addBooksDialog::iterateInsertEntries(QString dir, bool recursive)
{
    QVector<QString> entriesVector;
    QDirIterator iterator(dir, {"*.pdf", "*.cbr", "*.epub", "*.mobi"}, QDir::Files,
                          recursive ? QDirIterator::Subdirectories : QDirIterator::NoIteratorFlags);

    while (iterator.hasNext())
    {
        entriesVector.push_back(iterator.next());
    }

    long count = entriesVector.size();
    long counter = 0;

    bulkDetailsDialog dialog;
    common::openDialog(&dialog, ":/style.qss");

    QString tags = dialog.tags.isEmpty() ? "N/A" : dialog.tags;
    QString genre = dialog.genre.isEmpty() ? "N/A" : dialog.genre;
    QString author = dialog.author.isEmpty() ? "N/A" : dialog.author;

    queries::query.exec("BEGIN TRANSACTION");
    for(QString &entry : entriesVector)
    {
        int progress = ((double)counter / count) * 100;
        ui->progressBar->setValue(progress);
        insertBooks(entry, tags, genre, author);
        counter++;
    }
    queries::query.exec("COMMIT");
    ui->progressBar->setValue(100);
}

void addBooksDialog::on_buttonAdd_clicked()
{
    QString dir = ui->textFolderPath->text();
    if(QFileInfo::exists(dir))
    {
        iterateInsertEntries(dir, ui->checkBoxRecursive->isChecked());
    }

}
