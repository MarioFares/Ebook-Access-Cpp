#include "ui_linkmanagerwindow.h"

#include "include/common.h"
#include "include/queries.h"
#include "include/getnamedialog.h"
#include "include/insertlinkdialog.h"
#include "include/linkmanagerwindow.h"

#include <QUrl>
#include <QMenu>
#include <QFile>
#include <QClipboard>
#include <QCloseEvent>
#include <QListWidgetItem>
#include <QDesktopServices>

LinkManagerWindow::LinkManagerWindow(QWidget *parent):
    QMainWindow(parent), ui(new Ui::LinkManagerWindow)
{
    ui->setupUi(this);
    queries::connectToDatabase();
    refreshCollections("");

    this->LINKS_SORT = Qt::AscendingOrder;
    this->COLL_SORT = Qt::AscendingOrder;

    ui->listWidgetLinks->setSortingEnabled(true);
    ui->listWidgetCollections->setSortingEnabled(true);

    ui->listWidgetLinks->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->listWidgetCollections->setContextMenuPolicy(Qt::CustomContextMenu);

    setupConnections();
}

LinkManagerWindow::~LinkManagerWindow()
{
    delete ui;
}

void LinkManagerWindow::setupConnections()
{
    connect(ui->textLinks, &QLineEdit::textChanged, this, &LinkManagerWindow::searchLinks);
    connect(ui->textLinks, &QLineEdit::returnPressed, [this] { searchLinks(ui->textLinks->text()); });
    connect(ui->textCollections, &QLineEdit::textChanged, this, &LinkManagerWindow::refreshCollections);
    connect(ui->textCollections, &QLineEdit::returnPressed, [this] { refreshCollections(ui->textCollections->text()); });

    connect(ui->listWidgetLinks, &QListWidget::itemActivated, this, &LinkManagerWindow::openLink);
    connect(ui->listWidgetLinks, &QListWidget::itemClicked, this, &LinkManagerWindow::linkClicked);
    connect(ui->listWidgetLinks, &QListWidget::itemSelectionChanged, this, &LinkManagerWindow::linkSelectionChanged);
    connect(ui->listWidgetLinks, &QListWidget::customContextMenuRequested, this, &LinkManagerWindow::showLinksContextMenu);
    connect(ui->listWidgetCollections, &QListWidget::itemClicked, this, &LinkManagerWindow::collectionClicked);
    connect(ui->listWidgetCollections, &QListWidget::customContextMenuRequested, this, &LinkManagerWindow::showCollectionsContextMenu);


    connect(ui->buttonDetailsClear, &QPushButton::clicked, this, &LinkManagerWindow::clearDetails);
    connect(ui->buttonDetailsRestore, &QPushButton::clicked, this, &LinkManagerWindow::restoreDetails);
    connect(ui->buttonDetailsUpdate, &QPushButton::clicked, this, &LinkManagerWindow::updateDetails);

    connect(ui->buttonAddCollection, &QToolButton::clicked, this, &LinkManagerWindow::addCollection);
    connect(ui->buttonAddLink, &QToolButton::clicked, this, &LinkManagerWindow::addLink);
    connect(ui->buttonClearCollections, &QToolButton::clicked, this, &LinkManagerWindow::clearCollections);
    connect(ui->buttonSortCollections, &QToolButton::clicked, this, &LinkManagerWindow::sortCollections);
    connect(ui->buttonClearLinks, &QToolButton::clicked, this, &LinkManagerWindow::clearLinks);
    connect(ui->buttonSortLinks, &QToolButton::clicked, this, &LinkManagerWindow::sortLinks);
    connect(ui->buttonSearchCollections, &QToolButton::clicked, [this]
    {
        refreshCollections(ui->textCollections->text());
    });
    connect(ui->buttonSearchLinks, &QToolButton::clicked, [this]
    {
        searchLinks(ui->textLinks->text());
    });

    connect(ui->actionFullscreen, &QAction::triggered, [this] { common::toggleFullscreen(this); });
    connect(ui->actionHideLeftPanel, &QAction::triggered, this, &LinkManagerWindow::hideLeftPanel);
    connect(ui->actionHideRightPanel, &QAction::triggered, this, &LinkManagerWindow::hideRightPanel);
    connect(ui->actionHideSearchBars, &QAction::triggered, this, &LinkManagerWindow::hideSearchBars);
    connect(ui->actionResetLinks, &QAction::triggered, this, &LinkManagerWindow::resetLinks);
    connect(ui->actionResetCollections, &QAction::triggered, this, &LinkManagerWindow::resetCollections);
    connect(ui->actionDeleteCollection, &QAction::triggered, this, &LinkManagerWindow::actionDeleteCollection);
    connect(ui->actionRenameCollection, &QAction::triggered, this, &LinkManagerWindow::actionRenameCollection);
    connect(ui->actionOpenAllLinks, &QAction::triggered, this, &LinkManagerWindow::actionOpenAllLinks);
    connect(ui->actionDeleteLink, &QAction::triggered, this, &LinkManagerWindow::actionDeleteLink);
    connect(ui->actionEditLink, &QAction::triggered, this, &LinkManagerWindow::actionEditLink);
    connect(ui->actionClose, &QAction::triggered, this, &LinkManagerWindow::close);
    connect(ui->actionMaximize, &QAction::triggered, [this] { common::toggleMaximized(this); });
    connect(ui->actionMinimize, &QAction::triggered, this, &LinkManagerWindow::showMinimized);
    connect(ui->actionAddCollection, &QAction::triggered, this, &LinkManagerWindow::addCollection);
    connect(ui->actionAddLink, &QAction::triggered, this, &LinkManagerWindow::addLink);
    connect(ui->actionHideStatusBar, &QAction::triggered, this, &LinkManagerWindow::hideStatusBar);
}

