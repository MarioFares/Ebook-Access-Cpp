#include "ui_mainwindow.h"

#include "include/common.h"
#include "include/queries.h"
#include "include/mainwindow.h"
#include "include/quotedialog.h"
#include "include/yesnodialog.h"
#include "include/summarywindow.h"
#include "include/getnamedialog.h"
#include "include/addbookdialog.h"
#include "include/settingswindow.h"
#include "include/addbooksdialog.h"
#include "include/dataviewerwindow.h"
#include "include/bookdetailswindow.h"
#include "include/cleanebooksdialog.h"
#include "include/linkmanagerwindow.h"
#include "include/extselectiondialog.h"

#include <QDir>
#include <QFile>
#include <QTimer>
#include <QProcess>
#include <QSqlRecord>
#include <QToolButton>
#include <QSystemTrayIcon>
#include <QRandomGenerator>
#include <QDesktopServices>
#include <QOperatingSystemVersion>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    // Setup DB
    queries::connectToDatabase();
    queries::setupDb();
    queries::logSessionStart();

    // Create dirs
    QDir dir;
    dir.mkdir("./reports");
    dir.mkdir("./reports/ebooks");
    dir.mkdir("./reports/usage");

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
    ui->comboBoxSearchLoad->setContextMenuPolicy(Qt::CustomContextMenu);

    // Refresh ComboBoxes
    refreshFolders();
    refreshAuthors();
    refreshGenres();
    refreshSearches();

    // Setup Tray Icon
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(windowIcon());
    trayIcon->setVisible(true);
    trayIcon->setToolTip("Ebook Access\nClick to Open");
    trayIcon->show();

    // Set up clear buttons
    ui->textExts->setClearButtonEnabled(true);
    ui->textTagsCriteria->setClearButtonEnabled(true);
    ui->textSearchBar->setClearButtonEnabled(true);

//    QTimer::singleShot(1500, this, SLOT(showQuote()));

    ui->statusBar->showMessage("Ready");
    setupConnections();
}

MainWindow::~MainWindow()
{
    queries::logSessionEnd();
    queries::query.clear();
    queries::db.close();
    delete ui;
}


