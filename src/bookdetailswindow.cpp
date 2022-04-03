#include "ui_bookdetailswindow.h"

#include "include/queries.h"
#include "include/bookdetailswindow.h"

BookDetailsWindow::BookDetailsWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BookDetailsWindow)
{
    ui->setupUi(this);
    ui->statusbar->setHidden(true);
    queries::connectToDatabase();
    ui->listWidget->setSortingEnabled(true);

    searchEbooks("");
    setupConnections();
}

BookDetailsWindow::~BookDetailsWindow()
{
    delete ui;
}

void BookDetailsWindow::setupConnections()
{
    connect(ui->textSearch, &QLineEdit::textChanged, this, &BookDetailsWindow::searchEbooks);
    connect(ui->listWidget, &QListWidget::itemClicked, [this]
    {
        showBookDetails(ui->listWidget->currentItem()->text());
    });
    connect(ui->listWidget, &QListWidget::currentItemChanged, [this]
    {
        showBookDetails(ui->listWidget->currentItem()->text());
    });
}

void BookDetailsWindow::searchEbooks(const QString &name)
{
    ui->listWidget->clear();
    queries::selectNameBasedOnString(name);
    while (queries::query.next())
    {
        ui->listWidget->addItem(queries::query.value(0).toString());
    }
}

void BookDetailsWindow::showBookDetails(const QString &name)
{
    queries::selectAllBasedonName(name);
    while (queries::query.next())
    {
        ui->textName->setText(queries::query.value("name").toString());
        ui->textAuthor->setText(queries::query.value("author").toString());
        ui->textGenre->setText(queries::query.value("genre").toString());
        ui->textPages->setText(queries::query.value("pages").toString());
        ui->textPath->setText(queries::query.value("path").toString());
        ui->textSize->setText(queries::query.value("size").toString());
        ui->textExt->setText(queries::query.value("ext").toString());
        ui->textFolder->setText(queries::query.value("folder").toString());
        ui->textDateAdded->setText(queries::query.value("time_added").toString());
        ui->textDateModified->setText(queries::query.value("last_modified").toString());

        ui->textName->setCursorPosition(0);
        ui->textPath->setCursorPosition(0);
        ui->textFolder->setCursorPosition(0);

        setWindowTitle("MetaData - " + queries::query.value("name").toString());
    }
    QString tags = queries::selectTagsBasedOnName(name);
    ui->textTags->setText(tags);
}
