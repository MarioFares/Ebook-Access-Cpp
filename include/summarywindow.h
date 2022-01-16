#ifndef SUMMARYWINDOW_H
#define SUMMARYWINDOW_H

#include <QHash>
#include <QTextList>
#include <QMainWindow>
#include <QTextBrowser>
#include <QListWidgetItem>

#include "include/findhighlighter.h"

namespace Ui {
class SummaryWindow;
}

class SummaryWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit SummaryWindow(QWidget *parent = nullptr);
    ~SummaryWindow();

    void selectEbookSummary(const QString &name);

private slots:
    void setupConnections();

    bool eventFilter(QObject *obj, QEvent *event);

    void closeEvent(QCloseEvent *event);

    void createList(QTextListFormat::Style style);

    int changeListIndentation(const int &increment);

    void searchEbooks(const QString &textToSearch);

    bool handleBackspace();

    bool handleReturn();

    void insertClosingChar(const char &openChar, const QString &selectedText);

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

    void openEbookSummary(QListWidgetItem *item);

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
    Ui::SummaryWindow *ui;
    QTextCharFormat currentCopiedFormat;
    QFont currentCopiedFont;
    QHash<char, char> charPairs;
    FindHighlighter textHighlighter;
};



#endif // SUMMARYWINDOW_H
