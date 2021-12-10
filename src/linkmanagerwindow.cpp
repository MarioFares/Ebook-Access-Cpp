#include "include/linkmanagerwindow.h"
#include "ui_linkmanagerwindow.h"

#include "include/queries.h"
#include "include/common.h"

#include "include/insertlinkdialog.h"
#include "include/linkcollectiondialog.h"

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

    this->LINKS_SORT = Qt::AscendingOrder;
    this->COLL_SORT = Qt::AscendingOrder;

    ui->listWidgetLinks->setSortingEnabled(true);
    ui->listWidgetCollections->setSortingEnabled(true);

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
    menu.setStyleSheet(common::openSheet(":/styles/style.qss"));
    menu.addAction("Open", this, [this]{on_listWidgetLinks_itemDoubleClicked(ui->listWidgetLinks->currentItem());});
    menu.addAction("Edit", this, [this]{editLinkDetails(ui->listWidgetLinks->currentItem()->text());});
    menu.addAction("Delete", this, [this]{deleteLink(ui->listWidgetLinks->currentItem()->text());});
    menu.addAction("Copy Link", this, [this]{copyLink(ui->listWidgetLinks->currentItem()->text());});
    if (!ui->listWidgetLinks->selectedItems().isEmpty())
    {
        menu.exec(globalPos);
    }
}

void LinkManagerWindow::showCollectionsContextMenu(const QPoint &pos)
{
    QPoint globalPos = ui->listWidgetCollections->mapToGlobal(pos);
    QMenu menu;
    menu.setStyleSheet(common::openSheet(":/styles/style.qss"));
    menu.addAction("Add Link", this, [this]{on_buttonInsertLink_clicked();});
    menu.addAction("Rename", this, [this]{renameCollection(ui->listWidgetCollections->currentItem()->text());});
    menu.addAction("Delete", this, [this]{deleteCollection(ui->listWidgetCollections->currentItem()->text());});
    menu.addAction("Open Links", this, [this]{openAllLinks();});
    if (!ui->listWidgetCollections->selectedItems().isEmpty())
    {
        menu.exec(globalPos);
    }
}

void LinkManagerWindow::openAllLinks()
{
    QString currentColl = ui->listWidgetCollections->currentItem()->text();
    queries::selectLinksBasedOnCollection(currentColl, "");
    while(queries::query.next())
    {
        QUrl url = QUrl(queries::query.value(1).toString());
        if (url.isValid())
        {
            QDesktopServices::openUrl(url);
        }
    }
}

void LinkManagerWindow::deleteLink(const QString &name)
{
    queries::deleteLink(name);
    delete ui->listWidgetLinks->currentItem();
    ui->statusBar->showMessage("Link \"" + name + "\" deleted.");
}

void LinkManagerWindow::editLinkDetails(const QString &name)
{
    insertLinkDialog dialog(this);
    dialog.setWindowTitle("Edit Link");
    dialog.title = name;
    queries::selectLinkRecord(name);
    queries::query.next();
    dialog.link = queries::query.value(1).toString();
    dialog.setDataOnOpen();
    common::openDialog(&dialog, ":/styles/style.qss");

    queries::updateLinkDetails(name, dialog.title, dialog.link);
    ui->listWidgetLinks->currentItem()->setText(dialog.title);
    ui->statusBar->showMessage("Link \"" + name + "\" edited.");
}

void LinkManagerWindow::copyLink(const QString &name)
{
    queries::selectLinkRecord(name);
    queries::query.next();

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(queries::query.value(1).toString());
    ui->statusBar->showMessage("Link \"" + name + "\" copied.");
}

void LinkManagerWindow::renameCollection(const QString &name)
{
    linkCollectionDialog dialog(this);

    dialog.collectionName = name;
    dialog.setDataOnOpen();

    common::openDialog(&dialog, ":/styles/style.qss");
    queries::updateLinkCollectionName(name, dialog.collectionName);
    ui->listWidgetCollections->currentItem()->setText(dialog.collectionName);
    ui->statusBar->showMessage("Collection \"" + name + "\" renamed.");
}

void LinkManagerWindow::deleteCollection(const QString &name)
{
    queries::deleteCollection(name);
    ui->listWidgetLinks->clear();
    delete ui->listWidgetCollections->currentItem();
    ui->statusBar->showMessage("Collection \"" + name + "\" deleted.");
}

void LinkManagerWindow::refreshCollections(const QString &searchString)
{
    ui->listWidgetCollections->clear();
    queries::selectCollections(searchString);
    while(queries::query.next())
    {
        ui->listWidgetCollections->addItem(queries::query.value(0).toString());
    }
}

void LinkManagerWindow::refreshLinks(const QString &collectionName, const QString &searchString)
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
    common::openDialog(&dialog, ":/styles/style.qss");

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
    if (!ui->listWidgetCollections->selectedItems().isEmpty())
    {
        common::openDialog(&dialog, ":/styles/style.qss");
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
    if (!ui->listWidgetCollections->selectedItems().isEmpty())
    {
        refreshLinks(ui->listWidgetCollections->currentItem()->text(), arg1);
    }
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
    QString title = queries::query.value(0).toString();
    QString url = queries::query.value(1).toString();

    ui->textDetailsTitle->setText(title);
    ui->textDetailsUrl->setText(url);

    ui->statusBar->showMessage(title + ": " + url);
}

