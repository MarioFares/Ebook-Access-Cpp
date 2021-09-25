#include "addbooksdialog.h"
#include "ui_addbooksdialog.h"

#include "common.h"
#include "queries.h"
#include "bulkdetailsdialog.h"

#include <vector>
#include <filesystem>

#include <QMessageBox>
#include <QFileDialog>
#include <QProgressBar>
#include <QThread>

#include <iostream>
QProgressBar *progressBar;

namespace fs = std::filesystem;

addBooksDialog::addBooksDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addBooksDialog)
{
    ui->setupUi(this);
    progressBar = ui->progressBar;
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

void insertBooks(fs::directory_entry entry, QString tags, QString genre, QString author)
{
    QString name = QString::fromStdString(entry.path().stem().string());
    QString path = QString::fromStdString(entry.path().string());
    QString folder = QString::fromStdString(entry.path().parent_path().filename().string());
    QString ext = QString::fromStdString(entry.path().extension().string());
    long long size = entry.file_size();
    int pages = common::getPageCount(path);
    queries::insertBooksQuery(name, path, folder, ext, size, pages, tags, genre, author);
}

template <typename directory_iterator>
void iterateInsertEntries(directory_iterator dirIterator)
{
    std::vector<fs::directory_entry> entriesVector;
    for (fs::directory_entry entry : dirIterator)
    {
        if(entry.is_regular_file())
        {
            if(entry.path().extension() == ".pdf" || entry.path().extension() == ".epub" || entry.path().extension() == ".mobi" || entry.path().extension() == ".cbr")
            {
                entriesVector.push_back(entry);
            }
        }
    }
    size_t count = entriesVector.size();
    long counter = 0;
    bulkDetailsDialog dialog;
    common::openDialog(&dialog, ":/style.qss");
// cut this up into more functions

    QString tags = dialog.tags.isEmpty() ? "N/A" : dialog.tags;
    QString genre = dialog.genre.isEmpty() ? "N/A" : dialog.genre;
    QString author = dialog.author.isEmpty() ? "N/A" : dialog.author;


    queries::query.exec("BEGIN TRANSACTION");
    for(fs::directory_entry &entry : entriesVector)
    {
        int progress = ((double)counter / count) * 100;
        progressBar->setValue(progress);
        insertBooks(entry, tags, genre, author);
        counter++;
    }
    queries::query.exec("COMMIT");
    progressBar->setValue(100);
}


void addBooksDialog::on_buttonAdd_clicked()
{
    std::string dir = ui->textFolderPath->text().toStdString();

    if(fs::exists(fs::path(dir)))
    {
        if(ui->checkBoxRecursive->isChecked())
               iterateInsertEntries(fs::recursive_directory_iterator(dir));
        else
               iterateInsertEntries(fs::directory_iterator(dir));
    }

}

