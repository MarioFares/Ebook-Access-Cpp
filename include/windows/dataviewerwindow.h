#ifndef DATAVIEWERWINDOW_H
#define DATAVIEWERWINDOW_H

#include <QComboBox>
#include <QBoxLayout>
#include <QMainWindow>
#include <QHeaderView>
#include <QToolButton>
#include <QTableWidget>

class DataViewerWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit DataViewerWindow(QWidget *parent = nullptr);

private slots:
    void setupInterface();

    void setupConnections();

    void setupTabOrder();

    void closeEvent(QCloseEvent *event) override;

    void populateTable();

    void tableSelected(int index);

    void toggleColors();

    void toggleGrid();

    void toggleFitColumns();

    void showCellText();

    void showTableContextMenu(const QPoint &pos);

    void hideColumn(int index);

    void showColumn(int index);

    void showAllColumns();

    void setupGridMenu();

    void setFontColor();

    void setBackColor();

private:
    QHeaderView::ResizeMode _columnsResizeMode;
    QHash<int, QString> _hiddenColumns;

    // Widgets
    QWidget *_centralWidget;
    QFrame *_frameSearch;
    QComboBox *_comboBoxTables;
    QToolButton *_buttonRefresh;
    QToolButton *_buttonToggleColors;
    QToolButton *_buttonToggleGrid;
    QToolButton *_buttonToggleFitColumns;
    QToolButton *_buttonFontColor;
    QToolButton *_buttonBackColor;
    QToolButton *_buttonGridStyle;
    QTableWidget *_tableWidget;
    QMenuBar *_menubar;
    QStatusBar *_statusbar;

    // Layouts
    QVBoxLayout *_vertLayMain;
    QHBoxLayout *_horLayComboBox;
    QHBoxLayout *_horLayButtons;

    // Spacers
    QSpacerItem *_horSpacerComboBoxLeft;
    QSpacerItem *_horSpacerComboBoxRight;
    QSpacerItem *_horSpacerButtonsLeft;
    QSpacerItem *_horSpacerButtonsRight;
};

#endif // DATAVIEWERWINDOW_H
