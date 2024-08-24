#ifndef SUMMARYWINDOW_H
#define SUMMARYWINDOW_H

#include "include/widgets/texteditor.h"
#include "include/widgets/findhighlighter.h"
#include "include/widgets/booksearchwidget.h"

#include <QLabel>
#include <QAction>
#include <QSpinBox>
#include <QMenuBar>
#include <QTextList>
#include <QSplitter>
#include <QStatusBar>
#include <QToolButton>
#include <QPushButton>
#include <QMainWindow>
#include <QTextBrowser>
#include <QFontComboBox>
#include <QListWidgetItem>

class SummaryWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit SummaryWindow(QWidget *parent = nullptr);

    void selectEbookSummary(const QString &name);

private slots:
    void setupInterface();

    void setupConnections();

    void setupActions();

    void setupMenus();

    void setupTabOrder();

    void setupShortcuts();

    void closeEvent(QCloseEvent *event) override;

    void toggleBold();

    void toggleItalic();

    void toggleUnderline();

    void toggleStrikethrough();

    void setTextFontFamily(const QString &arg1, bool change = true);

    void setTextFontSize(int arg1, bool change = true);

    void toggleBulletedList();

    void toggleOrderedList();

    void changeTextAlignment(int index);

    void setSubscript();

    void setSuperscript();

    void insertCodeBlock();

    void toggleHighlight();

    void setFontColor();

    void hideSearchBar();

    void hideLeftPane();

    void hideTopToolbar();

    void hideRightToolbar();

    void hideRightPane();

    void insertLine();

    void resetFormat();

    void insertImage();

    void searchText();

    void clearSearch();

    void openEbookSummary(const QString &name);

    void printSummaryToPdf();

    void insertTable();

    void insertLink();

    void copyFormatting();

    void pasteMatchFormat();

    void saveSummary();

    void textFormatChanged();

    void exportSummaryToHtml();

    void setBackColor();

    void sentenceCase();

    void upperCase();

    void lowerCase();

    void capitalCase();

    void selectCurrentLine();

    void duplicateCurrentLine();

    void deleteCurrentLine();

    void zoomIn();

    void zoomOut();

    void insertNormalDate();

    void insertShortDate();

    void addTableRow();

    void addTableColumn();

    void findText(const QString &arg1);

    void findNext();

    void findPrevious();

    // Find functionality
    void highlightText();

    void textEditRefreshHighlighter(int cursorIndex);

    void toggleFindWidget(bool visible);

    void increaseFontSize();

    void decreaseFontSize();

    void increaseIndent();

    void decreaseIndent();

private:
    QTextCharFormat _currentCopiedFormat;
    QFont _currentCopiedFont;
    FindHighlighter _textHighlighter;

    // Widgets
    QAction *_actionUndo;
    QAction *_actionRedo;
    QAction *_actionBold;
    QAction *_actionItalic;
    QAction *_actionUnderline;
    QAction *_actionThrough;
    QAction *_actionIncFontSize;
    QAction *_actionDecFontSize;
    QAction *_actionIncIndent;
    QAction *_actionDecIndent;
    QAction *_actionSuperscript;
    QAction *_actionSubscript;
    QAction *_actionCopyFormatting;
    QAction *_actionHideSearchBar;
    QAction *_actionHideListWidget;
    QAction *_actionHideLeftPane;
    QAction *_actionHideTopToolbar;
    QAction *_actionHideRightToolbar;
    QAction *_actionHideRightPane;
    QAction *_actionInsertLink;
    QAction *_actionInsertImage;
    QAction *_actionInsertTable;
    QAction *_actionMaximize;
    QAction *_actionMinimize;
    QAction *_actionClose;
    QAction *_actionInsertLine;
    QAction *_actionPrintToPDF;
    QAction *_actionCopy;
    QAction *_actionPaste;
    QAction *_actionPasteMatchFormat;
    QAction *_actionSelectAll;
    QAction *_actionCut;
    QAction *_actionClearAll;
    QAction *_actionResetFormat;
    QAction *_actionZoomIn;
    QAction *_actionZoomOut;
    QAction *_actionSearchText;
    QAction *_actionClearSearch;
    QAction *_actionHighlight;
    QAction *_actionCodeBlock;
    QAction *_actionBulletedList;
    QAction *_actionOrderedList;
    QAction *_actionSaveSummary;
    QAction *_actionFullscreen;
    QAction *_actionExportHtml;
    QAction *_actionSentenceCase;
    QAction *_actionUpperCase;
    QAction *_actionLowerCase;
    QAction *_actionCapitalCase;
    QAction *_actionSelectCurrentLine;
    QAction *_actionDuplicateCurrentLine;
    QAction *_actionDeleteCurrentLine;
    QAction *_actionDateNormal;
    QAction *_actionDateShort;
    QAction *_actionTableRow;
    QAction *_actionTableColumn;
    QAction *_actionFind;
    QWidget *_centralWidget;
    QSplitter *_mainSplitterPane;
    QFrame *_frameEditor;
    QLabel *_labelTitle;
    QFrame *_frameEditorToolBox;
    QFontComboBox *_fontComboBox;
    QSpinBox *_spinBoxFontSize;
    QToolButton *_buttonEditorFontColor;
    QToolButton *_buttonEditorBackColor;
    QFrame *_lineVert1;
    QToolButton *_buttonBold;
    QToolButton *_buttonItalic;
    QToolButton *_buttonUnderline;
    QToolButton *_buttonThrough;
    QToolButton *_buttonHighlight;
    QToolButton *_buttonCodeBlock;
    QFrame *_lineVert2;
    QToolButton *_buttonBulletedList;
    QToolButton *_buttonOrderedList;
    QComboBox *_comboBoxAlignment;
    QFrame *_lineVert3;
    QToolButton *_buttonInsertTable;
    QToolButton *_buttonInsertImage;
    QToolButton *_buttonInsertLine;
    QToolButton *_buttonInsertLink;
    TextEditor *_textEditor;
    QFrame *_frameFind;
    QLineEdit *_textFind;
    QPushButton *_buttonNext;
    QPushButton *_buttonPrevious;
    QPushButton *_buttonCloseFind;
    QLabel *_labelFindMatchNum;
    QFrame *_frameRightToolBar;
    QToolButton *_buttonUndo;
    QToolButton *_buttonRedo;
    QToolButton *_buttonClearAll;
    QFrame *_horLine1;
    QToolButton *_buttonSuperscript;
    QToolButton *_buttonSubscript;
    QFrame *_horLine2;
    QToolButton *_buttonIncFontSize;
    QToolButton *_buttonDecFontSize;
    QFrame *_horLine3;
    QToolButton *_buttonIncIndent;
    QToolButton *_buttonDecIndent;
    QFrame *_horLine4;
    QStatusBar *_statusBar;
    QMenuBar *_menuBar;
    QMenu *_menuFile;
    QMenu *_menuEdit;
    QMenu *_menuView;
    QMenu *_menuInsert;
    QMenu *_menuDateTime;
    QMenu *_menuFormat;
    QMenu *_menuChangeCase;
    QMenu *_menuSearch;
    BookSearchWidget *_bookSearchWidget;

    // Layouts
    QHBoxLayout *_horLayMain;
    QVBoxLayout *_vertLayEditor;
    QHBoxLayout *_horLayToolBox;
    QHBoxLayout *_horLayFind;
    QVBoxLayout *_vertLayToolBar;

    // Spacers
    QSpacerItem *_horSpacerTopToolBar;
    QSpacerItem *_horSpacerFind;
    QSpacerItem *_vertSpacerRightToolBar;
};

#endif // SUMMARYWINDOW_H
