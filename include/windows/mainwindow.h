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
    Qt::SortOrder m_SORT;
    QHash<QString, quint32> m_sizeConvFactors;
    QHash<QString, QString> m_sizeUnits;
    QSystemTrayIcon *m_trayIcon;

    // Widgets
    QAction *m_actionAddBook;
    QAction *m_actionResetEbooks;
    QAction *m_actionResetDatabase;
    QAction *m_actionClearEntries;
    QAction *m_actionCleanEbooks;
    QAction *m_actionSearchFiles;
    QAction *m_actionSearchText;
    QAction *m_actionClearSearch;
    QAction *m_actionEbooksReport;
    QAction *m_actionUsageReport;
    QAction *m_actionApplicationSettings;
    QAction *m_actionFullscreen;
    QAction *m_actionMaximize;
    QAction *m_actionMinimize;
    QAction *m_actionClose;
    QAction *m_actionSortSearch;
    QAction *m_actionAddBooks;
    QAction *m_actionHideSearchBar;
    QAction *m_actionHideListWidget;
    QAction *m_actionHideDetailsSection;
    QAction *m_actionChooseRandomBook;
    QAction *m_actionHideRightFrame;
    QAction *m_actionHideUtilities;
    QAction *m_actionHideLeftFrame;
    QAction *m_actionMinimizeTray;
    QAction *m_actionResetSearches;
    QAction *m_actionResetSummaries;
    QAction *m_actionResetTags;
    QAction *m_actionWindowTop;
    QAction *m_actionSummaries;
    QAction *m_actionLinkManager;
    QAction *m_actionDataViewer;
    QAction *m_actionOpenDB;
    QAction *m_actionEbookReportsDir;
    QAction *m_actionUsageReportsDir;
    QAction *m_actionInstallationDir;
    QAction *m_actionHideStatusBar;
    QAction *m_actionBookDetails;
    QWidget *m_centralWidget;
    QFrame *m_frameMain;
    QSplitter *m_splitter;
    QFrame *m_frameMainLeft;
    QFrame *m_frameSearchCriteria;
    QLabel *m_labelGenreCriteria;
    QSpinBox *m_spinBoxToPagesCriteria;
    QLabel *m_labelPagesToCriteria;
    QLabel *m_labelSizeToCriteria;
    QLabel *m_labelAuthorCriteria;
    QLabel *m_labelSearchCriteria;
    QComboBox *m_comboBoxExtensionsCriteria;
    QSpinBox *m_spinBoxFromSizeCriteria;
    QLabel *m_labelTagsCriteria;
    QLabel *m_labelFolderCriteria;
    QComboBox *m_comboBoxSearchLoad;
    QComboBox *m_comboBoxTagsCriteria;
    QSpinBox *m_spinBoxToSizeCriteria;
    QSpinBox *m_spinBoxFromPagesCriteria;
    QComboBox *m_comboBoxFolderCriteria;
    QComboBox *m_comboBoxGenreCriteria;
    QComboBox *m_comboBoxAuthorCriteria;
    QLabel *m_labelSizeCriteria;
    QLabel *m_labelPagesCriteria;
    QLabel *m_labelExtensionCriteria;
    QToolButton *m_buttonSearchCriteria;
    QToolButton *m_buttonAuthor;
    QToolButton *m_buttonClearCriteria;
    QToolButton *m_buttonExtensions;
    QToolButton *m_buttonFolder;
    QToolButton *m_buttonGenre;
    QToolButton *m_buttonSaveCriteria;
    QToolButton *m_buttonSearchLoad;
    QToolButton *m_buttonSizeCriteria;
    QToolButton *m_buttonTags;
    QFrame *m_frameBottomButtons;
    QFrame *m_frameAddButtons;
    QToolButton *m_buttonAddBook;
    QToolButton *m_buttonAddBooks;
    QFrame *m_frameInfoButtons;
    QToolButton *m_buttonSummaries;
    QToolButton *m_buttonLinkManager;
    QToolButton *m_buttonDbViewer;
    QFrame *m_frameMainRight;
    QFrame *m_frameSearchToolBar;
    QLabel *m_labelSearchBar;
    QLineEdit *m_textSearchBar;
    QToolButton *m_buttonSearchString;
    QToolButton *m_buttonClearSearch;
    QToolButton *m_buttonSortSearch;
    QListWidget *m_ebooksListWidget;
    QFrame *m_frameDetails;
    QLabel *m_labelDetailsSize;
    QLineEdit *m_textDetailsGenre;
    QFrame *m_frameSizeDetails;
    QLineEdit *m_textDetailsSize;
    QPushButton *m_buttonSizeUnit;
    QLineEdit *m_textDetailsTags;
    QLabel *m_labelDetailsTags;
    QLabel *m_labelDetailsName;
    QLineEdit *m_textDetailsAuthor;
    QLineEdit *m_textDetailsFolder;
    QLineEdit *m_textDetailsExt;
    QLabel *m_labelDetailsExt;
    QLabel *m_labelDetailsPages;
    QLabel *m_labelDetailsGenre;
    QLabel *m_labelDetailsFolder;
    QLabel *m_labelDetailsAuthor;
    QLineEdit *m_textDetailsName;
    QLineEdit *m_textDetailsPages;
    QLabel *m_labelDetailsPublisher;
    QLineEdit *m_textDetailsPublisher;
    QLabel *m_labelDetailsDatePublished;
    QLineEdit *m_textDetailsDatePublished;
    QLabel *m_labelDetailsSeries;
    QLineEdit *m_textDetailsSeries;
    QLabel *m_labelDetailsRating;
    RatingComboBox *m_comboBoxDetailsRating;
    QLabel *m_labelDetailsStatus;
    StatusComboBox *m_comboBoxDetailsStatus;

    QPushButton *m_buttonDetailsRestore;
    QPushButton *m_buttonDetailsUpdate;
    QPushButton *m_buttonDetailsClear;
    QMenuBar *m_menubar;
    QMenu *m_menuFile;
    QMenu *m_menuGoTo;
    QMenu *m_menuAdd;
    QMenu *m_menuReset;
    QMenu *m_menuClean;
    QMenu *m_menuSearch;
    QMenu *m_menuGenerate;
    QMenu *m_menuSettings;
    QMenu *m_menuView;
    QMenu *m_menuTools;
    QStatusBar *m_statusBar;
    QFrame *m_frameGridLayCriteria;
    QScrollArea *m_scrollAreaCriteria;
    QLabel *m_labelSeriesCriteria;
    QComboBox *m_comboBoxSeriesCriteria;
    QLabel *m_labelPublisherCriteria;
    QComboBox *m_comboBoxPublisherCriteria;
    QLabel *m_labelDatePublishedCriteria;
    QComboBox *m_comboBoxDatePublishedCriteria;
    QLabel *m_labelRatingCriteria;
    QComboBox *m_comboBoxRatingCriteria;
    QLabel *m_labelStatusCriteria;
    QComboBox *m_comboBoxStatusCriteria;
    QToolButton *m_buttonPublisher;
    QToolButton *m_buttonDatePublished;
    QToolButton *m_buttonSeries;
    QToolButton *m_buttonRating;
    QToolButton *m_buttonStatus;
    QFrame *m_frameDetailsFields;
    QScrollArea *m_scrollAreaDetailsFields;
    QLabel *m_labelDetailsComments;
    QPlainTextEdit *m_textEditDetailsComments;

    // Layouts
    QHBoxLayout *m_horLayCentral;
    QHBoxLayout *m_horLayMain;
    QVBoxLayout *m_vertLayMainLeft;
    QHBoxLayout *m_horLayCriteria;
    QGridLayout *m_gridLayCriteria;
    QHBoxLayout *m_horLayBottomButtons;
    QHBoxLayout *m_horLayAddButtons;
    QHBoxLayout *m_horLayToolButtons;
    QVBoxLayout *m_vertLayMainRight;
    QHBoxLayout *m_horLaySearchBar;
    QGridLayout *m_gridLayDetails;
    QVBoxLayout *m_vertLayDetails;
    QGridLayout *m_gridLayDetailsFields;
    QHBoxLayout *m_horLayDetailsSize;
    QHBoxLayout *m_horLayDetailsButtons;
    QVBoxLayout *m_vertLaySearchCriteria;

    // Spacers
    QSpacerItem *m_horSpacerButtonsLeft;
    QSpacerItem *m_horSpacerButtonsRight;
    QSpacerItem *m_vertSpacerSearchCriteriaTop;
};

#endif // MAINWINDOW_H