void LinkManagerWindow::on_buttonDetailsClear_clicked()
{
    ui->textDetailsTitle->clear();
    ui->textDetailsUrl->clear();
}


void LinkManagerWindow::on_buttonDetailsRestore_clicked()
{
    if (!ui->listWidgetLinks->selectedItems().isEmpty())
    {
        on_listWidgetLinks_itemClicked(ui->listWidgetLinks->currentItem());
        ui->statusBar->showMessage("Details restored successfully.");
    }
}


void LinkManagerWindow::on_buttonDetailsUpdate_clicked()
{
    QString oldTitle = ui->listWidgetLinks->currentItem()->text();
    QString newTitle = ui->textDetailsTitle->text();
    QString newUrl = ui->textDetailsUrl->text();

    queries::updateLinkDetails(oldTitle, newTitle, newUrl);
    ui->listWidgetLinks->currentItem()->setText(newTitle);
    ui->statusBar->showMessage("Link \"" + oldTitle + "\" edited.");
}


void LinkManagerWindow::on_listWidgetLinks_itemSelectionChanged()
{
    if (!ui->listWidgetLinks->selectedItems().isEmpty())
    {
        on_listWidgetLinks_itemClicked(ui->listWidgetLinks->currentItem());
    }
}


void LinkManagerWindow::on_buttonSearchCollections_clicked()
{
    on_textCollections_textChanged(ui->textCollections->text());
}


void LinkManagerWindow::on_buttonClearCollections_clicked()
{
    ui->textCollections->clear();
    ui->listWidgetCollections->clear();
}


void LinkManagerWindow::on_buttonSortCollections_clicked()
{
    this->COLL_SORT = (this->COLL_SORT == Qt::AscendingOrder ? Qt::DescendingOrder : Qt::AscendingOrder);
    ui->listWidgetCollections->sortItems(this->COLL_SORT);
}


void LinkManagerWindow::on_buttonSearchLinks_clicked()
{
    on_textLinks_textChanged(ui->textLinks->text());
}


void LinkManagerWindow::on_buttonClearLinks_clicked()
{
    ui->textLinks->clear();
    ui->listWidgetLinks->clear();
}


void LinkManagerWindow::on_buttonSortLinks_clicked()
{
    this->LINKS_SORT = (this->LINKS_SORT == Qt::AscendingOrder ? Qt::DescendingOrder : Qt::AscendingOrder);
    ui->listWidgetLinks->sortItems(this->LINKS_SORT);
}

void LinkManagerWindow::on_actionFullscreen_triggered()
{
    !this->isFullScreen() ? this->showFullScreen() : this->showNormal();
}

void LinkManagerWindow::on_actionHideLeftPanel_triggered()
{
    ui->frameLeft->setHidden(!ui->frameLeft->isHidden());
    ui->actionHideLeftPanel->setText(ui->frameLeft->isHidden() ? "Show Left Panel" : "Hide Left Panel");
}


void LinkManagerWindow::on_actionHideRightPanel_triggered()
{
    ui->frameRight->setHidden(!ui->frameRight->isHidden());
    ui->actionHideRightPanel->setText(ui->frameRight->isHidden() ? "Show Right Panel" : "Hide Right Panel");
}


void LinkManagerWindow::on_actionHideSearchBars_triggered()
{
    ui->frameLinksSearch->setHidden(!ui->frameLinksSearch->isHidden());
    ui->frameCollectionsSearch->setHidden(!ui->frameCollectionsSearch->isHidden());
    ui->actionHideSearchBars->setText(ui->frameCollectionsSearch->isHidden() ? "Show Search Bars" : "Hide Search Bars");
}


void LinkManagerWindow::on_actionResetLinks_triggered()
{
    queries::resetLinksTable();
}


void LinkManagerWindow::on_actionResetCollections_triggered()
{
    queries::resetCollectionsTable();
}


void LinkManagerWindow::on_actionResetAll_triggered()
{
    queries::resetLinksTable();
    queries::resetCollectionsTable();
}


void LinkManagerWindow::on_actionAddLink_triggered()
{
    on_buttonInsertLink_clicked();
}


void LinkManagerWindow::on_actionEditLink_triggered()
{
    if (ui->listWidgetLinks->currentItem())
    {
        editLinkDetails(ui->listWidgetLinks->currentItem()->text());
    }
}

void LinkManagerWindow::on_actionAddCollection_triggered()
{
    on_buttonInsertCollection_clicked();
}


void LinkManagerWindow::on_actionDeleteCollection_triggered()
{
    if (ui->listWidgetCollections->currentItem())
    {
        deleteCollection(ui->listWidgetCollections->currentItem()->text());
    }
}


void LinkManagerWindow::on_actionRenameCollection_triggered()
{
    if (ui->listWidgetCollections->currentItem())
    {
        renameCollection(ui->listWidgetCollections->currentItem()->text());
    }
}


void LinkManagerWindow::on_actionOpenAllLinks_triggered()
{
    openAllLinks();
}


void LinkManagerWindow::on_actionDeleteLink_triggered()
{
    if (ui->listWidgetLinks->currentItem())
    {
        deleteLink(ui->listWidgetLinks->currentItem()->text());
    }
}