void MainWindow::setupConnections()
{
    connect(ui->ebooksListWidget, &QListWidget::customContextMenuRequested, this, &MainWindow::showContextMenu);
    trayIcon->connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(show()));
    connect(ui->comboBoxSearchLoad, &QComboBox::customContextMenuRequested, this, &MainWindow::showSearchBoxContextMenu);


    connect(ui->buttonDetailsClear, &QPushButton::clicked, this, &MainWindow::clearDetails);
    connect(ui->buttonDetailsRestore, &QPushButton::clicked, this, &MainWindow::restoreDetails);
    connect(ui->buttonDetailsUpdate, &QPushButton::clicked, this, &MainWindow::updateDetails);

    connect(ui->buttonAddBooks, &QToolButton::clicked, this, &MainWindow::showAddBooksDialog);
    connect(ui->buttonSearchString, &QToolButton::clicked, this, &MainWindow::searchString);
    connect(ui->buttonClearCriteria, &QToolButton::clicked, this, &MainWindow::clearCriteria);
    connect(ui->buttonSortSearch, &QToolButton::clicked, this, &MainWindow::sortSearch);
    connect(ui->buttonAddBook, &QToolButton::clicked, this, &MainWindow::showAddBookDialog);
    connect(ui->buttonClearSearch, &QToolButton::clicked, this, &MainWindow::clearSearch);
    connect(ui->buttonSearchCriteria, &QToolButton::clicked, this, &MainWindow::searchCriteria);
    connect(ui->buttonSaveCriteria, &QToolButton::clicked, this, &MainWindow::saveCriteria);
    connect(ui->buttonSearchLoad, &QToolButton::clicked, this, &MainWindow::loadSearch);
    connect(ui->buttonLinkManager, &QToolButton::clicked, this, &MainWindow::showLinkManager);
    connect(ui->buttonDbViewer, &QToolButton::clicked, this, &MainWindow::showDbViewer);
    connect(ui->buttonSizeCriteria, &QToolButton::clicked, this, &MainWindow::toggleSizeCriteria);
    connect(ui->buttonExtensions, &QToolButton::clicked, this, &MainWindow::selectExtensions);
    connect(ui->buttonFolder, &QToolButton::clicked, this, &MainWindow::selectFolders);
    connect(ui->buttonAuthor, &QToolButton::clicked, this, &MainWindow::selectAuthors);
    connect(ui->buttonGenre, &QToolButton::clicked, this, &MainWindow::selectGenres);
    connect(ui->buttonTags, &QToolButton::clicked, this, &MainWindow::selectTags);
    connect(ui->buttonSizeUnit, &QToolButton::clicked, this, &MainWindow::toggleSizeUnit);
    connect(ui->buttonSummaries, &QToolButton::clicked, [this] { showSummary(""); });

    connect(ui->ebooksListWidget, &QListWidget::itemClicked, this, &MainWindow::showEbookDetails);
    connect(ui->ebooksListWidget, &QListWidget::itemActivated, this, &MainWindow::openEbook);
    connect(ui->ebooksListWidget, &QListWidget::itemSelectionChanged, [this]
    {
        if (ui->ebooksListWidget->count() >= 1)
        {
            showEbookDetails(ui->ebooksListWidget->currentItem());
        }
    });

    connect(ui->comboBoxSearchLoad, &QComboBox::currentTextChanged, this, &MainWindow::loadSearch);
    connect(ui->textTagsCriteria, &QLineEdit::returnPressed, this, &MainWindow::searchCriteria);
    connect(ui->textExts, &QLineEdit::returnPressed, this, &MainWindow::searchCriteria);
    connect(ui->textSearchBar, &QLineEdit::returnPressed, this, &MainWindow::searchString);
    connect(ui->textSearchBar, &QLineEdit::textChanged, this, &MainWindow::searchString);

    connect(ui->textDetailsName, &QLineEdit::returnPressed, this, &MainWindow::updateDetails);
    connect(ui->textDetailsAuthor, &QLineEdit::returnPressed, this, &MainWindow::updateDetails);
    connect(ui->textDetailsGenre, &QLineEdit::returnPressed, this, &MainWindow::updateDetails);
    connect(ui->textDetailsFolder, &QLineEdit::returnPressed, this, &MainWindow::updateDetails);
    connect(ui->textDetailsPages, &QLineEdit::returnPressed, this, &MainWindow::updateDetails);
    connect(ui->textDetailsTags, &QLineEdit::returnPressed, this, &MainWindow::updateDetails);

    connect(ui->actionBookDetails, &QAction::triggered, [this] { showBookDetailsWindow(""); });
    connect(ui->actionSearchFiles, &QAction::triggered, this, &MainWindow::searchCriteria);
    connect(ui->actionSearchText, &QAction::triggered, this, &MainWindow::searchString);
    connect(ui->actionSortSearch, &QAction::triggered, this, &MainWindow::sortSearch);
    connect(ui->actionResetEbooks, &QAction::triggered, this, &MainWindow::resetEbooks);
    connect(ui->actionHideSearchBar, &QAction::triggered, this, &MainWindow::hideSearchBar);
    connect(ui->actionHideDetailsSection, &QAction::triggered, this, &MainWindow::hideDetailsSection);
    connect(ui->actionHideRightFrame, &QAction::triggered, this, &MainWindow::hideRightFrame);
    connect(ui->actionHideUtilities, &QAction::triggered, this, &MainWindow::hideUtilities);
    connect(ui->actionHideLeftFrame, &QAction::triggered, this, &MainWindow::hideLeftFrame);
    connect(ui->actionCleanEbooks, &QAction::triggered, this, &MainWindow::showCleanEbooksDialog);
    connect(ui->actionChooseRandomBook, &QAction::triggered, this, &MainWindow::chooseRandomBook);
    connect(ui->actionFullscreen, &QAction::triggered, [this] { common::toggleFullscreen(this); });
    connect(ui->actionResetSummaries, &QAction::triggered, this, &MainWindow::resetSummaries);
    connect(ui->actionResetTags, &QAction::triggered, this, &MainWindow::resetTags);
    connect(ui->actionResetSearches, &QAction::triggered, this, &MainWindow::resetSearches);
    connect(ui->actionResetDatabase, &QAction::triggered, this, &MainWindow::resetDb);
    connect(ui->actionWindowTop, &QAction::triggered, this, &MainWindow::toggleWindowOnTop);
    connect(ui->actionLinkManager, &QAction::triggered, this, &MainWindow::showLinkManager);
    connect(ui->actionDataViewer, &QAction::triggered, this, &MainWindow::showDbViewer);
    connect(ui->actionApplicationSettings, &QAction::triggered, this, &MainWindow::showSettings);
    connect(ui->actionEbooksReport, &QAction::triggered, this, &MainWindow::genEbooksReport);
    connect(ui->actionHideStatusBar, &QAction::triggered, this, &MainWindow::hideStatusBar);
    connect(ui->actionClearSearch, &QAction::triggered, this, &MainWindow::clearSearch);
    connect(ui->actionMin, &QAction::triggered, this, &MainWindow::showMinimized);
    connect(ui->actionMax, &QAction::triggered, [this] { common::toggleMaximized(this); });
    connect(ui->actionClose, &QAction::triggered, this, &MainWindow::close);
    connect(ui->actionSummaries, &QAction::triggered, [this] { showSummary(""); });
    connect(ui->actionMinimizeTray, &QAction::triggered, [this]
    {
        QTimer::singleShot(0, this, SLOT(hide()));
    });
    connect(ui->actionOpenDB, &QAction::triggered, [this]
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile("./database.db"));
    });
    connect(ui->actionEbookReportsDir, &QAction::triggered, [this]
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile("./reports/ebooks/"));
    });
    connect(ui->actionUsageReportsDir, &QAction::triggered, [this]
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile("./reports/usage/"));
    });
    connect(ui->actionInstallationDir, &QAction::triggered, [this]
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile("./"));
    });
}

