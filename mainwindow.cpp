#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "common.h"
#include "queries.h"
#include "yesnodialog.h"
#include "summarywindow.h"
#include "addbookdialog.h"
#include "addbooksdialog.h"
#include "searchnamedialog.h"
#include "bookdetailswindow.h"
#include "cleanebooksdialog.h"
#include "linkmanagerwindow.h"
#include "extselectiondialog.h"

#include <QDir>
#include <QFile>
#include <QTimer>
#include <QProcess>
#include <QSystemTrayIcon>
#include <QDesktopServices>
#include <QOperatingSystemVersion>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    queries::connectToDatabase();
    queries::createEbooksTable();
    queries::createSettingsTable();
    queries::createSearchTable();
    queries::createLinksTable();
    queries::createLinkCollectionsTable();

    SORT = Qt::AscendingOrder;
    ui->setupUi(this);
    ui->ebooksListWidget->setSortingEnabled(true);
    QSizePolicy retain = ui->frameSearchToolBar->sizePolicy();
    retain.setRetainSizeWhenHidden(true);
    ui->frameSearchToolBar->setSizePolicy(retain);

    ui->ebooksListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->ebooksListWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));

    refreshFolders();
    refreshAuthors();
    refreshGenres();
    refreshSearches();

    auto *trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(windowIcon());
    trayIcon->setVisible(true);
    trayIcon->setToolTip("Ebook Access\nClick to Open");
    trayIcon->show();
    trayIcon->connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(show()));

    ui->statusBar->showMessage("Ready");
}

MainWindow::~MainWindow()
{
    queries::query.clear();
    queries::db.close();
    delete ui;
}

