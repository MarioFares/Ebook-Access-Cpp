#include "addbookdialog.h"
#include "ui_addbookdialog.h"

#include <filesystem>
#include <queries.h>
#include <common.h>
#include <iostream>
#include <QFile>
#include <QProcess>
#include <QFileDialog>

namespace fs = std::filesystem;

addBookDialog::addBookDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addBookDialog)
{
    ui->setupUi(this);
}

addBookDialog::~addBookDialog()
{
    delete ui;
}

void addBookDialog::on_buttonCancel_clicked()
{
    close();
}


void addBookDialog::on_buttonClear_clicked()
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


void addBookDialog::on_buttonBrowse_clicked()
{
    fs::path filePath = QFileDialog::getOpenFileName(this,
                                                     tr("Open File"), "/", tr("Ebook Files (*.pdf *.epub *.mobi *.cbr)")).toStdString();


    fs::directory_entry dirEntry(filePath);

    ui->textName->setText(QString::fromStdString(dirEntry.path().stem().string()));
    ui->textExtension->setText(QString::fromStdString(dirEntry.path().extension().string()));
    ui->textPath->setText(QString::fromStdString(dirEntry.path().string()));
    ui->textFolder->setText(QString::fromStdString(filePath.parent_path().filename().string()));
    ui->textSize->setText(QString::number(dirEntry.file_size()));

    int pages = common::getPageCount(QString::fromStdString(dirEntry.path().string()));

    ui->textPages->setText(QString::number(pages));
//    QDirIterator iterator("E:\\Personal\\Archives\\Ebook Archive", {"*.pdf", "*.cbr", "*.epub", "*.mobi"}, QDir::Files, QDirIterator::Subdirectories);
//    int counter = 0;
//    while (iterator.hasNext())
//    {
//        counter++;
//        std::cout << iterator.next().toStdString() << std::endl;
//        std::cout << counter << std::endl;
//    }
}


void addBookDialog::on_buttonAdd_clicked()
{
    QString name = ui->textName->text();
    QString folder = ui->textFolder->text();
    QString genre = ui->textGenre->text();
    QString path = ui->textPath->text();
    QString author = ui->textAuthor->text();
    long pages = ui->textPages->text().toLong();
    long long size = ui->textSize->text().toLongLong();
    QString ext = ui->textExtension->text();
    QString tags = ui->textTags->text();

    if(fs::exists(fs::path(path.toStdString())))
    {
        queries::insertBooksQuery(name, path, folder, ext, size, pages, tags, genre, author);
        common::showMsgBox("Success!", "Ebook successfully added.", ":/summarystyle.qss", QMessageBox::Information, ":/icons/books_icon.png");
    }
}

