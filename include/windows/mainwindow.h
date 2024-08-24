#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "include/widgets/ratingcombobox.h"
#include "include/widgets/statuscombobox.h"

#include <QFile>
#include <QList>
#include <QLabel>
#include <QPointer>
#include <QSpinBox>
#include <QComboBox>
#include <QSplitter>
#include <QBoxLayout>
#include <QScrollArea>
#include <QToolButton>
#include <QMainWindow>
#include <QPushButton>
#include <QApplication>
#include <QPlainTextEdit>
#include <QListWidgetItem>
#include <QSystemTrayIcon>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void setupInterface();

    void setupMenus();

    void setupActions();

    void setupConnections();

    void setupTabOrder();

    void setupShortcuts();

    void closeEvent(QCloseEvent *event) override;

    static void showBookDetailsWindow(const QString &name = "");

    static void showSummary(const QString &name = "");

    void showSettings();

    static void showLinkManager();

    static void refreshComboBox(QComboBox *comboBox);

    void refreshFolders();

    void refreshAuthors();

    void refreshGenres();

    void refreshTags();

    void refreshExtensions();

    void refreshPublishers();

    void refreshDatePublished();

    void refreshSeries();

    void refreshStatus();

    void refreshRatings();

    void refreshSearches();

    void refreshAll();

    void openSummaryWindow();

    double changeBookSizeUnit(double size, const QString &unit);

    void showContextMenu(const QPoint &);

    void showSearchBoxContextMenu(const QPoint &pos);

    void deleteListItem();

    void openFolder();

    void showQuote();

    void showAddBooksDialog();

    void searchString();

    void clearCriteria();

    void clearDetails();

    void sortSearch();

    void resetEbooks();

    void hideSearchBar();

    void hideDetailsSection();

    void hideListWidget();

    void showAddBookDialog();

    void clearSearch();

    void searchCriteria();

    QString convertValuesToIndices(QString values, std::function<QString(QString)> func);

    void showEbookDetails(QListWidgetItem *item);

    void restoreDetails();

    void updateDetails();

    void saveCriteria();

    void loadSearch();

    void showCleanEbooksDialog();

    void chooseRandomBook();

    void openEbook(QListWidgetItem *item);

    void toggleSizeCriteria();

    void hideRightFrame();

    void hideUtilities();

    void hideLeftFrame();

    void extSelectionSetup(const QString &title, const QString &prompt, QWidget *widget);

    void selectExtensions();

    void selectFolders();

    void selectAuthors();

    void selectGenres();

    void selectTags();

    void selectPublishers();

    void selectDatePublished();

    void selectSeries();

    void selectStatus();

    void selectRatings();

    void toggleSizeUnit();

    void resetSummaries();

    void resetTags();

    void resetSearches();

    void resetDb();

    void toggleWindowOnTop();

    static void showDbViewer();

    void genEbooksReport();

    void hideStatusBar();

    void editListItem(QListWidgetItem *item);

