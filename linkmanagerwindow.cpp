#include "linkmanagerwindow.h"
#include "ui_linkmanagerwindow.h"

#include "queries.h"
#include "common.h"

#include "insertlinkdialog.h"
#include "linkcollectiondialog.h"

#include <QUrl>
#include <QMenu>
#include <QFile>
#include <QClipboard>
#include <QCloseEvent>
#include <QListWidgetItem>
#include <QDesktopServices>
LinkManagerWindow::LinkManagerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LinkManagerWindow)
{
    ui->setupUi(this);
    queries::connectToDatabase();
    refreshCollections("");

    ui->listWidgetLinks->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listWidgetLinks, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showLinksContextMenu(QPoint)));

    ui->listWidgetCollections->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listWidgetCollections, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showCollectionsContextMenu(QPoint)));
}

LinkManagerWindow::~LinkManagerWindow()
{
    delete ui;
}

void LinkManagerWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
    delete this;
}

void LinkManagerWindow::showLinksContextMenu(const QPoint &pos)
{
    QPoint globalPos = ui->listWidgetLinks->mapToGlobal(pos);

    QMenu menu;
    menu.setStyleSheet(common::openSheet(":/style.qss"));

    menu.addAction("Open", this, [this]{on_listWidgetLinks_itemDoubleClicked(ui->listWidgetLinks->currentItem());});
    menu.addAction("Edit", this, [this]{editLinkDetails(ui->listWidgetLinks->currentItem()->text());});
    menu.addAction("Delete", this, [this]{deleteLink(ui->listWidgetLinks->currentItem()->text());});
    menu.addAction("Copy Link", this, [this]{copyLink(ui->listWidgetLinks->currentItem()->text());});

    if (!ui->listWidgetLinks->selectedItems().isEmpty())
    {menu.exec(globalPos);}
}

void LinkManagerWindow::showCollectionsContextMenu(const QPoint &pos)
{
    QPoint globalPos = ui->listWidgetCollections->mapToGlobal(pos);

    QMenu menu;
    menu.setStyleSheet(common::openSheet(":/style.qss"));

    menu.addAction("Rename", this, [this]{renameCollection(ui->listWidgetCollections->currentItem()->text());});
    menu.addAction("Delete", this, [this]{deleteCollection(ui->listWidgetCollections->currentItem()->text());});
    menu.addAction("Open Links", this, [this]{openAllLinks();});

    if (!ui->listWidgetCollections->selectedItems().isEmpty())
    {menu.exec(globalPos);}
}

void LinkManagerWindow::openAllLinks()
{
    QString currentColl = ui->listWidgetCollections->currentItem()->text();
    queries::selectLinksBasedOnCollection(currentColl, "");
    while(queries::query.next())
    {
        QDesktopServices::openUrl(QUrl(queries::query.value(1).toString()));
    }
}

void LinkManagerWindow::deleteLink(QString name)
{
    queries::deleteLink(name);
    delete ui->listWidgetLinks->currentItem();
    ui->statusBar->showMessage("Link \"" + name + "\" deleted.");
}

void LinkManagerWindow::editLinkDetails(QString name)
{
    insertLinkDialog dialog(this);
    dialog.title = name;
    queries::selectLinkRecord(name);
    queries::query.next();
    dialog.link = queries::query.value(1).toString();
    dialog.setDataOnOpen();
    common::openDialog(&dialog, ":/style.qss");

    queries::updateLinkDetails(name, dialog.title, dialog.link);
    ui->listWidgetLinks->currentItem()->setText(dialog.title);
    ui->statusBar->showMessage("Link \"" + name + "\" edited.");
}

void LinkManagerWindow::copyLink(QString name)
{
    queries::selectLinkRecord(name);
    queries::query.next();

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(queries::query.value(1).toString());
    ui->statusBar->showMessage("Link \"" + name + "\" copied.");
}

void LinkManagerWindow::renameCollection(QString name)
{
    linkCollectionDialog dialog(this);

    dialog.collectionName = name;
    dialog.setDataOnOpen();

    common::openDialog(&dialog, ":/style.qss");
    queries::updateLinkCollectionName(name, dialog.collectionName);
    ui->listWidgetCollections->currentItem()->setText(dialog.collectionName);
    ui->statusBar->showMessage("Collection \"" + name + "\" renamed.");
}

void LinkManagerWindow::deleteCollection(QString name)
{
    queries::deleteCollection(name);
    ui->listWidgetLinks->clear();
    delete ui->listWidgetCollections->currentItem();
    ui->statusBar->showMessage("Collection \"" + name + "\" deleted.");
}

void LinkManagerWindow::refreshCollections(QString searchString)
{
    ui->listWidgetCollections->clear();
    queries::selectCollections(searchString);
    while(queries::query.next())
    {
        ui->listWidgetCollections->addItem(queries::query.value(0).toString());
    }
}

void LinkManagerWindow::refreshLinks(QString collectionName, QString searchString)
{
    ui->listWidgetLinks->clear();
    queries::selectLinksBasedOnCollection(collectionName, searchString);
    while(queries::query.next())
    {
        ui->listWidgetLinks->addItem(queries::query.value(0).toString());
    }
}

void LinkManagerWindow::on_buttonInsertCollection_clicked()
{
    linkCollectionDialog dialog(this);
    common::openDialog(&dialog, ":/style.qss");

    if (!dialog.collectionName.isEmpty())
    {
        queries::insertLinkCollection(dialog.collectionName);
        ui->statusBar->showMessage("New collection \"" + dialog.collectionName + "\" added.");
    }
    refreshCollections("");
}

void LinkManagerWindow::on_buttonInsertLink_clicked()
{
    insertLinkDialog dialog(this);
    if (ui->listWidgetCollections->selectedItems().count() != 0)
    {
        common::openDialog(&dialog, ":/style.qss");
    }

    if (!dialog.link.isEmpty() && !dialog.title.isEmpty())
    {
        QString collectionName = ui->listWidgetCollections->currentItem()->text();
        int collectionId = queries::selectCollectionId(collectionName);
        queries::insertLink(collectionId, dialog.title, dialog.link);
        refreshLinks(ui->listWidgetCollections->currentItem()->text(), "");
        ui->statusBar->showMessage("New link \"" + dialog.title + "\" added.");

    }
}

void LinkManagerWindow::on_textCollections_textChanged(const QString &arg1)
{
    refreshCollections(arg1);
}

void LinkManagerWindow::on_textLinks_textChanged(const QString &arg1)
{
    if (ui->listWidgetCollections->selectedItems().count() !=0 )
        refreshLinks(ui->listWidgetCollections->currentItem()->text(), arg1);
}

void LinkManagerWindow::on_listWidgetCollections_itemClicked(QListWidgetItem *item)
{
    refreshLinks(item->text(), "");
    ui->statusBar->showMessage("Collection \"" + item->text() + "\" selected.");
}

void LinkManagerWindow::on_listWidgetLinks_itemDoubleClicked(QListWidgetItem *item)
{
    queries::selectLinkRecord(item->text());
    queries::query.next();

    QDesktopServices::openUrl(QUrl(queries::query.value(1).toString()));
}

void LinkManagerWindow::on_listWidgetLinks_itemClicked(QListWidgetItem *item)
{
    queries::selectLinkRecord(item->text());
    queries::query.next();

    ui->statusBar->showMessage(queries::query.value(0).toString() + ": " + queries::query.value(1).toString());
}