void MainWindow::trayClicked(QSystemTrayIcon::ActivationReason r)
{
    if( r == QSystemTrayIcon::Trigger)
    {
        this->show();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
}

void MainWindow::showSummary(QString name)
{
    auto *summaryWindow = new SummaryWindow();
    summaryWindow->setStyleSheet(common::openSheet(":/summarystyle.qss"));
    summaryWindow->ensurePolished();
    summaryWindow->show();
    summaryWindow->callSelectEbookSummary(name);
}

void MainWindow::showLinkManager()
{
    auto *linkManagerWindow = new LinkManagerWindow();
    linkManagerWindow->setStyleSheet(common::openSheet(":/summarystyle.qss"));
    linkManagerWindow->ensurePolished();
    linkManagerWindow->show();
}

void MainWindow::showDetails()
{
    auto *detailsWindow = new bookDetailsWindow();
    detailsWindow->setStyleSheet(common::openSheet(":/summarystyle.qss"));
    detailsWindow->ensurePolished();
    detailsWindow->show();
}

void MainWindow::refreshFolders()
{
    ui->comboBoxFolderCriteria->clear();
    ui->comboBoxFolderCriteria->addItem("");
    queries::selectFoldersQuery();
    while(queries::query.next())
    {
        ui->comboBoxFolderCriteria->addItem(queries::query.value(0).toString());
    }
    ui->comboBoxFolderCriteria->model()->sort(0, Qt::AscendingOrder);
}

void MainWindow::refreshAuthors()
{
    ui->comboBoxAuthorCriteria->clear();
    ui->comboBoxAuthorCriteria->addItem("");
    queries::selectAuthorsQuery();
    while(queries::query.next())
    {
        ui->comboBoxAuthorCriteria->addItem(queries::query.value(0).toString());
    }
    ui->comboBoxAuthorCriteria->model()->sort(0, Qt::AscendingOrder);
}

void MainWindow::refreshGenres()
{
    ui->comboBoxGenreCriteria->clear();
    ui->comboBoxGenreCriteria->addItem("");
    queries::selectGenreQuery();
    while(queries::query.next())
    {
        ui->comboBoxGenreCriteria->addItem(queries::query.value(0).toString());
    }
    ui->comboBoxGenreCriteria->model()->sort(0, Qt::AscendingOrder);
}

void MainWindow::refreshSearches()
{
    ui->comboBoxSearchLoad->clear();
    ui->comboBoxSearchLoad->addItem("");
    queries::selectSearchesQuery();
    while(queries::query.next())
    {
        ui->comboBoxSearchLoad->addItem(queries::query.value(0).toString());
    }
    ui->comboBoxSearchLoad->model()->sort(0, Qt::AscendingOrder);
}

double MainWindow::changeBookSizeUnit(double size, QString unit)
{
    if (unit == "KB")
    {
        size /= 1024;
    }
    else if (unit == "MB")
    {
        size /= (1024 * 1024);
    }
    else if (unit == "GB")
    {
        size /= (1024 * 1024 * 1024);
    }

    return size;
}

void MainWindow::showContextMenu(const QPoint &pos)
{
    QPoint globalPos = ui->ebooksListWidget->mapToGlobal(pos);

    QMenu menu;
    menu.setStyleSheet(common::openSheet(":/style.qss"));

    menu.addAction("Open Ebook", this, [this]{on_ebooksListWidget_itemActivated(ui->ebooksListWidget->currentItem());});
    menu.addAction("Open Summary", this, SLOT(openSummaryWindow()));
    menu.addAction("Show in Folder", this, SLOT(openFolder()));
    menu.addAction("Delete Ebook", this, SLOT(deleteListItem()));

    if (!ui->ebooksListWidget->selectedItems().isEmpty())
    {
        menu.exec(globalPos);
    }
}

void MainWindow::openSummaryWindow()
{
    showSummary(ui->ebooksListWidget->currentItem()->text());
}

void MainWindow::deleteListItem()
{
    QString itemName = ui->ebooksListWidget->currentItem()->text();
    QListWidgetItem *currentItem = ui->ebooksListWidget->currentItem();

    yesNoDialog dialog(this, "Delete File", "Delete File", "Do you wish to delete the file on your hard drive as well?");
    common::openDialog(&dialog, ":/style.qss");
    bool result = dialog.getResult();

    if (result)
    {
        queries::selectPathBasedonName(itemName);
        queries::query.next();
        QString path = queries::query.value(0).toString();
        QFile file(path);
        file.remove();
    }

    queries::deleteBook(itemName);
    delete (currentItem);

    ui->statusBar->showMessage(itemName + " deleted.");
}

void MainWindow::openFolder()
{
    QString fileName = ui->ebooksListWidget->currentItem()->text();
    queries::selectPathBasedonName(fileName);
    queries::query.next();
    QString filePath = queries::query.value(0).toString();
    QFileInfo file(filePath);

    if (QOperatingSystemVersion::current().type() == 1)
    {
        QStringList args;
        args << "/select," << QDir::toNativeSeparators(queries::query.value(0).toString());
        auto *process = new QProcess(this);
        process->start("explorer.exe", args);
    }
    else
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile((file.dir().path())));
    }

}

void MainWindow::on_buttonAddBooks_clicked()
{
    addBooksDialog dialog(this);
    common::openDialog(&dialog, ":/style.qss");


    refreshFolders();
    refreshAuthors();
    refreshGenres();
}

void MainWindow::on_buttonSearchString_clicked()
{
    ui->ebooksListWidget->clear();
    QString stringToSearch = ui->textSearchBar->text();
    queries::selectNameBasedOnString(stringToSearch);
    long count = 0;
    while(queries::query.next())
    {
        ui->ebooksListWidget->addItem(queries::query.value(0).toString());
        count++;
    }
    ui->statusBar->showMessage("Number of ebooks: " + QString::number(count));
}

void MainWindow::on_buttonClearCriteria_clicked()
{
    ui->comboBoxFolderCriteria->setCurrentIndex(0);
    ui->comboBoxAuthorCriteria->setCurrentIndex(0);
    ui->comboBoxGenreCriteria->setCurrentIndex(0);

    ui->textTagsCriteria->clear();
    ui->textExts->clear();

    ui->spinBoxFromPagesCriteria->setValue(0);
    ui->spinBoxToPagesCriteria->setValue(100000);
    ui->spinBoxFromSizeCriteria->setValue(0);
    ui->spinBoxToSizeCriteria->setValue(1024);

    ui->ebooksListWidget->clear();
    ui->statusBar->clearMessage();
}