void MainWindow::showQuote()
{
    QuoteDialog dialog(this);
    common::openDialog(&dialog, ":styles/quote.qss");
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

void MainWindow::showBookDetailsWindow(const QString &name)
{
    BookDetailsWindow *bookDetailsWindow = new BookDetailsWindow();
    common::openWindow(bookDetailsWindow, ":/styles/style.qss");
    bookDetailsWindow->showBookDetails(name);
    bookDetailsWindow->setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::showSummary(const QString &name)
{
    SummaryWindow *summaryWindow = new SummaryWindow();
    common::openWindow(summaryWindow, ":/styles/style.qss");
    summaryWindow->selectEbookSummary(name);
    summaryWindow->setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::showLinkManager()
{
    LinkManagerWindow *linkManagerWindow = new LinkManagerWindow();
    common::openWindow(linkManagerWindow, ":/styles/style.qss");
    linkManagerWindow->setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::showDbViewer()
{
    DataViewerWindow *dataViewerWindow = new DataViewerWindow();
    common::openWindow(dataViewerWindow, ":/styles/style.qss");
    dataViewerWindow->setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::showSettings()
{
    SettingsWindow *settingsWindow = new SettingsWindow();
    common::openWindow(settingsWindow, ":/styles/style.qss");
    settingsWindow->setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::refreshComboBox(QComboBox *comboBox)
{
    comboBox->clear();
    comboBox->addItem("");
    while (queries::query.next())
    {
        QString value = queries::query.value(0).toString();
        if (value.isEmpty())
        {
            continue;
        }
        comboBox->addItem(value);
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
        openEbook(ui->ebooksListWidget->currentItem());
    });
    menu.addAction("Open Summary", this, SLOT(openSummaryWindow()));
    menu.addAction("Open MetaData", this, [this]
    {
        showBookDetailsWindow(ui->ebooksListWidget->currentItem()->text());
    });
    menu.addAction("Show in Folder", this, SLOT(openFolder()));
    menu.addAction("Delete Ebook", this, SLOT(deleteListItem()));

    if (!ui->ebooksListWidget->selectedItems().isEmpty())
    {
        menu.exec(globalPos);
    }
}

void MainWindow::showSearchBoxContextMenu(const QPoint &pos)
{
    QPoint globalPos = ui->comboBoxSearchLoad->mapToGlobal(pos);

    QMenu menu;
    menu.setStyleSheet(common::openSheet(":/styles/style.qss"));

    menu.addAction("Delete Search", this, [this]
    {
        QString item = ui->comboBoxSearchLoad->currentText();
        queries::deleteSearch(item);
        refreshSearches();
    });

    menu.exec(globalPos);
}

void MainWindow::openSummaryWindow()
{
    showSummary(ui->ebooksListWidget->currentItem()->text());
}

void MainWindow::deleteListItem()
{
    QString itemName = ui->ebooksListWidget->currentItem()->text();
    QListWidgetItem *currentItem = ui->ebooksListWidget->currentItem();

    YesNoDialog dialog(this, "Delete File", "Delete File", "Do you wish to delete the file on your hard drive as well?");
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
        QProcess *process = new QProcess(this);
        process->start("explorer.exe", args);
    }
    else
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile((file.dir().path())));
    }
}

void MainWindow::showAddBooksDialog()
{
    AddBooksDialog dialog(this);
    common::openDialog(&dialog, ":/styles/style.qss");

    refreshFolders();
    refreshAuthors();
    refreshGenres();
}

void MainWindow::searchString()
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

void MainWindow::clearCriteria()
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

void MainWindow::clearDetails()
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

void MainWindow::sortSearch()
{
    SORT = (SORT == Qt::AscendingOrder ? Qt::DescendingOrder : Qt::AscendingOrder);
    ui->ebooksListWidget->sortItems(SORT);
}

void MainWindow::resetEbooks()
{
    queries::resetEbooksTableQuery();
    refreshFolders();
    refreshAuthors();
    refreshGenres();
    ui->ebooksListWidget->clear();
    ui->statusBar->showMessage("Ebooks, summaries, and tags have been deleted.");
}


void MainWindow::hideSearchBar()
{
    common::changeWidgetVisibility(ui->frameSearchToolBar, ui->actionHideSearchBar);
}

void MainWindow::hideDetailsSection()
{
    ui->frameDetails->setHidden(!ui->frameDetails->isHidden());
    ui->actionHideDetailsSection->setText(ui->frameDetails->isHidden() ? "Show Details Section" : "Hide Details Section");
    ui->frameMainRight->setHidden(ui->ebooksListWidget->isHidden() && ui->frameDetails->isHidden());
}

void MainWindow::hideRightFrame()
{
    common::changeWidgetVisibility(ui->frameMainRight, ui->actionHideRightFrame);
}

void MainWindow::hideUtilities()
{
    common::changeWidgetVisibility(ui->frameBottomButtons, ui->actionHideUtilities);
}

void MainWindow::hideLeftFrame()
{
    common::changeWidgetVisibility(ui->frameMainLeft, ui->actionHideLeftFrame);
}

void MainWindow::showAddBookDialog()
{
    AddBookDialog dialog(this);
    common::openDialog(&dialog, ":/styles/style.qss");
    refreshFolders();
    refreshAuthors();
    refreshGenres();
}

void MainWindow::clearSearch()
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

void MainWindow::searchCriteria()
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

void MainWindow::showEbookDetails(QListWidgetItem *item)
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
    QString tags = queries::selectTagsBasedOnName(fileName);

    ui->textDetailsName->setText(fileName);
    ui->textDetailsName->setCursorPosition(0);
    ui->textDetailsAuthor->setText(author);
    ui->textDetailsGenre->setText(genre);
    ui->textDetailsPages->setText(pages);
    ui->textDetailsExt->setText(ext);
    ui->textDetailsSize->setText(QString::number(size));
    ui->textDetailsFolder->setText(folder);
    ui->textDetailsFolder->setCursorPosition(0);
    ui->textDetailsTags->setText(tags);
    ui->textDetailsTags->setCursorPosition(0);
    ui->statusBar->showMessage("Current selected ebook: " + fileName);
}

void MainWindow::restoreDetails()
{
    if (!ui->ebooksListWidget->selectedItems().isEmpty())
    {
        showEbookDetails(ui->ebooksListWidget->currentItem());
        ui->statusBar->showMessage("Details restored successfully.");
    }
}

void MainWindow::updateDetails()
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
            YesNoDialog dialog(this, "Rename File", "Rename File", "Do you wish to rename the file on your hard drive as well?");
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

void MainWindow::saveCriteria()
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

    GetNameDialog dialog(this, "Search Name", "Please provide a name for the search you want to save:");
    common::openDialog(&dialog, ":/styles/style.qss");
    if (!dialog.name.isEmpty())
    {
        queries::insertSearchQuery(dialog.name, folder, author, genre, tags, ext,
            sizeFrom, sizeTo, sizeIn, pagesFrom, pagesTo);
        ui->statusBar->showMessage("Search saved successfully.");
    }

    refreshSearches();
}

