#include "ui_addbookdialog.h"

#include "include/addbookdialog.h"
#include "include/queries.h"
#include "include/common.h"

#include <QFileDialog>

addBookDialog::addBookDialog(QWidget *parent):
    QDialog(parent), ui(new Ui::addBookDialog)
{
    ui->setupUi(this);
    setupConnections();
    ui->textPath->setCompleter(common::dirCompleter(this));
}

addBookDialog::~addBookDialog()
{
    delete ui;
}

void addBookDialog::setupConnections()
{
    connect(ui->buttonClear, &QPushButton::clicked, this, &addBookDialog::clearInputs);
    connect(ui->buttonBrowse, &QPushButton::clicked, this, &addBookDialog::browseFile);
    connect(ui->buttonCancel, &QPushButton::clicked, this, &addBookDialog::close);
    connect(ui->buttonAdd, &QPushButton::clicked, this, &addBookDialog::addFile);
}

void addBookDialog::clearInputs()
{
    ui->textName->clear();
    ui->textFolder->clear();
    ui->textAuthor->clear();
    ui->textGenre->clear();
    ui->textTags->clear();
    ui->textSize->clear();
    ui->textExtension->clear();
    ui->textPath->clear();
    ui->textPages->clear();
}

void addBookDialog::browseFile()
{
    QString filePath = QFileDialog::getOpenFileName(this,
        tr("Open File"), "/", tr("All Files (*.*)"));

    QFileInfo file(filePath);
    ui->textName->setText(file.completeBaseName());
    ui->textExtension->setText("." + file.suffix());
    ui->textFolder->setText(file.dir().dirName());
    ui->textPath->setText(file.absoluteFilePath());
    ui->textSize->setText(QString::number(file.size()));

    quint32 pages = common::getPageCount(filePath);
    ui->textPages->setText(QString::number(pages));
}

void addBookDialog::addFile()
{
    QString name = ui->textName->text();
    QString folder = ui->textFolder->text();
    QString genre = ui->textGenre->text();
    QString path = ui->textPath->text();
    QString author = ui->textAuthor->text();
    quint32 pages = ui->textPages->text().toUInt();
    quint64 size = ui->textSize->text().toULongLong();
    QString ext = ui->textExtension->text();
    QString tags = ui->textTags->text();

    QFileInfo file(path);
    if (file.exists() && file.isFile())
    {
        queries::insertBooksQuery(name, path, folder, ext, size,
            pages, tags, genre, author);
        common::showMsgBox("Success!", "Ebook successfully added.",
            ":/styles/summarystyle.qss", QMessageBox::Information,
            ":/icons/books_icon.png");
    }
    else
    {
        common::showMsgBox("Path Error!", "File path not valid!", ":/styles/style.qss", QMessageBox::Warning, ":/icons/books_icon.png");
    }
}
