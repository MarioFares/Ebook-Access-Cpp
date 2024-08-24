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
    QHeaderView::ResizeMode m_columnsResizeMode;
    QHash<int, QString> m_hiddenColumns;

    // Widgets
    QWidget *m_centralWidget;
    QFrame *m_frameSearch;
    QComboBox *m_comboBoxTables;
    QToolButton *m_buttonRefresh;
    QToolButton *m_buttonToggleColors;
    QToolButton *m_buttonToggleGrid;
    QToolButton *m_buttonToggleFitColumns;
    QToolButton *m_buttonFontColor;
    QToolButton *m_buttonBackColor;
    QToolButton *m_buttonGridStyle;
    QTableWidget *m_tableWidget;
    QMenuBar *m_menubar;
    QStatusBar *m_statusbar;

    // Layouts
    QVBoxLayout *m_vertLayMain;
    QHBoxLayout *m_horLayComboBox;
    QHBoxLayout *m_horLayButtons;

    // Spacers
    QSpacerItem *m_horSpacerComboBoxLeft;
    QSpacerItem *m_horSpacerComboBoxRight;
    QSpacerItem *m_horSpacerButtonsLeft;
    QSpacerItem *m_horSpacerButtonsRight;
};

#endif // DATAVIEWERWINDOW_H
