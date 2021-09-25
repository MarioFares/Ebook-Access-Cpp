#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <common.h>
#include <queries.h>
#include <summarywindow.h>
#include <addbookdialog.h>
#include <addbooksdialog.h>
#include <searchnamedialog.h>
#include <cleanebooksdialog.h>
#include <linkmanagerwindow.h>

#include <filesystem>

#include <QDir>
#include <QFile>
#include <QProcess>
#include <QMessageBox>
#include <QDesktopServices>
#include <QOperatingSystemVersion>

namespace fs = std::filesystem;

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

    fs::create_directory("./print");

    refreshFolders();
    refreshAuthors();
    refreshGenres();
    refreshSearches();

    ui->statusBar->showMessage("Ready");
}

MainWindow::~MainWindow()
{
    queries::query.clear();
    queries::db.close();
    delete ui;
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
    QFile file(":/style.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    menu.setStyleSheet(styleSheet);

    menu.addAction("Open Ebook", this, [this]{on_ebooksListWidget_itemActivated(ui->ebooksListWidget->currentItem());});
    menu.addAction("Open Summary", this, SLOT(openSummaryWindow()));
    menu.addAction("Show in Folder", this, SLOT(openFolder()));
    menu.addAction("Delete Ebook", this, SLOT(deleteListItem()));


    if (ui->ebooksListWidget->selectedItems().size() != 0)
    {menu.exec(globalPos);}
}

void MainWindow::openSummaryWindow()
{
    showSummary(ui->ebooksListWidget->currentItem()->text());
}

void MainWindow::deleteListItem()
{
    QString itemName = ui->ebooksListWidget->currentItem()->text();
    queries::deleteBook(itemName);
    delete (ui->ebooksListWidget->currentItem());
    ui->statusBar->showMessage(itemName + " deleted.");
}

void MainWindow::openFolder()
{
    QString fileName = ui->ebooksListWidget->currentItem()->text();
    queries::selectPathBasedonName(fileName);
    queries::query.next();
    fs::path filePath = queries::query.value(0).toString().toStdString();

    if (QOperatingSystemVersion::current().type() == 1)
    {
        QStringList args;
        args << "/select," << QDir::toNativeSeparators(queries::query.value(0).toString());
        QProcess *process = new QProcess(this);
        process->start("explorer.exe", args);
    }
    else
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile(QString::fromStdString(filePath.parent_path().string())));
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

    ui->spinBoxFromPagesCriteria->setValue(0);
    ui->spinBoxToPagesCriteria->setValue(0);
    ui->spinBoxFromSizeCriteria->setValue(0);
    ui->spinBoxToSizeCriteria->setValue(0);

    ui->checkBoxCbr->setChecked(false);
    ui->checkBoxEpub->setChecked(false);
    ui->checkBoxMobi->setChecked(false);
    ui->checkBoxPdf->setChecked(false);

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

    ui->statusBar->showMessage("All ebooks have been deleted.");
}

void MainWindow::on_actionHideSearchBar_triggered()
{
    ui->frameSearchToolBar->setHidden(!ui->frameSearchToolBar->isHidden());
    ui->actionHideSearchBar->setText(ui->frameSearchToolBar->isHidden() ? "Show Search Bar" : "Hide Search Bar");
}

void MainWindow::on_actionHideListWidget_triggered()
{
    ui->ebooksListWidget->setHidden(!ui->ebooksListWidget->isHidden());
    ui->actionHideListWidget->setText(ui->ebooksListWidget->isHidden() ? "Show List Widget" : "Hide List Widget");
    ui->frameMainRight->setHidden(ui->ebooksListWidget->isHidden() && ui->frameDetails->isHidden());
}

void MainWindow::on_actionHideDetailsSection_triggered()
{
    ui->frameDetails->setHidden(!ui->frameDetails->isHidden());
    ui->actionHideDetailsSection->setText(ui->frameDetails->isHidden() ? "Show Details Section" : "Hide Details Section");
    ui->frameMainRight->setHidden(ui->ebooksListWidget->isHidden() && ui->frameDetails->isHidden());

}

void MainWindow::on_actionHideCriteriaSection_triggered()
{
    ui->frameSearchCriteria->setHidden(!ui->frameSearchCriteria->isHidden());
    ui->actionHideCriteriaSection->setText(ui->frameSearchCriteria->isHidden() ? "Show Criteria Section" : "Hide Criteria Section");
    ui->frameMainLeft->setHidden(ui->frameSearchCriteria->isHidden() && ui->frameBottomButtons->isHidden());
}

void MainWindow::on_actionHideAddsSection_triggered()
{
    ui->frameAddButtons->setHidden(!ui->frameAddButtons->isHidden());
    ui->actionHideAddsSection->setText(ui->frameAddButtons->isHidden() ? "Show Add Section" : "Hide Add Section");
    ui->frameBottomButtons->setHidden(ui->frameAddButtons->isHidden() && ui->frameInfoButtons->isHidden());
    ui->frameMainLeft->setHidden(ui->frameSearchCriteria->isHidden() && ui->frameBottomButtons->isHidden());

}

