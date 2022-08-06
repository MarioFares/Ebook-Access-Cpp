#ifndef SUMMARYWINDOW_H
#define SUMMARYWINDOW_H

#include "include/texteditor.h"
#include "include/findhighlighter.h"
#include "include/booksearchwidget.h"

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

class SummaryWindow : public QMainWindow
{
Q_OBJECT

public:
	explicit SummaryWindow(QWidget* parent = nullptr);

	void selectEbookSummary(const QString& name);

private slots:

	void setupInterface();

	void setupConnections();

	void setupActions();

	void setupMenus();

	void setupTabOrder();

	void setupShortcuts();

	void closeEvent(QCloseEvent* event) override;

	void toggleBold();

	void toggleItalic();

	void toggleUnderline();

	void toggleStrikethrough();

	void setTextFontFamily(const QString& arg1, bool change = true);

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

	void openEbookSummary(const QString& name);

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

	void findText(const QString& arg1);

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
	QTextCharFormat m_currentCopiedFormat;
	QFont m_currentCopiedFont;
	FindHighlighter m_textHighlighter;

	// Widgets
	QAction* m_actionUndo;
	QAction* m_actionRedo;
	QAction* m_actionBold;
	QAction* m_actionItalic;
	QAction* m_actionUnderline;
	QAction* m_actionThrough;
	QAction* m_actionIncFontSize;
	QAction* m_actionDecFontSize;
	QAction* m_actionIncIndent;
	QAction* m_actionDecIndent;
	QAction* m_actionSuperscript;
	QAction* m_actionSubscript;
	QAction* m_actionCopyFormatting;
	QAction* m_actionHideSearchBar;
	QAction* m_actionHideListWidget;
	QAction* m_actionHideLeftPane;
	QAction* m_actionHideTopToolbar;
	QAction* m_actionHideRightToolbar;
	QAction* m_actionHideRightPane;
	QAction* m_actionInsertLink;
	QAction* m_actionInsertImage;
	QAction* m_actionInsertTable;
	QAction* m_actionMaximize;
	QAction* m_actionMinimize;
	QAction* m_actionClose;
	QAction* m_actionInsertLine;
	QAction* m_actionPrintToPDF;
	QAction* m_actionCopy;
	QAction* m_actionPaste;
	QAction* m_actionPasteMatchFormat;
	QAction* m_actionSelectAll;
	QAction* m_actionCut;
	QAction* m_actionClearAll;
	QAction* m_actionResetFormat;
	QAction* m_actionZoomIn;
	QAction* m_actionZoomOut;
	QAction* m_actionSearchText;
	QAction* m_actionClearSearch;
	QAction* m_actionHighlight;
	QAction* m_actionCodeBlock;
	QAction* m_actionBulletedList;
	QAction* m_actionOrderedList;
	QAction* m_actionSaveSummary;
	QAction* m_actionFullscreen;
	QAction* m_actionExportHtml;
	QAction* m_actionSentenceCase;
	QAction* m_actionUpperCase;
	QAction* m_actionLowerCase;
	QAction* m_actionCapitalCase;
	QAction* m_actionSelectCurrentLine;
	QAction* m_actionDuplicateCurrentLine;
	QAction* m_actionDeleteCurrentLine;
	QAction* m_actionDateNormal;
	QAction* m_actionDateShort;
	QAction* m_actionTableRow;
	QAction* m_actionTableColumn;
	QAction* m_actionFind;
	QWidget* m_centralWidget;
	QSplitter* m_mainSplitterPane;
	QFrame* m_frameEditor;
	QLabel* m_labelTitle;
	QFrame* m_frameEditorToolBox;
	QFontComboBox* m_fontComboBox;
	QSpinBox* m_spinBoxFontSize;
	QToolButton* m_buttonEditorFontColor;
	QToolButton* m_buttonEditorBackColor;
	QFrame* m_lineVert1;
	QToolButton* m_buttonBold;
	QToolButton* m_buttonItalic;
	QToolButton* m_buttonUnderline;
	QToolButton* m_buttonThrough;
	QToolButton* m_buttonHighlight;
	QToolButton* m_buttonCodeBlock;
	QFrame* m_lineVert2;
	QToolButton* m_buttonBulletedList;
	QToolButton* m_buttonOrderedList;
	QComboBox* m_comboBoxAlignment;
	QFrame* m_lineVert3;
	QToolButton* m_buttonInsertTable;
	QToolButton* m_buttonInsertImage;
	QToolButton* m_buttonInsertLine;
	QToolButton* m_buttonInsertLink;
	TextEditor* m_textEditor;
	QFrame* m_frameFind;
	QLineEdit* m_textFind;
	QPushButton* m_buttonNext;
	QPushButton* m_buttonPrevious;
	QPushButton* m_buttonCloseFind;
	QLabel* m_labelFindMatchNum;
	QFrame* m_frameRightToolBar;
	QToolButton* m_buttonUndo;
	QToolButton* m_buttonRedo;
	QToolButton* m_buttonClearAll;
	QFrame* m_horLine1;
	QToolButton* m_buttonSuperscript;
	QToolButton* m_buttonSubscript;
	QFrame* m_horLine2;
	QToolButton* m_buttonIncFontSize;
	QToolButton* m_buttonDecFontSize;
	QFrame* m_horLine3;
	QToolButton* m_buttonIncIndent;
	QToolButton* m_buttonDecIndent;
	QFrame* m_horLine4;
	QStatusBar* m_statusBar;
	QMenuBar* m_menuBar;
	QMenu* m_menuFile;
	QMenu* m_menuEdit;
	QMenu* m_menuView;
	QMenu* m_menuInsert;
	QMenu* m_menuDateTime;
	QMenu* m_menuFormat;
	QMenu* m_menuChangeCase;
	QMenu* m_menuSearch;
	BookSearchWidget* m_bookSearchWidget;

	// Layouts
	QHBoxLayout* m_horLayMain;
	QVBoxLayout* m_vertLayEditor;
	QHBoxLayout* m_horLayToolBox;
	QHBoxLayout* m_horLayFind;
	QVBoxLayout* m_vertLayToolBar;

	// Spacers
	QSpacerItem* m_horSpacerTopToolBar;
	QSpacerItem* m_horSpacerFind;
	QSpacerItem* m_vertSpacerRightToolBar;
};

#endif // SUMMARYWINDOW_H
