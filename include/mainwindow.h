#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFile>
#include <QList>
#include <QPointer>
#include <QComboBox>
#include <QMainWindow>
#include <QApplication>
#include <QListWidgetItem>
#include <QSystemTrayIcon>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void setupConnections();

    void closeEvent(QCloseEvent *event);

    void showBookDetailsWindow();

    void showSummary(const QString &name = "");

    void showLinkManager();

    void refreshComboBox(QComboBox *comboBox);

    void refreshFolders();

    void refreshAuthors();

    void refreshGenres();

    void refreshSearches();

    void openSummaryWindow();

    double changeBookSizeUnit(double size, const QString &unit);

    void showContextMenu(const QPoint&);

    void showSearchBoxContextMenu(const QPoint &pos);

    void trayClicked(QSystemTrayIcon::ActivationReason r);

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

    void showAddBookDialog();

    void clearSearch();

    void searchCriteria();

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

    void toggleSizeUnit();

    void resetSummaries();

    void resetTags();

    void resetSearches();

    void resetDb();

    void toggleWindowOnTop();

    void showDbViewer();

    void genEbooksReport();

    void hideStatusBar();


private:
    Ui::MainWindow *ui;
    Qt::SortOrder SORT;
    QHash<QString, quint32> sizeConvFactors;
    QHash<QString, QString> sizeUnits;
    QSystemTrayIcon *trayIcon;

};
#endif // MAINWINDOW_H
