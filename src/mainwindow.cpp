#include "ui_mainwindow.h"

#include "include/mainwindow.h"
#include "include/common.h"
#include "include/queries.h"
#include "include/yesnodialog.h"
#include "include/summarywindow.h"
#include "include/addbookdialog.h"
#include "include/addbooksdialog.h"
#include "include/dataviewerwindow.h"
#include "include/searchnamedialog.h"
#include "include/cleanebooksdialog.h"
#include "include/linkmanagerwindow.h"
#include "include/extselectiondialog.h"

#include <QDir>
#include <QFile>
#include <QTimer>
#include <QProcess>
#include <QSystemTrayIcon>
#include <QDesktopServices>
#include <QOperatingSystemVersion>
#include <QToolButton>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    // Setup DB
    queries::connectToDatabase();
    queries::createEbooksTable();
    queries::createSettingsTable();
    queries::createSearchTable();
    queries::createLinksTable();
    queries::createLinkCollectionsTable();

    // Setup Conversion
    sizeConvFactors["KB"] = 1024;
    sizeConvFactors["MB"] = 1024 * 1024;
    sizeConvFactors["GB"] = 1024 * 1024 * 1024;

    // Setup Size Factors to Toggle Sizes
    sizeUnits["KB"] = "MB";
    sizeUnits["MB"] = "GB";
    sizeUnits["GB"] = "KB";

    // Setup List Sorting
    SORT = Qt::AscendingOrder;
    ui->setupUi(this);
    ui->ebooksListWidget->setSortingEnabled(true);

    // Set Search Frame Policy
    QSizePolicy retain = ui->frameSearchToolBar->sizePolicy();
    retain.setRetainSizeWhenHidden(true);
    ui->frameSearchToolBar->setSizePolicy(retain);

    // Set Right-Click Menu
    ui->ebooksListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->ebooksListWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));

    // Refresh ComboBoxes
    refreshFolders();
    refreshAuthors();
    refreshGenres();
    refreshSearches();

    // Setup Tray Icon
    auto *trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(windowIcon());
    trayIcon->setVisible(true);
    trayIcon->setToolTip("Ebook Access\nClick to Open");
    trayIcon->show();
    trayIcon->connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(show()));

    // Set up clear buttons
    ui->textExts->setClearButtonEnabled(true);
    ui->textTagsCriteria->setClearButtonEnabled(true);
    ui->textSearchBar->setClearButtonEnabled(true);

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
    if (r == QSystemTrayIcon::Trigger)
    {
        this->show();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
}

void MainWindow::showSummary(const QString &name)
{
    auto *summaryWindow = new SummaryWindow();
    common::openWindow(summaryWindow, ":/styles/style.qss");
    summaryWindow->callSelectEbookSummary(name);
}

void MainWindow::showLinkManager()
{
    auto *linkManagerWindow = new LinkManagerWindow();
    common::openWindow(linkManagerWindow, ":/styles/style.qss");
}

void MainWindow::refreshComboBox(QComboBox *comboBox)
{
    comboBox->clear();
    comboBox->addItem("");
    while (queries::query.next())
    {
        comboBox->addItem(queries::query.value(0).toString());
    }

    comboBox->model()->sort(0, Qt::AscendingOrder);
}

void MainWindow::refreshFolders()
{
    queries::selectFoldersQuery();
    refreshComboBox(ui->comboBoxFolderCriteria);
}

void MainWindow::refreshAuthors()
{
    queries::selectAuthorsQuery();
    refreshComboBox(ui->comboBoxAuthorCriteria);
}

void MainWindow::refreshGenres()
{
    queries::selectGenreQuery();
    refreshComboBox(ui->comboBoxGenreCriteria);
}

void MainWindow::refreshSearches()
{
    queries::selectSearchesQuery();
    refreshComboBox(ui->comboBoxSearchLoad);
}

double MainWindow::changeBookSizeUnit(double size, const QString &unit)
{
    size /= this->sizeConvFactors[unit];
    return size;
}

void MainWindow::showContextMenu(const QPoint &pos)
{
    QPoint globalPos = ui->ebooksListWidget->mapToGlobal(pos);

    QMenu menu;
    menu.setStyleSheet(common::openSheet(":/styles/style.qss"));

    menu.addAction("Open Ebook", this, [this]
    {
        on_ebooksListWidget_itemActivated(ui->ebooksListWidget->currentItem());
    });
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
    common::openDialog(&dialog, ":/styles/style.qss");
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
    delete(currentItem);

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
    common::openDialog(&dialog, ":/styles/style.qss");

    refreshFolders();
    refreshAuthors();
    refreshGenres();
}