void MainWindow::loadSearch()
{
    if (ui->comboBoxSearchLoad->currentText().isEmpty())
    {
        clearCriteria();
        return;
    }

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

void MainWindow::showCleanEbooksDialog()
{
    CleanEbooksDialog dialog(this);
    common::openDialog(&dialog, ":/styles/style.qss");
}

void MainWindow::chooseRandomBook()
{
    queries::selectCountEbooks();
    queries::query.next();
    quint32 numberOfEbooks = queries::query.value(0).toUInt();
    if (numberOfEbooks != 0)
    {
        quint32 randomNumber = QRandomGenerator::global()->bounded(numberOfEbooks);
        queries::selectNameBasedOnRowid(randomNumber);
        queries::query.next();
        QString randomName = queries::query.value(0).toString();
        ui->textSearchBar->setText(randomName);
        ui->ebooksListWidget->setCurrentRow(0);
        ui->statusBar->showMessage("Random Book: " + randomName);
    }
}

void MainWindow::openEbook(QListWidgetItem *item)
{
    QString ebookName = item->text();
    queries::selectPathBasedonName(ebookName);
    queries::query.next();
    QString path = queries::query.value(0).toString();
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
    ui->statusBar->showMessage(ebookName + " ebook opened.");
}

void MainWindow::toggleSizeCriteria()
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

    ExtSelectionDialog *dialog = new ExtSelectionDialog(this, results, title, prompt);
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

void MainWindow::selectExtensions()
{
    queries::selectExt();
    extSelectionSetup("Extensions", "Select Available Extensions", ui->textExts);
}

void MainWindow::selectFolders()
{
    queries::selectFoldersQuery();
    extSelectionSetup("Folders", "Select Available Folders", ui->comboBoxFolderCriteria);
}

void MainWindow::selectAuthors()
{
    queries::selectAuthorsQuery();
    extSelectionSetup("Authors", "Select Available Authors", ui->comboBoxAuthorCriteria);
}

void MainWindow::selectGenres()
{
    queries::selectGenreQuery();
    extSelectionSetup("Genres", "Select Available Genres", ui->comboBoxGenreCriteria);
}

void MainWindow::selectTags()
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

    ExtSelectionDialog *dialog = new ExtSelectionDialog(this, tags, "Tags", "Select Available Tags");
    common::openDialog(dialog, ":/styles/style.qss");

    tags = dialog->getExtVector();
    QString tagString = tags.join(common::SEP);
    ui->textTagsCriteria->setText(tagString);
}

