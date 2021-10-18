#include "addbooksdialog.h"
#include "ui_addbooksdialog.h"

#include "common.h"
#include "queries.h"
#include "bulkdetailsdialog.h"
#include "extselectiondialog.h"

#include <QFileInfo>
#include <QFileDialog>
#include <QDirIterator>

addBooksDialog::addBooksDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addBooksDialog)
{
    ui->setupUi(this);
    ui->textFolderPath->setCompleter(common::dirCompleter(this));
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

void addBooksDialog::insertBooks(QFileInfo entry, QString tags, QString genre, QString author)
{
    QString name = entry.completeBaseName();
    QString path = entry.absoluteFilePath();
    QString ext = "." + entry.suffix();
    QString folder = entry.dir().dirName();
    long long size = entry.size();
    int pages = common::getPageCount(path);
    queries::insertBooksQuery(name, path, folder, ext, size, pages, tags, genre, author);
}

QVector<QFileInfo> addBooksDialog::getEntriesVector(QString dir, bool recursive)
{
    QVector<QFileInfo> entriesVector;
    QDirIterator iterator(dir, QDir::Files, recursive ? QDirIterator::Subdirectories : QDirIterator::NoIteratorFlags);
    while (iterator.hasNext())
    {
        QFileInfo file(iterator.next());
        entriesVector.push_back(file);
    }
    return entriesVector;
}

QVector<QString> addBooksDialog::getExtVector(QVector<QFileInfo> entries)
{
    QVector<QString> extVector;
    for (QFileInfo &file : entries)
    {
        QString ext = "." + file.suffix().toLower();
        if (!extVector.contains(ext))
            extVector.push_back(ext);
    }
    return extVector;
}

void addBooksDialog::setupEntries(QString dir, bool recursive)
{
    QVector<QFileInfo> entriesVector = getEntriesVector(dir, recursive);
    QVector<QString> extVector = getExtVector(entriesVector);

    bulkDetailsDialog dialog;
    common::openDialog(&dialog, ":/style.qss");

    QString tags = dialog.tags.isEmpty() ? "N/A" : dialog.tags;
    QString genres = dialog.genre.isEmpty() ? "N/A" : dialog.genre;
    QString authors = dialog.author.isEmpty() ? "N/A" : dialog.author;

    auto *extDialog = new extSelectionDialog(this, extVector, "Extensions", "Select Extensions");
    common::openDialog(extDialog, ":/style.qss");

    QVector<QString> selectedExts = extDialog->getExtVector();

    iterateInsertEntries(entriesVector, selectedExts, tags, genres, authors);
}

void addBooksDialog::iterateInsertEntries(QVector<QFileInfo> entriesVector, QVector<QString> selectedExts,
                                          QString tags, QString genres, QString authors)
{
    queries::db.transaction();
    size_t count = entriesVector.size();
    double counter = 0;
    for(QFileInfo &entry : entriesVector)
    {
        QString ext = "." + entry.suffix().toLower();
        int progress = (counter / count) * 100;
        ui->progressBar->setValue(progress);
        counter++;
        if (selectedExts.contains(ext))
        {
            insertBooks(entry, tags, genres, authors);
        }
    }
    queries::db.commit();
    ui->progressBar->setValue(100);
}

void addBooksDialog::on_buttonAdd_clicked()
{
    QString dir = ui->textFolderPath->text();
    if(QFileInfo::exists(dir))
    {
        setupEntries(dir, ui->checkBoxRecursive->isChecked());
    }
}
