#include "include/common.h"
#include "include/queries.h"
#include "include/dialogs/addbooksdialog.h"
#include "include/dialogs/bulkdetailsdialog.h"
#include "include/dialogs/extselectiondialog.h"

#include <QFileInfo>
#include <QFileDialog>
#include <QPushButton>
#include <QDirIterator>
#include <QProgressBar>

AddBooksDialog::AddBooksDialog(QWidget *parent) : QDialog(parent) {
    setupInterface();
    setupConnections();
    setupTabOrder();
}

void AddBooksDialog::setupInterface() {
    // Window
    setWindowTitle("Add Books");
    setFixedSize(405, 300);

    // Widgets
    _buttonAdd = new QPushButton("Add");
    _buttonAdd->setCursor(Qt::PointingHandCursor);
    _buttonAdd->setFlat(true);
    _buttonAdd->setMinimumSize(80, 25);

    _buttonClose = new QPushButton("Close");
    _buttonClose->setCursor(Qt::PointingHandCursor);
    _buttonClose->setFlat(true);
    _buttonClose->setMinimumSize(80, 25);

    _buttonBrowseFolders = new QPushButton("...");
    _buttonBrowseFolders->setCursor(Qt::PointingHandCursor);
    _buttonBrowseFolders->setFlat(true);
    _buttonBrowseFolders->setMinimumSize(50, 20);

    _checkBoxRecursive = new QCheckBox("Recursive");
    _checkBoxRecursive->setCheckable(true);
    _checkBoxRecursive->setChecked(false);

    _textFolderPath = new QLineEdit();
    _textFolderPath->setCompleter(common::dirCompleter(this));
    _textFolderPath->setClearButtonEnabled(true);

    _labelAddBooks = new QLabel("Add Books");
    _labelFolder = new QLabel("Folder: ");

    _progressBar = new QProgressBar();
    _progressBar->setValue(0);
    _progressBar->setMinimumSize(381, 23);

    // Layouts
    _horLayTitle = new QHBoxLayout();
    _horSpacerTitleLeft = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
    _horSpacerTitleRight = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
    _horLayTitle->addSpacerItem(_horSpacerTitleLeft);
    _horLayTitle->addWidget(_labelAddBooks);
    _horLayTitle->addSpacerItem(_horSpacerTitleRight);

    _horLayFolder = new QHBoxLayout();
    _horLayFolder->addWidget(_labelFolder);
    _horLayFolder->addWidget(_textFolderPath);
    _horLayFolder->addWidget(_buttonBrowseFolders);

    _horLayCheckBox = new QHBoxLayout();
    _horSpacerCheckBoxRight = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
    _horLayCheckBox->addWidget(_checkBoxRecursive);
    _horLayCheckBox->addSpacerItem(_horSpacerCheckBoxRight);

    _horLayProgressBar = new QHBoxLayout();
    _horSpacerProgressBarLeft = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
    _horSpacerProgressBarRight = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
    _horLayProgressBar->addSpacerItem(_horSpacerProgressBarLeft);
    _horLayProgressBar->addWidget(_progressBar);
    _horLayProgressBar->addSpacerItem(_horSpacerProgressBarRight);

    _horLayButtons = new QHBoxLayout();
    _horSpacerButtonsLeft = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
    _horSpacerButtonsRight = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
    _horLayButtons->addSpacerItem(_horSpacerButtonsLeft);
    _horLayButtons->addWidget(_buttonAdd);
    _horLayButtons->addWidget(_buttonClose);
    _horLayButtons->addSpacerItem(_horSpacerButtonsRight);

    _vertLayMain = new QVBoxLayout();
    _vertLayMain->addLayout(_horLayTitle);
    _vertLayMain->addLayout(_horLayFolder);
    _vertLayMain->addLayout(_horLayCheckBox);
    _vertLayMain->addLayout(_horLayProgressBar);
    _vertLayMain->addLayout(_horLayButtons);

    setLayout(_vertLayMain);
}

void AddBooksDialog::setupConnections() {
    connect(_buttonClose, &QPushButton::clicked, this, &AddBooksDialog::close);
    connect(_buttonBrowseFolders, &QPushButton::clicked, this, &AddBooksDialog::browseDirs);
    connect(_buttonAdd, &QPushButton::clicked, this, &AddBooksDialog::addDir);
}