void MainWindow::toggleSizeUnit()
{
    QString currentText = ui->buttonSizeUnit->text();
    ui->buttonSizeUnit->setText(sizeUnits[currentText]);
    if (!ui->ebooksListWidget->selectedItems().isEmpty())
    {
        showEbookDetails(ui->ebooksListWidget->currentItem());
    }
}

void MainWindow::resetSummaries()
{
    queries::resetSummaries();
    ui->statusBar->showMessage("All summaries have been deleted.");
}

void MainWindow::resetTags()
{
    queries::resetTags();
    ui->statusBar->showMessage("All tags have been deleted.");

}

void MainWindow::resetSearches()
{
    queries::resetSearchesTable();
    refreshSearches();
    ui->statusBar->showMessage("All searches have been deleted.");
}

void MainWindow::resetDb()
{
    resetEbooks();
    resetSearches();
    ui->statusBar->showMessage("All data has been deleted.");
}

void MainWindow::toggleWindowOnTop()
{
    Qt::WindowFlags flags = windowFlags();
    flags ^= Qt::WindowStaysOnTopHint;
    setWindowFlags(flags);
    show();
}

void MainWindow::genEbooksReport()
{
    QString fileName = QDateTime::currentDateTime().toString("yyyy.MM.dd hh.mm.ss") + ".html";
    QFile file = QFile("./reports/ebooks/" + fileName);
    file.open(QIODevice::WriteOnly);
    file.write(R"(<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css" integrity="sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm" crossorigin="anonymous">)");
    file.write(R"(<table style="table-layout:fixed; width: 100vw; height: 100vh;" id="table" class="table table-responsive table-striped table-bordered table-hover table-sm table-dark">)");
    file.write(R"(  <thead class="bg-primary">)");
    file.write("    <tr>");
    file.write("        <th>Name</th>");
    file.write("        <th>Author</th>");
    file.write("        <th>Genre</th>");
    file.write("        <th>Path</th>");
    file.write("        <th>Extension</th>");
    file.write("        <th>Pages</th>");
    file.write("        <th>Folder</th>");
    file.write("        <th>Tags</th>");
    file.write("        <th>Timestamp</th>");
    file.write("    </tr>");
    file.write("  </thead>");
    file.write("  <tbody>");

    queries::query.exec("SELECT name, author, genre, path, ext, pages, folder, tags, time_added FROM ebooks");
    qint32 fieldCount = queries::query.record().count();
    while (queries::query.next())
    {
        file.write(R"(<tr style="overflow: hidden; text-overflow: ellipsis; word-wrap: break-word;">)");
        for (int i = 0; i < fieldCount; i++)
        {
            file.write(R"(<td style="overflow: hidden; text-overflow: ellipsis; word-wrap: break-word;">)");
            file.write(queries::query.value(i).toString().toStdString().c_str());
            file.write("</td>");
        }
        file.write("</tr>");
    }
    file.write("  </tbody>");
    file.write("</table>");
    file.close();
    QDesktopServices::openUrl(QUrl::fromLocalFile("./reports/ebooks/" + fileName));
}

void MainWindow::hideStatusBar()
{
    common::changeWidgetVisibility(ui->statusBar, ui->actionHideStatusBar);
}