void MainWindow::on_actionHideUtilitiesSection_triggered()
{
    ui->frameInfoButtons->setHidden(!ui->frameInfoButtons->isHidden());
    ui->actionHideUtilitiesSection->setText(ui->frameInfoButtons->isHidden() ? "Show Utilities Section" : "Hide Utilities Section");
    ui->frameBottomButtons->setHidden(ui->frameAddButtons->isHidden() && ui->frameInfoButtons->isHidden());
    ui->frameMainLeft->setHidden(ui->frameSearchCriteria->isHidden() && ui->frameBottomButtons->isHidden());

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

    if (ui->comboBoxToggleSize->currentText() == "KB")
    {
        sizeTo = sizeTo * 1024;
        sizeFrom = sizeFrom * 1024;
    }
    else if (ui->comboBoxToggleSize->currentText() == "MB")
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

    QString extensions[4] = {ui->checkBoxPdf->isChecked() ? ".pdf" : "",
                             ui->checkBoxEpub->isChecked() ? ".epub" : "",
                             ui->checkBoxMobi->isChecked() ? ".mobi" : "",
                             ui->checkBoxCbr->isChecked() ? ".cbr" : ""
                            };

    queries::selectNameBasedOnCriteria(folder, genre, author, tags, pagesFrom, pagesTo, sizeFrom, sizeTo, extensions);
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

    double size = changeBookSizeUnit(queries::query.value(6).toDouble(), ui->comboBoxSizeUnit->currentText());

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
    if (ui->ebooksListWidget->selectedItems().size() != 0)
    {on_ebooksListWidget_itemClicked(ui->ebooksListWidget->currentItem());
    ui->statusBar->showMessage("Details restored successfully.");
    }
}

void MainWindow::on_buttonDetailsUpdate_clicked()
{
    if (ui->ebooksListWidget->selectedItems().size() != 0)
    {
        QString oldName = ui->ebooksListWidget->currentItem()->text();
        QString newName = ui->textDetailsName->text();
        QString author = ui->textDetailsAuthor->text();
        QString folder = ui->textDetailsFolder->text();
        QString genre = ui->textDetailsGenre->text();

        QString tags = ui->textDetailsTags->text().trimmed();
        int pages = ui->textDetailsPages->text().toInt();

        queries::updateBookQuery(oldName, newName, folder, genre, author, pages, tags);
        ui->ebooksListWidget->currentItem()->setText(newName);

        if (!ui->textDetailsTags->text().isEmpty())
        {

        }
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
    int pdf = ui->checkBoxPdf->isChecked();
    int epub = ui->checkBoxEpub->isChecked();
    int mobi = ui->checkBoxMobi->isChecked();
    int cbr = ui->checkBoxCbr->isChecked();
    int sizeTo = ui->spinBoxToSizeCriteria->value();
    int sizeFrom = ui->spinBoxFromSizeCriteria->value();
    QString sizeIn = ui->comboBoxToggleSize->currentText();
    int pagesTo = ui->spinBoxToPagesCriteria->value();
    int pagesFrom = ui->spinBoxFromPagesCriteria->value();

    searchNameDialog dialog(this);
    common::openDialog(&dialog, ":/style.qss");
    if(!dialog.searchName.isEmpty())
    {
        queries::insertSearchQuery(dialog.searchName, folder, author, genre, tags, pdf, epub, mobi, cbr,
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

        ui->checkBoxPdf->setChecked(queries::query.value(5).toBool());
        ui->checkBoxEpub->setChecked(queries::query.value(6).toBool());
        ui->checkBoxMobi->setChecked(queries::query.value(7).toBool());
        ui->checkBoxCbr->setChecked(queries::query.value(8).toBool());

        ui->spinBoxFromSizeCriteria->setValue(queries::query.value(9).toInt());
        ui->spinBoxToSizeCriteria->setValue(queries::query.value(10).toInt());
        ui->comboBoxToggleSize->setCurrentText(queries::query.value(11).toString());

        ui->spinBoxFromPagesCriteria->setValue(queries::query.value(12).toInt());
        ui->spinBoxToPagesCriteria->setValue(queries::query.value(13).toInt());

        ui->statusBar->showMessage("Search loaded.");
    }
}

void MainWindow::on_actionCleanEbooks_triggered()
{
    cleanEbooksDialog dialog(this);
    common::openDialog(&dialog, ":/style.qss");

}

void MainWindow::on_comboBoxSizeUnit_currentTextChanged()
{
   if (ui->ebooksListWidget->selectedItems().size() != 0)
    {on_ebooksListWidget_itemClicked(ui->ebooksListWidget->currentItem());}
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