void MainWindow::on_buttonSearchString_clicked()
{
    ui->ebooksListWidget->clear();
    QString stringToSearch = ui->textSearchBar->text();
    queries::selectNameBasedOnString(stringToSearch);
    quint32 count = 0;
    while (queries::query.next())
    {
        ui->ebooksListWidget->addItem(queries::query.value(0).toString());
        count++;
    }

    ui->statusBar->showMessage("Number of ebooks: " + QString::number(count));
}

void MainWindow::on_buttonClearCriteria_clicked()
{
    ui->spinBoxFromSizeCriteria->setValue(0);
    ui->spinBoxFromPagesCriteria->setValue(0);
    ui->spinBoxToSizeCriteria->setValue(1024);
    ui->spinBoxToPagesCriteria->setValue(100000);
    ui->comboBoxGenreCriteria->setCurrentIndex(0);
    ui->comboBoxAuthorCriteria->setCurrentIndex(0);
    ui->comboBoxFolderCriteria->setCurrentIndex(0);
    ui->textExts->clear();
    ui->textTagsCriteria->clear();
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
    common::changeWidgetVisibility(ui->frameSearchToolBar, ui->actionHideSearchBar);
}

void MainWindow::on_actionHideDetailsSection_triggered()
{
    ui->frameDetails->setHidden(!ui->frameDetails->isHidden());
    ui->actionHideDetailsSection->setText(ui->frameDetails->isHidden() ? "Show Details Section" : "Hide Details Section");
    ui->frameMainRight->setHidden(ui->ebooksListWidget->isHidden() && ui->frameDetails->isHidden());
}

void MainWindow::on_actionHideRightFrame_triggered()
{
    common::changeWidgetVisibility(ui->frameMainRight, ui->actionHideRightFrame);
}

void MainWindow::on_actionHideUtilities_triggered()
{
    common::changeWidgetVisibility(ui->frameBottomButtons, ui->actionHideUtilities);
}

void MainWindow::on_actionHideLeftFrame_triggered()
{
    common::changeWidgetVisibility(ui->frameMainLeft, ui->actionHideLeftFrame);
}