void MainWindow::on_buttonDetailsClear_clicked()
{
    ui->textDetailsAuthor->clear();
    ui->textDetailsName->clear();
    ui->textDetailsGenre->clear();
    ui->textDetailsTags->clear();

    ui->textDetailsExt->clear();
    ui->textDetailsSize->clear();
    ui->textDetailsFolder->clear();
    ui->textDetailsPages->clear();

    ui->statusBar->clearMessage();
}

void MainWindow::on_buttonSortSearch_clicked()
{
    SORT = (SORT == Qt::AscendingOrder ? Qt::DescendingOrder : Qt::AscendingOrder);
    ui->ebooksListWidget->sortItems(SORT);
}

void MainWindow::on_actionResetEbooks_triggered()
{
    queries::resetEbooksTableQuery();

    refreshFolders();
    refreshAuthors();
    refreshGenres();
    ui->ebooksListWidget->clear();
    ui->statusBar->showMessage("All ebooks have been deleted.");
}

void MainWindow::on_actionHideSearchBar_triggered()
{
    ui->frameSearchToolBar->setHidden(!ui->frameSearchToolBar->isHidden());
    ui->actionHideSearchBar->setText(ui->frameSearchToolBar->isHidden() ? "Show Search Bar" : "Hide Search Bar");
}

void MainWindow::on_actionHideDetailsSection_triggered()
{
    ui->frameDetails->setHidden(!ui->frameDetails->isHidden());
    ui->actionHideDetailsSection->setText(ui->frameDetails->isHidden() ? "Show Details Section" : "Hide Details Section");
    ui->frameMainRight->setHidden(ui->ebooksListWidget->isHidden() && ui->frameDetails->isHidden());

}

void MainWindow::on_actionHideRightFrame_triggered()
{
    ui->frameMainRight->setHidden(!ui->frameMainRight->isHidden());
    ui->actionHideRightFrame->setText(ui->frameMainRight->isHidden() ? "Show Right Frame" : "Hide Right Frame");
}

void MainWindow::on_actionHideUtilities_triggered()
{
    ui->frameBottomButtons->setHidden(!ui->frameBottomButtons->isHidden());
    ui->actionHideUtilities->setText(ui->frameBottomButtons->isHidden() ? "Show Utilities" : "Hide Utilities");
}

void MainWindow::on_actionHideLeftFrame_triggered()
{
    ui->frameMainLeft->setHidden(!ui->frameMainLeft->isHidden());
    ui->actionHideLeftFrame->setText(ui->frameMainLeft->isHidden() ? "Show Left Frame" : "Hide Left Frame");
}

void MainWindow::on_buttonAddBook_clicked()
{
    addBookDialog dialog(this);
    common::openDialog(&dialog, ":/style.qss");

    refreshFolders();
    refreshAuthors();
    refreshGenres();
}

void MainWindow::on_buttonClearSearch_clicked()
{
    ui->textSearchBar->clear();
    ui->ebooksListWidget->clear();

    ui->textDetailsName->clear();
    ui->textDetailsFolder->clear();
    ui->textDetailsGenre->clear();
    ui->textDetailsAuthor->clear();
    ui->textDetailsExt->clear();
    ui->textDetailsPages->clear();
    ui->textDetailsSize->clear();
    ui->textDetailsTags->clear();

    ui->statusBar->clearMessage();
}

void MainWindow::on_buttonSettings_clicked()
{
    showDetails();
}

