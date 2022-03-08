#include "ui_addbooksdialog.h"

#include "include/addbooksdialog.h"
#include "include/common.h"
#include "include/queries.h"
#include "include/bulkdetailsdialog.h"
#include "include/extselectiondialog.h"

#include <QFileInfo>
#include <QFileDialog>
#include <QDirIterator>

addBooksDialog::addBooksDialog(QWidget *parent):
    QDialog(parent), ui(new Ui::addBooksDialog)
{
    ui->setupUi(this);
    setupConnections();
    ui->textFolderPath->setCompleter(common::dirCompleter(this));
}

addBooksDialog::~addBooksDialog()
{
    delete ui;
}

void addBooksDialog::setupConnections()
{
    connect(ui->buttonClose, &QPushButton::clicked, this, &addBooksDialog::close);
    connect(ui->buttonBrowseFolders, &QPushButton::clicked, this, &addBooksDialog::browseDirs);
    connect(ui->buttonAdd, &QPushButton::clicked, this, &addBooksDialog::addDir);
}

void addBooksDialog::browseDirs()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose Folder"),
        "/",
        QFileDialog::ShowDirsOnly |
        QFileDialog::DontResolveSymlinks);
    ui->textFolderPath->setText(dir);
}

void addBooksDialog::addDir()
{
    QString dirPath = ui->textFolderPath->text();
    QFileInfo dir(dirPath);
    if (dir.exists() && dir.isDir())
    {
        setupEntries(dirPath, ui->checkBoxRecursive->isChecked());
    }
    else
    {
        common::showMsgBox("Path Error!", "Directory path not valid!", ":/styles/style.qss", QMessageBox::Warning, ":/icons/books_icon.png");
    }
}

void addBooksDialog::setupEntries(const QString &dir, bool recursive)
{
    // Iterate over directory and get fileInfo and extensions
    QVector<QString> extVector;
    QVector<QFileInfo> entriesVector;
    QDirIterator iterator(dir, QDir::Files, recursive ? QDirIterator::Subdirectories : QDirIterator::NoIteratorFlags);
    while (iterator.hasNext())
    {
        QFileInfo file(iterator.next());
        entriesVector.push_back(file);

        QString ext = "." + file.suffix().toLower();
        if (!extVector.contains(ext))
        {
            extVector.push_back(ext);
        }
    }

    // Input for common tags, genres, or authors
    bulkDetailsDialog dialog;
    common::openDialog(&dialog, ":/styles/style.qss");

    QString tags = dialog.tags;
    QString genres = dialog.genre.isEmpty() ? "N/A" : dialog.genre;
    QString authors = dialog.author.isEmpty() ? "N/A" : dialog.author;

    // Let the user select desired extensions
    extSelectionDialog *extDialog = new extSelectionDialog(this, extVector, "Extensions", "Select Extensions");
    common::openDialog(extDialog, ":/styles/style.qss");

    QVector<QString> selectedExts = extDialog->getExtVector();
    iterateInsertEntries(entriesVector, selectedExts, tags, genres, authors);
}

void addBooksDialog::iterateInsertEntries(const QVector<QFileInfo> &entriesVector, const QVector<QString> &selectedExts, const QString &tags, const QString &genres, const QString &authors)
{
    queries::db.transaction();
    size_t count = entriesVector.size();
    double counter = 0;
    for (const QFileInfo &entry: entriesVector)
    {
        QString ext = "." + entry.suffix().toLower();
        quint32 progress = (counter / count) *100;
        ui->progressBar->setValue(progress);
        counter++;
        if (selectedExts.contains(ext))
        {
            insertBook(entry, tags, genres, authors);
        }
    }

    queries::db.commit();
    ui->progressBar->setValue(100);
}

void addBooksDialog::insertBook(const QFileInfo &entry, const QString &tags, const QString &genre, const QString &author)
{
    QString name = entry.completeBaseName();
    QString path = entry.absoluteFilePath();
    QString ext = "." + entry.suffix();
    QString folder = entry.dir().dirName();
    quint64 size = entry.size();
    quint32 pages = common::getPageCount(path);
    queries::insertBooksQuery(name, path, folder, ext, size, pages, tags, genre, author);
}