void MainWindow::on_buttonAddBook_clicked()
{
    addBookDialog dialog(this);
    common::openDialog(&dialog, ":/styles/style.qss");
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

void MainWindow::on_buttonSearchCriteria_clicked()
{
    ui->ebooksListWidget->clear();
    QString folder = ui->comboBoxFolderCriteria->currentText();
    QString author = ui->comboBoxAuthorCriteria->currentText();
    QString genre = ui->comboBoxGenreCriteria->currentText();
    QString tags = ui->textTagsCriteria->text();
    quint64 sizeTo = ui->spinBoxToSizeCriteria->value();
    quint64 sizeFrom = ui->spinBoxFromSizeCriteria->value();
    QString convUnit = ui->buttonSizeCriteria->text();
    sizeTo = sizeTo *this->sizeConvFactors[convUnit];
    sizeFrom = sizeFrom *this->sizeConvFactors[convUnit];
    quint32 pagesTo = ui->spinBoxToPagesCriteria->value();
    quint32 pagesFrom = ui->spinBoxFromPagesCriteria->value();
    QString ext = ui->textExts->text();
    queries::selectNameBasedOnCriteria(folder, genre, author, tags, ext, pagesFrom, pagesTo, sizeFrom, sizeTo);
    quint32 count = 0;
    while (queries::query.next())
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
    QString author = queries::query.value("author").toString();
    author = (author == "N/A" ? "" : author);
    QString genre = queries::query.value("genre").toString();
    genre = (genre == "N/A" ? "" : genre);
    QString ext = queries::query.value("ext").toString();
    QString pages = queries::query.value("pages").toString();
    double size = changeBookSizeUnit(queries::query.value("size").toDouble(), ui->buttonSizeUnit->text());
    QString folder = queries::query.value("folder").toString();
    QString tags = queries::query.value("tags").toString();

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
        quint32 pages = ui->textDetailsPages->text().toUInt();

        queries::selectPathBasedonName(oldName);
        queries::query.next();
        QString path = queries::query.value(0).toString();
        if (newName != oldName)
        {
            yesNoDialog dialog(this, "Rename File", "Rename File", "Do you wish to rename the file on your hard drive as well?");
            common::openDialog(&dialog, ":/styles/style.qss");
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
    quint32 sizeTo = ui->spinBoxToSizeCriteria->value();
    quint32 sizeFrom = ui->spinBoxFromSizeCriteria->value();
    QString sizeIn = ui->buttonSizeCriteria->text();
    quint32 pagesTo = ui->spinBoxToPagesCriteria->value();
    quint32 pagesFrom = ui->spinBoxFromPagesCriteria->value();

    searchNameDialog dialog(this);
    common::openDialog(&dialog, ":/styles/style.qss");
    if (!dialog.searchName.isEmpty())
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
        ui->comboBoxFolderCriteria->setCurrentText(queries::query.value("folder").toString());
        ui->comboBoxAuthorCriteria->setCurrentText(queries::query.value("author").toString());
        ui->comboBoxGenreCriteria->setCurrentText(queries::query.value("genre").toString());
        ui->textTagsCriteria->setText(queries::query.value("tags").toString());
        ui->textExts->setText(queries::query.value("ext").toString());
        ui->spinBoxFromSizeCriteria->setValue(queries::query.value("size_from").toUInt());
        ui->spinBoxToSizeCriteria->setValue(queries::query.value("size_to").toUInt());
        ui->buttonSizeCriteria->setText(queries::query.value("size_in").toString());
        ui->spinBoxFromPagesCriteria->setValue(queries::query.value("pages_from").toUInt());
        ui->spinBoxToPagesCriteria->setValue(queries::query.value("pages_to").toUInt());

        ui->statusBar->showMessage("Search loaded.");
    }
}

void MainWindow::on_actionCleanEbooks_triggered()
{
    cleanEbooksDialog dialog(this);
    common::openDialog(&dialog, ":/styles/style.qss");
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
    quint32 numberOfEbooks = queries::query.value(0).toUInt();
    if (numberOfEbooks != 0)
    {
        srand((unsigned) time(0));
        quint32 randomNumber = 1 + (rand() % numberOfEbooks);
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
    {
        on_ebooksListWidget_itemClicked(ui->ebooksListWidget->currentItem());
    }
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
    ui->buttonSizeCriteria->setText(sizeUnits[currentText]);
}

void MainWindow::extSelectionSetup(const QString &title, const QString &prompt, QWidget *widget)
{
    QVector<QString> results;
    while (queries::query.next())
    {
        QString value = queries::query.value(0).toString();
        if (!results.contains(value))
        {
            results.push_back(value);
        }
    }

    auto *dialog = new extSelectionDialog(this, results, title, prompt);
    common::openDialog(dialog, ":/styles/style.qss");

    results = dialog->getExtVector();
    QString resultString = results.join(common::SEP);

    if (QComboBox *cb = qobject_cast<QComboBox*> (widget))
    {
        cb->setCurrentText(resultString);
    }
    else if (QLineEdit *le = qobject_cast<QLineEdit*> (widget))
    {
        le->setText(resultString);
    }
}

void MainWindow::on_buttonExtensions_clicked()
{
    queries::selectExt();
    extSelectionSetup("Extensions", "Select Available Extensions", ui->textExts);
}

void MainWindow::on_buttonFolder_clicked()
{
    queries::selectFoldersQuery();
    extSelectionSetup("Folders", "Select Available Folders", ui->comboBoxFolderCriteria);
}

void MainWindow::on_buttonAuthor_clicked()
{
    queries::selectAuthorsQuery();
    extSelectionSetup("Authors", "Select Available Authors", ui->comboBoxAuthorCriteria);
}

void MainWindow::on_buttonGenre_clicked()
{
    queries::selectGenreQuery();
    extSelectionSetup("Genres", "Select Available Genres", ui->comboBoxGenreCriteria);
}

void MainWindow::on_buttonTags_clicked()
{
    QVector<QString> tags;
    queries::selectTags();
    while (queries::query.next())
    {
        QString tagString = queries::query.value(0).toString();
        QStringList tagList = tagString.split(common::SEP);
        for (QString &tag: tagList)
        {
            if (!tags.contains(tag))
            {
                tags.push_back(tag);
            }
        }
    }

    auto *dialog = new extSelectionDialog(this, tags, "Tags", "Select Available Tags");
    common::openDialog(dialog, ":/styles/style.qss");

    tags = dialog->getExtVector();
    QString tagString = tags.join(common::SEP);
    ui->textTagsCriteria->setText(tagString);
}

void MainWindow::on_buttonSizeUnit_clicked()
{
    QString currentText = ui->buttonSizeUnit->text();
    ui->buttonSizeUnit->setText(sizeUnits[currentText]);
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

void MainWindow::on_actionWindowTop_triggered()
{
    Qt::WindowFlags flags = windowFlags();
    flags ^= Qt::WindowStaysOnTopHint;
    setWindowFlags(flags);
    show();
}

void MainWindow::on_buttonDbViewer_clicked()
{
    auto *summaryWindow = new DataViewerWindow();
    common::openWindow(summaryWindow, ":/styles/style.qss");
}

void MainWindow::on_actionSummaries_triggered()
{
    on_buttonSummaries_clicked();
}

void MainWindow::on_actionLinkManager_triggered()
{
    on_buttonLinkManager_clicked();
}

void MainWindow::on_actionDataViewer_triggered()
{
    on_buttonDbViewer_clicked();
}