void LinkManagerWindow::showLinksContextMenu(const QPoint &pos)
{
    QPoint globalPos = ui->listWidgetLinks->mapToGlobal(pos);

    QMenu menu;
    menu.setStyleSheet(common::openSheet(":/styles/style.qss"));
    menu.addAction("Open", this, [this]
    {
        openLink(ui->listWidgetLinks->currentItem());
    });
    menu.addAction("Edit", this, [this]
    {
        editLinkDetails(ui->listWidgetLinks->currentItem()->text());
    });
    menu.addAction("Delete", this, [this]
    {
        deleteLink(ui->listWidgetLinks->currentItem()->text());
    });
    menu.addAction("Copy Link", this, [this]
    {
        copyLink(ui->listWidgetLinks->currentItem()->text());
    });
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
    menu.addAction("Add Link", this, [this]
    {
        addLink();
    });
    menu.addAction("Rename", this, [this]
    {
        renameCollection(ui->listWidgetCollections->currentItem()->text());
    });
    menu.addAction("Delete", this, [this]
    {
        deleteCollection(ui->listWidgetCollections->currentItem()->text());
    });
    menu.addAction("Open Links", this, [this]
    {
        openAllLinks();
    });
    if (!ui->listWidgetCollections->selectedItems().isEmpty())
    {
        menu.exec(globalPos);
    }
}

void LinkManagerWindow::openAllLinks()
{
    QString currentColl = ui->listWidgetCollections->currentItem()->text();
    queries::selectLinksBasedOnCollection(currentColl, "");
    while (queries::query.next())
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
    InsertLinkDialog dialog(this);
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
    GetNameDialog dialog(this, "Collection Name", "Please provide a name for the collection you want to create:");

    dialog.name = name;
    dialog.setDataOnOpen();

    common::openDialog(&dialog, ":/styles/style.qss");
    queries::updateLinkCollectionName(name, dialog.name);
    ui->listWidgetCollections->currentItem()->setText(dialog.name);
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
    while (queries::query.next())
    {
        ui->listWidgetCollections->addItem(queries::query.value(0).toString());
    }
}

void LinkManagerWindow::refreshLinks(const QString &collectionName, const QString &searchString)
{
    ui->listWidgetLinks->clear();
    queries::selectLinksBasedOnCollection(collectionName, searchString);
    while (queries::query.next())
    {
        ui->listWidgetLinks->addItem(queries::query.value(0).toString());
    }
}

void LinkManagerWindow::addCollection()
{
    GetNameDialog dialog(this, "Collection Name", "Please provide a name for the collection you want to create:");
    common::openDialog(&dialog, ":/styles/style.qss");

    if (!dialog.name.isEmpty())
    {
        queries::insertLinkCollection(dialog.name);
        ui->statusBar->showMessage("New collection \"" + dialog.name + "\" added.");
    }

    refreshCollections("");
}