void MainWindow::on_buttonSearchCriteria_clicked()
{
    ui->ebooksListWidget->clear();
    QString folder = ui->comboBoxFolderCriteria->currentText();
    QString author = ui->comboBoxAuthorCriteria->currentText();
    QString genre = ui->comboBoxGenreCriteria->currentText();
    QString tags = ui->textTagsCriteria->text();

    long long sizeTo = ui->spinBoxToSizeCriteria->value();
    long long sizeFrom = ui->spinBoxFromSizeCriteria->value();

    if (ui->buttonSizeCriteria->text() == "KB")
    {
        sizeTo = sizeTo * 1024;
        sizeFrom = sizeFrom * 1024;
    }
    else if (ui->buttonSizeCriteria->text() == "MB")
    {
        sizeTo = sizeTo * 1024 * 1024;
        sizeFrom = sizeFrom * 1024 * 1024;
    }
    else
    {   sizeTo = sizeTo * 1024 * 1024 * 1024;
        sizeFrom = sizeFrom * 1024 * 1024 * 1024;
    }

    long pagesTo = ui->spinBoxToPagesCriteria->value();
    long pagesFrom = ui->spinBoxFromPagesCriteria->value();

    QString ext = ui->textExts->text();

    queries::selectNameBasedOnCriteria(folder, genre, author, tags, ext, pagesFrom, pagesTo, sizeFrom, sizeTo);
    long count = 0;
    while(queries::query.next())
    {
        ui->ebooksListWidget->addItem(queries::query.value(0).toString());
        count++;
    }
    ui->statusBar->showMessage("Number of ebooks: " + QString::number(count));
}

void MainWindow::on_ebooksListWidget_itemClicked(QListWidgetItem *item)
{
    QString fileName = item->text();
    queries::selectAllBasedonName(fileName);
    queries::query.next();
    QString author = queries::query.value(1).toString();
    author = (author == "N/A" ? "" : author);
    QString genre = queries::query.value(2).toString();
    genre = (genre == "N/A" ? "" : genre);
    QString ext = queries::query.value(4).toString();
    QString pages = queries::query.value(5).toString();

    double size = changeBookSizeUnit(queries::query.value(6).toDouble(), ui->buttonSizeUnit->text());

    QString folder = queries::query.value(7).toString();
    QString tags = queries::query.value(8).toString();

    ui->textDetailsName->setText(fileName);
    ui->textDetailsAuthor->setText(author);
    ui->textDetailsGenre->setText(genre);
    ui->textDetailsPages->setText(pages);
    ui->textDetailsExt->setText(ext);
    ui->textDetailsSize->setText(QString::number(size));
    ui->textDetailsFolder->setText(folder);
    ui->textDetailsTags->setText(tags);
    ui->statusBar->showMessage("Current selected ebook: " + fileName);

}

void MainWindow::on_buttonDetailsRestore_clicked()
{
    if (!ui->ebooksListWidget->selectedItems().isEmpty())
    {
        on_ebooksListWidget_itemClicked(ui->ebooksListWidget->currentItem());
        ui->statusBar->showMessage("Details restored successfully.");
    }
}

void MainWindow::on_buttonDetailsUpdate_clicked()
{
    if (!ui->ebooksListWidget->selectedItems().isEmpty())
    {
        QString oldName = ui->ebooksListWidget->currentItem()->text();
        QString newName = ui->textDetailsName->text();
        QString author = ui->textDetailsAuthor->text();
        QString folder = ui->textDetailsFolder->text();
        QString genre = ui->textDetailsGenre->text();
        QString tags = ui->textDetailsTags->text().trimmed();
        int pages = ui->textDetailsPages->text().toInt();

        queries::selectPathBasedonName(oldName);
        queries::query.next();
        QString path = queries::query.value(0).toString();

        if (newName != oldName)
        {
            yesNoDialog dialog(this, "Rename File", "Rename File", "Do you wish to rename the file on your hard drive as well?");
            common::openDialog(&dialog, ":/style.qss");
            bool result = dialog.getResult();
            if (result)
            {
                QFile file(path);
                QFileInfo info(file);
                path = info.absolutePath() + "/" + newName + "." + info.suffix();
                file.rename(path);
            }
        }
        queries::updateBookQuery(oldName, newName, folder, genre, author, pages, tags, path);
        ui->ebooksListWidget->currentItem()->setText(newName);

        refreshAuthors();
        refreshFolders();
        refreshGenres();

        ui->statusBar->showMessage("Details updated successfully.");
    }
}

