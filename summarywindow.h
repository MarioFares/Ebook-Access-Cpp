#ifndef SUMMARYWINDOW_H
#define SUMMARYWINDOW_H

#include <QHash>
#include <QTextList>
#include <QMainWindow>
#include <QTextBrowser>
#include <QListWidgetItem>

namespace Ui {
class SummaryWindow;
}

class SummaryWindow : public QMainWindow
{
    Q_OBJECT


public:
    void callSelectEbookSummary(QString name)
    {
        selectEbookSummary(name);
    }

    explicit SummaryWindow(QWidget *parent = nullptr);
    ~SummaryWindow();


private slots:
    bool eventFilter(QObject *obj, QEvent *event);

    void closeEvent(QCloseEvent *event);

    void selectEbookSummary(QString name);

    void createList(QTextListFormat::Style style);

    int changeListIndentation(int increment);

    void searchEbooks(QString textToSearch);

    bool handleBackspace();

    bool handleReturn();

    void insertClosingChar(char openChar, QString selectedText);

    void on_buttonBold_clicked();

    void on_buttonItalic_clicked();

    void on_buttonUnderline_clicked();

    void on_buttonThrough_clicked();

    void on_fontComboBox_currentTextChanged(const QString &arg1, bool change = true);

    void on_spinBoxFontSize_valueChanged(int arg1, bool change = true);

    void on_buttonBullets_clicked();

    void on_buttonUndo_clicked();

    void on_buttonRedo_clicked();

    void on_buttonClearText_clicked();

    void on_buttonOrder_clicked();

    void on_buttonIncreaseFontSize_clicked();

    void on_buttonDecreaseFontSize_clicked();

    void on_buttonIncreaseIndent_clicked();

    void on_buttonDecreaseIndent_clicked();

    void on_comboBoxAlignment_currentIndexChanged(int index);

    void on_buttonSubscript_clicked();

    void on_buttonSuperscript_clicked();

    void on_buttonCode_clicked();

    void on_buttonHighlight_clicked();

    void on_buttonEditorFontColor_clicked();

    void on_actionHideSearchBar_triggered();

    void on_actionHideListWidget_triggered();

    void on_actionHideLeftPane_triggered();

    void on_actionHideTopToolbar_triggered();

    void on_actionHideRightToolbar_triggered();

    void on_actionHideRightPane_triggered();

    void on_buttonInsertLine_clicked();

    void on_actionResetFormat_triggered();

    void on_buttonInsertImage_clicked();

    void on_actionPaste_triggered();

    void on_actionSearchText_triggered();

    void on_actionClearSearch_triggered();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_actionPrint_triggered();

    void on_buttonTable_clicked();

    void on_buttonLink_clicked();

    void on_actionCopyFormatting_triggered();

    void on_actionPasteMatchFormat_triggered();

    void on_actionSaveSummary_triggered();

    void on_textEditor_currentCharFormatChanged();

    void on_actionFullscreen_triggered();

    void on_actionExportHtml_triggered();

    void on_buttonEditorBackColor_clicked();

private:
    Ui::SummaryWindow *ui;
    QTextCharFormat currentCopiedFormat;
    QFont currentCopiedFont;
    QHash<char, char> charPairs;
};



#endif // SUMMARYWINDOW_H