void LinkManagerWindow::addLink()
{
    InsertLinkDialog dialog(this);
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

void LinkManagerWindow::searchLinks(const QString &arg1)
{
    if (!ui->listWidgetCollections->selectedItems().isEmpty())
    {
        refreshLinks(ui->listWidgetCollections->currentItem()->text(), arg1);
    }
}

void LinkManagerWindow::collectionClicked(QListWidgetItem *item)
{
    refreshLinks(item->text(), "");
    ui->statusBar->showMessage("Collection \"" + item->text() + "\" selected.");
}

void LinkManagerWindow::openLink(QListWidgetItem *item)
{
    queries::selectLinkRecord(item->text());
    queries::query.next();

    QDesktopServices::openUrl(QUrl(queries::query.value(1).toString()));
}

void LinkManagerWindow::linkClicked(QListWidgetItem *item)
{
    queries::selectLinkRecord(item->text());
    queries::query.next();
    QString title = queries::query.value(0).toString();
    QString url = queries::query.value(1).toString();

    ui->textDetailsTitle->setText(title);
    ui->textDetailsUrl->setText(url);

    ui->statusBar->showMessage(title + ": " + url);
}

void LinkManagerWindow::clearDetails()
{
    ui->textDetailsTitle->clear();
    ui->textDetailsUrl->clear();
}

void LinkManagerWindow::restoreDetails()
{
    if (!ui->listWidgetLinks->selectedItems().isEmpty())
    {
        linkClicked(ui->listWidgetLinks->currentItem());
        ui->statusBar->showMessage("Details restored successfully.");
    }
}

void LinkManagerWindow::updateDetails()
{
    QString oldTitle = ui->listWidgetLinks->currentItem()->text();
    QString newTitle = ui->textDetailsTitle->text();
    QString newUrl = ui->textDetailsUrl->text();

    queries::updateLinkDetails(oldTitle, newTitle, newUrl);
    ui->listWidgetLinks->currentItem()->setText(newTitle);
    ui->statusBar->showMessage("Link \"" + oldTitle + "\" edited.");
}

void LinkManagerWindow::linkSelectionChanged()
{
    if (!ui->listWidgetLinks->selectedItems().isEmpty())
    {
        linkClicked(ui->listWidgetLinks->currentItem());
    }
}

void LinkManagerWindow::clearCollections()
{
    ui->textCollections->clear();
    ui->listWidgetCollections->clear();
}

void LinkManagerWindow::sortCollections()
{
    this->COLL_SORT = (this->COLL_SORT == Qt::AscendingOrder ? Qt::DescendingOrder : Qt::AscendingOrder);
    ui->listWidgetCollections->sortItems(this->COLL_SORT);
}

void LinkManagerWindow::clearLinks()
{
    ui->textLinks->clear();
    ui->listWidgetLinks->clear();
}

void LinkManagerWindow::sortLinks()
{
    this->LINKS_SORT = (this->LINKS_SORT == Qt::AscendingOrder ? Qt::DescendingOrder : Qt::AscendingOrder);
    ui->listWidgetLinks->sortItems(this->LINKS_SORT);
}

void LinkManagerWindow::hideLeftPanel()
{
    common::changeWidgetVisibility(ui->frameLeft, ui->actionHideLeftPanel);
}

void LinkManagerWindow::hideRightPanel()
{
    common::changeWidgetVisibility(ui->frameRight, ui->actionHideRightPanel);
}

void LinkManagerWindow::hideSearchBars()
{
    ui->frameLinksSearch->setHidden(!ui->frameLinksSearch->isHidden());
    ui->frameCollectionsSearch->setHidden(!ui->frameCollectionsSearch->isHidden());
    ui->actionHideSearchBars->setText(ui->frameCollectionsSearch->isHidden() ? "Show Search Bars" : "Hide Search Bars");
}

void LinkManagerWindow::resetLinks()
{
    queries::resetLinksTable();
}

void LinkManagerWindow::resetCollections()
{
    queries::resetCollectionsTable();
}

void LinkManagerWindow::actionEditLink()
{
    if (ui->listWidgetLinks->currentItem())
    {
        editLinkDetails(ui->listWidgetLinks->currentItem()->text());
    }
}

void LinkManagerWindow::actionDeleteCollection()
{
    if (ui->listWidgetCollections->currentItem())
    {
        deleteCollection(ui->listWidgetCollections->currentItem()->text());
    }
}

void LinkManagerWindow::actionRenameCollection()
{
    if (ui->listWidgetCollections->currentItem())
    {
        renameCollection(ui->listWidgetCollections->currentItem()->text());
    }
}

void LinkManagerWindow::actionOpenAllLinks()
{
    openAllLinks();
}

void LinkManagerWindow::actionDeleteLink()
{
    if (ui->listWidgetLinks->currentItem())
    {
        deleteLink(ui->listWidgetLinks->currentItem()->text());
    }
}

void LinkManagerWindow::hideStatusBar()
{
    common::changeWidgetVisibility(ui->statusBar, ui->actionHideStatusBar);
}