void MainWindow::on_buttonSaveCriteria_clicked()
{
    QString folder = ui->comboBoxFolderCriteria->currentText();
    QString author = ui->comboBoxAuthorCriteria->currentText();
    QString genre = ui->comboBoxGenreCriteria->currentText();
    QString tags = ui->textTagsCriteria->text();
    QString ext = ui->textExts->text();
    int sizeTo = ui->spinBoxToSizeCriteria->value();
    int sizeFrom = ui->spinBoxFromSizeCriteria->value();
    QString sizeIn = ui->buttonSizeCriteria->text();
    int pagesTo = ui->spinBoxToPagesCriteria->value();
    int pagesFrom = ui->spinBoxFromPagesCriteria->value();

    searchNameDialog dialog(this);
    common::openDialog(&dialog, ":/style.qss");
    if(!dialog.searchName.isEmpty())
    {
        queries::insertSearchQuery(dialog.searchName, folder, author, genre, tags, ext,
                                   sizeFrom, sizeTo, sizeIn, pagesFrom, pagesTo);
        ui->statusBar->showMessage("Search saved successfully.");
    }

    refreshSearches();
}

void MainWindow::on_buttonSearchLoad_clicked()
{
    if (!ui->comboBoxSearchLoad->currentText().isEmpty())
    {
        QString searchName = ui->comboBoxSearchLoad->currentText();
        queries::selectSearchCriteriaQuery(searchName);
        queries::query.next();
        ui->comboBoxFolderCriteria->setCurrentText(queries::query.value(1).toString());
        ui->comboBoxAuthorCriteria->setCurrentText(queries::query.value(2).toString());
        ui->comboBoxGenreCriteria->setCurrentText(queries::query.value(3).toString());
        ui->textTagsCriteria->setText(queries::query.value(4).toString());


        ui->textExts->setText(queries::query.value(5).toString());


        ui->spinBoxFromSizeCriteria->setValue(queries::query.value(6).toInt());
        ui->spinBoxToSizeCriteria->setValue(queries::query.value(7).toInt());
        ui->buttonSizeCriteria->setText(queries::query.value(8).toString());

        ui->spinBoxFromPagesCriteria->setValue(queries::query.value(9).toInt());
        ui->spinBoxToPagesCriteria->setValue(queries::query.value(10).toInt());

        ui->statusBar->showMessage("Search loaded.");
    }
}

void MainWindow::on_actionCleanEbooks_triggered()
{
    cleanEbooksDialog dialog(this);
    common::openDialog(&dialog, ":/style.qss");

}

void MainWindow::on_buttonSummaries_clicked()
{
    showSummary("");
}

void MainWindow::on_buttonLinkManager_clicked()
{
    showLinkManager();
}

void MainWindow::on_actionChooseRandomBook_triggered()
{
    queries::selectCountEbooks();
    queries::query.next();
    int numberOfEbooks = queries::query.value(0).toInt();
    if (numberOfEbooks != 0)
    {
        srand((unsigned) time(0));
        int randomNumber = 1 + (rand() % numberOfEbooks);
        queries::selectNameBasedOnRowid(randomNumber);
        queries::query.next();
        QString randomName = queries::query.value(0).toString();
        ui->textSearchBar->setText(randomName);
        ui->ebooksListWidget->setCurrentRow(0);
        ui->statusBar->showMessage("Random Book: " + randomName);
    }

}

void MainWindow::on_ebooksListWidget_itemSelectionChanged()
{
    if (ui->ebooksListWidget->count() >= 1)
        on_ebooksListWidget_itemClicked(ui->ebooksListWidget->currentItem());
}

void MainWindow::on_actionFullscreen_triggered()
{
    !this->isFullScreen() ? this->showFullScreen() : this->showNormal();
}

void MainWindow::on_ebooksListWidget_itemActivated(QListWidgetItem *item)
{
    QString ebookName = item->text();
    queries::selectPathBasedonName(ebookName);
    queries::query.next();
    QString path = queries::query.value(0).toString();
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));

    ui->statusBar->showMessage(ebookName + " ebook opened.");
}

void MainWindow::on_buttonSizeCriteria_clicked()
{
    QString currentText = ui->buttonSizeCriteria->text();
    if (currentText == "KB")
        ui->buttonSizeCriteria->setText("MB");
    else if (currentText == "MB")
        ui->buttonSizeCriteria->setText("GB");
    else
        ui->buttonSizeCriteria->setText("KB");
}