void AddBooksDialog::setupTabOrder() {
    QWidget::setTabOrder(_textFolderPath, _buttonBrowseFolders);
    QWidget::setTabOrder(_buttonBrowseFolders, _checkBoxRecursive);
    QWidget::setTabOrder(_checkBoxRecursive, _buttonAdd);
    QWidget::setTabOrder(_buttonAdd, _buttonClose);
}

void AddBooksDialog::browseDirs() {
    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose Folder"),
                                                    "/",
                                                    QFileDialog::ShowDirsOnly |
                                                    QFileDialog::DontResolveSymlinks);
    _textFolderPath->setText(dir);
}

void AddBooksDialog::addDir() {
    QString dirPath = _textFolderPath->text();
    QFileInfo dir(dirPath);
    if (dir.exists() && dir.isDir()) {
        setupEntries(dirPath, _checkBoxRecursive->isChecked());
    } else {
        common::showMsgBox("Path Error!", "Directory path not valid!", ":/styles/style.qss", QMessageBox::Warning,
                           ":/icons/books_icon.png");
    }
}

void AddBooksDialog::setupEntries(const QString &dir, bool recursive) {
    // Iterate over directory and get fileInfo and extensions
    QVector<QString> extVector;
    QVector<QFileInfo> entriesVector;
    QDirIterator iterator(dir, QDir::Files, recursive ? QDirIterator::Subdirectories : QDirIterator::NoIteratorFlags);
    while (iterator.hasNext()) {
        QFileInfo file(iterator.next());
        entriesVector.push_back(file);

        QString ext = "." + file.suffix().toLower();
        if (!extVector.contains(ext)) {
            extVector.push_back(ext);
        }
    }

    // Input for common tags, genres, or authors
    BulkDetailsDialog dialog;
    common::openDialog(&dialog, ":/styles/style.qss");

    QString tags = dialog._tags;
    QString genres = dialog._genre.isEmpty() ? "N/A" : dialog._genre;
    QString authors = dialog._author.isEmpty() ? "N/A" : dialog._author;
    QString publisher = dialog._publisher.isEmpty() ? "N/A" : dialog._publisher;
    QString datePublished = dialog._datePublished.isEmpty() ? "N/A" : dialog._datePublished;
    QString series = dialog._series.isEmpty() ? "N/A" : dialog._series;
    quint32 rating = dialog._rating;
    quint32 status = dialog._status;

    // Let the user select desired extensions
    ExtSelectionDialog *extDialog = new ExtSelectionDialog(this, extVector, "Extensions", "Select Extensions");
    common::openDialog(extDialog, ":/styles/style.qss");

    QVector<QString> selectedExts = extDialog->getExtVector();
    iterateInsertEntries(entriesVector, selectedExts, tags, genres, authors, publisher, datePublished, series, rating,
                         status);
}

void AddBooksDialog::iterateInsertEntries(const QVector<QFileInfo> &entriesVector, const QVector<QString> &selectedExts,
                                          const QString &tags, const QString &genres, const QString &authors,
                                          const QString &publisher, const QString &datePublished, const QString &series,
                                          quint32 rating, quint32 status) {
    queries::db.transaction();
    size_t count = entriesVector.size();
    double counter = 0;
    for (const QFileInfo &entry: entriesVector) {
        QString ext = "." + entry.suffix().toLower();
        quint32 progress = (counter / count) * 100;
        _progressBar->setValue(progress);
        counter++;
        if (selectedExts.contains(ext)) {
            insertBook(entry, tags, genres, authors, publisher, datePublished, series, rating, status);
        }
    }

    queries::db.commit();
    _progressBar->setValue(100);
}

void AddBooksDialog::insertBook(const QFileInfo &entry, const QString &tags, const QString &genre,
                                const QString &author, const QString &publisher, const QString &datePublished,
                                const QString &series, quint32 rating, quint32 status) {
    QString name = entry.completeBaseName();
    QString path = entry.absoluteFilePath();
    QString ext = "." + entry.suffix();
    QString folder = entry.dir().dirName();
    quint64 size = entry.size();
    quint32 pages = common::getPageCount(path);
    queries::insertBooksQuery(name, path, folder, ext, size, pages, tags, genre, author, publisher, datePublished,
                              series, rating, status);
}