private:
    Qt::SortOrder _SORT;
    QHash<QString, quint32> _sizeConvFactors;
    QHash<QString, QString> _sizeUnits;
    QSystemTrayIcon *_trayIcon;

    // Widgets
    QAction *_actionAddBook;
    QAction *_actionResetEbooks;
    QAction *_actionResetDatabase;
    QAction *_actionClearEntries;
    QAction *_actionCleanEbooks;
    QAction *_actionSearchFiles;
    QAction *_actionSearchText;
    QAction *_actionClearSearch;
    QAction *_actionEbooksReport;
    QAction *_actionUsageReport;
    QAction *_actionApplicationSettings;
    QAction *_actionFullscreen;
    QAction *_actionMaximize;
    QAction *_actionMinimize;
    QAction *_actionClose;
    QAction *_actionSortSearch;
    QAction *_actionAddBooks;
    QAction *_actionHideSearchBar;
    QAction *_actionHideListWidget;
    QAction *_actionHideDetailsSection;
    QAction *_actionChooseRandomBook;
    QAction *_actionHideRightFrame;
    QAction *_actionHideUtilities;
    QAction *_actionHideLeftFrame;
    QAction *_actionMinimizeTray;
    QAction *_actionResetSearches;
    QAction *_actionResetSummaries;
    QAction *_actionResetTags;
    QAction *_actionWindowTop;
    QAction *_actionSummaries;
    QAction *_actionLinkManager;
    QAction *_actionDataViewer;
    QAction *_actionOpenDB;
    QAction *_actionEbookReportsDir;
    QAction *_actionUsageReportsDir;
    QAction *_actionInstallationDir;
    QAction *_actionHideStatusBar;
    QAction *_actionBookDetails;
    QWidget *_centralWidget;
    QFrame *_frameMain;
    QSplitter *_splitter;
    QFrame *_frameMainLeft;
    QFrame *_frameSearchCriteria;
    QLabel *_labelGenreCriteria;
    QSpinBox *_spinBoxToPagesCriteria;
    QLabel *_labelPagesToCriteria;
    QLabel *_labelSizeToCriteria;
    QLabel *_labelAuthorCriteria;
    QLabel *_labelSearchCriteria;
    QComboBox *_comboBoxExtensionsCriteria;
    QSpinBox *_spinBoxFromSizeCriteria;
    QLabel *_labelTagsCriteria;
    QLabel *_labelFolderCriteria;
    QComboBox *_comboBoxSearchLoad;
    QComboBox *_comboBoxTagsCriteria;
    QSpinBox *_spinBoxToSizeCriteria;
    QSpinBox *_spinBoxFromPagesCriteria;
    QComboBox *_comboBoxFolderCriteria;
    QComboBox *_comboBoxGenreCriteria;
    QComboBox *_comboBoxAuthorCriteria;
    QLabel *_labelSizeCriteria;
    QLabel *_labelPagesCriteria;
    QLabel *_labelExtensionCriteria;
    QToolButton *_buttonSearchCriteria;
    QToolButton *_buttonAuthor;
    QToolButton *_buttonClearCriteria;
    QToolButton *_buttonExtensions;
    QToolButton *_buttonFolder;
    QToolButton *_buttonGenre;
    QToolButton *_buttonSaveCriteria;
    QToolButton *_buttonSearchLoad;
    QToolButton *_buttonSizeCriteria;
    QToolButton *_buttonTags;
    QFrame *_frameBottomButtons;
    QFrame *_frameAddButtons;
    QToolButton *_buttonAddBook;
    QToolButton *_buttonAddBooks;
    QFrame *_frameInfoButtons;
    QToolButton *_buttonSummaries;
    QToolButton *_buttonLinkManager;
    QToolButton *_buttonDbViewer;
    QFrame *_frameMainRight;
    QFrame *_frameSearchToolBar;
    QLabel *_labelSearchBar;
    QLineEdit *_textSearchBar;
    QToolButton *_buttonSearchString;
    QToolButton *_buttonClearSearch;
    QToolButton *_buttonSortSearch;
    QListWidget *_ebooksListWidget;
    QFrame *_frameDetails;
    QLabel *_labelDetailsSize;
    QLineEdit *_textDetailsGenre;
    QFrame *_frameSizeDetails;
    QLineEdit *_textDetailsSize;
    QPushButton *_buttonSizeUnit;
    QLineEdit *_textDetailsTags;
    QLabel *_labelDetailsTags;
    QLabel *_labelDetailsName;
    QLineEdit *_textDetailsAuthor;
    QLineEdit *_textDetailsFolder;
    QLineEdit *_textDetailsExt;
    QLabel *_labelDetailsExt;
    QLabel *_labelDetailsPages;
    QLabel *_labelDetailsGenre;
    QLabel *_labelDetailsFolder;
    QLabel *_labelDetailsAuthor;
    QLineEdit *_textDetailsName;
    QLineEdit *_textDetailsPages;
    QLabel *_labelDetailsPublisher;
    QLineEdit *_textDetailsPublisher;
    QLabel *_labelDetailsDatePublished;
    QLineEdit *_textDetailsDatePublished;
    QLabel *_labelDetailsSeries;
    QLineEdit *_textDetailsSeries;
    QLabel *_labelDetailsRating;
    RatingComboBox *_comboBoxDetailsRating;
    QLabel *_labelDetailsStatus;
    StatusComboBox *_comboBoxDetailsStatus;

    QPushButton *_buttonDetailsRestore;
    QPushButton *_buttonDetailsUpdate;
    QPushButton *_buttonDetailsClear;
    QMenuBar *_menubar;
    QMenu *_menuFile;
    QMenu *_menuGoTo;
    QMenu *_menuAdd;
    QMenu *_menuReset;
    QMenu *_menuClean;
    QMenu *_menuSearch;
    QMenu *_menuGenerate;
    QMenu *_menuSettings;
    QMenu *_menuView;
    QMenu *_menuTools;
    QStatusBar *_statusBar;
    QFrame *_frameGridLayCriteria;
    QScrollArea *_scrollAreaCriteria;
    QLabel *_labelSeriesCriteria;
    QComboBox *_comboBoxSeriesCriteria;
    QLabel *_labelPublisherCriteria;
    QComboBox *_comboBoxPublisherCriteria;
    QLabel *_labelDatePublishedCriteria;
    QComboBox *_comboBoxDatePublishedCriteria;
    QLabel *_labelRatingCriteria;
    QComboBox *_comboBoxRatingCriteria;
    QLabel *_labelStatusCriteria;
    QComboBox *_comboBoxStatusCriteria;
    QToolButton *_buttonPublisher;
    QToolButton *_buttonDatePublished;
    QToolButton *_buttonSeries;
    QToolButton *_buttonRating;
    QToolButton *_buttonStatus;
    QFrame *_frameDetailsFields;
    QScrollArea *_scrollAreaDetailsFields;
    QLabel *_labelDetailsComments;
    QPlainTextEdit *_textEditDetailsComments;

    // Layouts
    QHBoxLayout *_horLayCentral;
    QHBoxLayout *_horLayMain;
    QVBoxLayout *_vertLayMainLeft;
    QHBoxLayout *_horLayCriteria;
    QGridLayout *_gridLayCriteria;
    QHBoxLayout *_horLayBottomButtons;
    QHBoxLayout *_horLayAddButtons;
    QHBoxLayout *_horLayToolButtons;
    QVBoxLayout *_vertLayMainRight;
    QHBoxLayout *_horLaySearchBar;
    QGridLayout *_gridLayDetails;
    QVBoxLayout *_vertLayDetails;
    QGridLayout *_gridLayDetailsFields;
    QHBoxLayout *_horLayDetailsSize;
    QHBoxLayout *_horLayDetailsButtons;
    QVBoxLayout *_vertLaySearchCriteria;

    // Spacers
    QSpacerItem *_horSpacerButtonsLeft;
    QSpacerItem *_horSpacerButtonsRight;
    QSpacerItem *_vertSpacerSearchCriteriaTop;
};

#endif // MAINWINDOW_H