void MainWindow::on_buttonExtensions_clicked()
{
    queries::selectExt();
    QVector<QString> ext;
    while(queries::query.next())
    {
        ext.push_back(queries::query.value(0).toString());
    }

    auto *dialog = new extSelectionDialog(this, ext, "Extensions", "Select Available Extensions");
    common::openDialog(dialog, ":/style.qss");

    ext = dialog->getExtVector();
    QString extString = ext.join(common::SEP);
    ui->textExts->setText(extString);
}

void MainWindow::on_buttonFolder_clicked()
{
    queries::selectFoldersQuery();
    QVector<QString> folders;
    while(queries::query.next())
    {
        folders.push_back(queries::query.value(0).toString());
    }

    auto *dialog = new extSelectionDialog(this, folders, "Folders", "Select Available Folders");
    common::openDialog(dialog, ":/style.qss");

    folders = dialog->getExtVector();
    QString folderString = folders.join(common::SEP);
    ui->comboBoxFolderCriteria->setCurrentText(folderString);
}

void MainWindow::on_buttonAuthor_clicked()
{
    queries::selectAuthorsQuery();
    QVector<QString> authors;
    while(queries::query.next())
    {
        authors.push_back(queries::query.value(0).toString());
    }

    auto *dialog = new extSelectionDialog(this, authors, "Authors", "Select Available Authors");
    common::openDialog(dialog, ":/style.qss");

    authors = dialog->getExtVector();
    QString authorsString = authors.join(common::SEP);
    ui->comboBoxAuthorCriteria->setCurrentText(authorsString);
}

void MainWindow::on_buttonGenre_clicked()
{
    queries::selectGenreQuery();
    QVector<QString> genres;
    while(queries::query.next())
    {
        genres.push_back(queries::query.value(0).toString());
    }

    auto *dialog = new extSelectionDialog(this, genres, "Genres", "Select Available Genres");
    common::openDialog(dialog, ":/style.qss");

    genres = dialog->getExtVector();
    QString extString = genres.join(common::SEP);
    ui->comboBoxGenreCriteria->setCurrentText(extString);
}

void MainWindow::on_buttonTags_clicked()
{
    QVector<QString> tags;
    queries::selectTags();
    while(queries::query.next())
    {
        QString tagString = queries::query.value(0).toString();
        QStringList tagList = tagString.split(common::SEP);
        for(QString &tag : tagList)
        {
            if(!tags.contains(tag))
            {
                tags.push_back(tag);
            }
        }
    }

    auto *dialog = new extSelectionDialog(this, tags, "Tags", "Select Available Tags");
    common::openDialog(dialog, ":style.qss");

    tags = dialog->getExtVector();
    QString tagString = tags.join(common::SEP);
    ui->textTagsCriteria->setText(tagString);
}

void MainWindow::on_buttonSizeUnit_clicked()
{
    QString currentText = ui->buttonSizeUnit->text();
    if (currentText == "KB")
        ui->buttonSizeUnit->setText("MB");
    else if (currentText == "MB")
        ui->buttonSizeUnit->setText("GB");
    else
        ui->buttonSizeUnit->setText("KB");

    if (!ui->ebooksListWidget->selectedItems().isEmpty())
    {
        on_ebooksListWidget_itemClicked(ui->ebooksListWidget->currentItem());
    }
}

void MainWindow::on_actionMinimizeTray_triggered()
{
    QTimer::singleShot(0, this, SLOT(hide()));
}

void MainWindow::on_actionResetSummaries_triggered()
{
    queries::resetSummaries();
}

void MainWindow::on_actionResetTags_triggered()
{
    queries::resetTags();
}

void MainWindow::on_actionResetSearches_triggered()
{
    queries::resetSearchesTable();
    refreshSearches();
}

void MainWindow::on_actionResetDatabase_triggered()
{
    on_actionResetEbooks_triggered();
    on_actionResetSummaries_triggered();
}

