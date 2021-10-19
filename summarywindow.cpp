#include "common.h"
#include "queries.h"
#include "summarywindow.h"
#include "ui_summarywindow.h"
#include "insertlinkdialog.h"
#include "inserttabledialog.h"

#include <fstream>

#include <QFile>
#include <QTextList>
#include <QTextTable>
#include <QFileDialog>
#include <QColorDialog>
#include <QDesktopServices>
#include <QtPrintSupport/QPrinter>

SummaryWindow::SummaryWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SummaryWindow)
{
    ui->setupUi(this);
    queries::connectToDatabase();
    ui->listWidget->setSortingEnabled(true);
    ui->textEditor->installEventFilter(this);
    ui->textEditor->setOpenExternalLinks(true);

    charPairs['('] = ')';
    charPairs['"'] = '"';
    charPairs['['] = ']';
    charPairs['{'] = '}';

    ui->textEditor->setStyleSheet(common::openSheet(":/textEditorStyle.qss"));
    searchEbooks("");
}

SummaryWindow::~SummaryWindow()
{
    if (!ui->labelTitle->text().isEmpty())
    {
        queries::updateSummary(ui->labelTitle->text(), ui->textEditor->toHtml());
    }
    delete ui;
}

void SummaryWindow::closeEvent(QCloseEvent *event)
{
    if (!ui->labelTitle->text().isEmpty())
    {
        queries::updateSummary(ui->labelTitle->text(), ui->textEditor->toHtml());
    }
    event->accept();
    delete this;
}

bool SummaryWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->textEditor && event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        switch (keyEvent->key())
        {
            case Qt::Key_Tab:
                this->changeListIndentation(1);
                return true;
            case Qt::Key_Backtab:
                this->changeListIndentation(-1);
                return true;
            case Qt::Key_ParenLeft:
                insertClosingChar('(', ui->textEditor->textCursor().selectedText());
                return true;
            case Qt::Key_QuoteDbl:
                insertClosingChar('"', ui->textEditor->textCursor().selectedText());
                return true;
            case Qt::Key_BracketLeft:
                insertClosingChar('[', ui->textEditor->textCursor().selectedText());
                return true;
            case Qt::Key_BraceLeft:
                insertClosingChar('{', ui->textEditor->textCursor().selectedText());
                return true;
            case Qt::Key_Backspace:
                return handleBackspace();
            case Qt::Key_Return:
                return handleReturn();
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

void SummaryWindow::insertClosingChar(char openChar, QString selectedText)
{
    QTextCursor cursor = ui->textEditor->textCursor();
    QString newText = openChar + selectedText + charPairs[openChar];
    cursor.removeSelectedText();
    cursor.insertText(newText);
    if (selectedText == "")
    {
        cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 1);
        ui->textEditor->setTextCursor(cursor);
    }
}

bool SummaryWindow::handleBackspace()
{
    QTextCursor cursor = ui->textEditor->textCursor();
    QTextBlock currentBlock = cursor.block();
    if (!cursor.selectedText().isEmpty())
    {
        cursor.clearSelection();
    }
    if (cursor.currentList() && currentBlock.text().isEmpty())
    {
        if(changeListIndentation(-1) == 0)
        {
            int index = ui->textEditor->textCursor().currentList()->itemNumber(currentBlock);
            cursor.currentList()->removeItem(index);
        }
    }
    else
    {
        ui->textEditor->textCursor().deletePreviousChar();
    }
    return true;
}

bool SummaryWindow::handleReturn()
{
    QTextCursor cursor = ui->textEditor->textCursor();
    QTextBlock currentBlock = cursor.block();
    if (!cursor.selectedText().isEmpty())
    {
        cursor.clearSelection();
    }
    if (cursor.currentList() && currentBlock.text().isEmpty())
    {
        if(changeListIndentation(-1) == 0)
        {
            int index = ui->textEditor->textCursor().currentList()->itemNumber(currentBlock);
            cursor.currentList()->removeItem(index);
        }
        return true;
    }
    else
    {
        return false;
    }
}

void SummaryWindow::selectEbookSummary(QString name)
{
    if (!name.isEmpty())
    {
        QList listItems = ui->listWidget->findItems(name, Qt::MatchExactly);
        on_listWidget_itemClicked(listItems[0]);
        ui->listWidget->setCurrentItem(listItems[0]);
    }
}

void SummaryWindow::searchEbooks(QString textToSearch)
{
    ui->listWidget->clear();
    queries::selectNameBasedOnString(textToSearch);
    while(queries::query.next())
    {
        ui->listWidget->addItem(queries::query.value(0).toString());
    }
}

void SummaryWindow::createList(QTextListFormat::Style listStyle)
{
    QTextCursor cursor = ui->textEditor->textCursor();
    cursor.setKeepPositionOnInsert(true);
    QTextList *currentList = cursor.currentList();
    if(currentList)
    {
        QTextListFormat listFormat;
        listFormat.setIndent(0);
        listFormat.setStyle(listStyle);
        currentList->setFormat(listFormat);
        for(int i = currentList->count() - 1; i >= 0 ; --i)
            currentList->removeItem( i );
    }
    else
    {
        QTextListFormat::Style style = listStyle;
        QTextListFormat listFormat;
        listFormat.setStyle(style);
        cursor.createList(listFormat);
    }
}

int SummaryWindow::changeListIndentation(int increment)
{
    QTextCursor cursor = ui->textEditor->textCursor();
    QTextList *currentList = cursor.currentList();

    if(currentList)
    {
        QTextListFormat listFormat;
        QTextListFormat::Style currentStyle = currentList->format().style();

        listFormat.setIndent(cursor.currentList()->format().indent() + increment);
        if (currentStyle == QTextListFormat::ListDisc || currentStyle == QTextListFormat::ListCircle || currentStyle == QTextListFormat::ListSquare)
        {
            if (listFormat.indent() == 1 || listFormat.indent() % 3 == 1){listFormat.setStyle(QTextListFormat::ListDisc);}
            if (listFormat.indent() == 2 || listFormat.indent() % 3 == 2){listFormat.setStyle(QTextListFormat::ListCircle);}
            if (listFormat.indent() % 3 == 0){listFormat.setStyle(QTextListFormat::ListSquare);}
        }
        if (currentStyle == QTextListFormat::ListDecimal || currentStyle == QTextListFormat::ListLowerAlpha || currentStyle == QTextListFormat::ListLowerRoman)
        {
            if (listFormat.indent() == 1 || listFormat.indent() % 3 == 1){listFormat.setStyle(QTextListFormat::ListDecimal);}
            if (listFormat.indent() == 2 || listFormat.indent() % 3 == 2){listFormat.setStyle(QTextListFormat::ListLowerAlpha);}
            if (listFormat.indent() % 3 == 0){listFormat.setStyle(QTextListFormat::ListLowerRoman);}
        }
        cursor.createList(listFormat);
        return listFormat.indent();
    }
    else
    {
        QTextBlockFormat blockFormat = cursor.block().blockFormat();
        blockFormat.setIndent(increment == 1 ? blockFormat.indent() + 1 : (blockFormat.indent() == 0 ? 0 : blockFormat.indent() - 1));
        cursor.setBlockFormat(blockFormat);
        return blockFormat.indent();
    }
}

void SummaryWindow::on_buttonBold_clicked()
{
    QFont currentFont = ui->textEditor->currentFont();
    currentFont.setBold(!currentFont.bold());
    ui->textEditor->setCurrentFont(currentFont);
    on_textEditor_currentCharFormatChanged();
}

void SummaryWindow::on_buttonItalic_clicked()
{
    QFont currentFont = ui->textEditor->currentFont();
    currentFont.setItalic(!currentFont.italic());
    ui->textEditor->setCurrentFont(currentFont);
    on_textEditor_currentCharFormatChanged();
}

void SummaryWindow::on_buttonUnderline_clicked()
{
    QFont currentFont = ui->textEditor->currentFont();
    currentFont.setUnderline(!currentFont.underline());
    ui->textEditor->setCurrentFont(currentFont);
    on_textEditor_currentCharFormatChanged();
}

void SummaryWindow::on_buttonThrough_clicked()
{
    QFont currentFont = ui->textEditor->currentFont();
    currentFont.setStrikeOut(!currentFont.strikeOut());
    ui->textEditor->setCurrentFont(currentFont);
    on_textEditor_currentCharFormatChanged();
}

void SummaryWindow::on_fontComboBox_currentTextChanged(const QString &arg1, bool change)
{
    if (change)
    {
        ui->textEditor->setCurrentFont(arg1);
    }
}

void SummaryWindow::on_spinBoxFontSize_valueChanged(int arg1, bool change)
{
    if (change)
    {
        QFont currentFont = ui->textEditor->currentFont();
        currentFont.setPointSize(arg1);
        ui->textEditor->setCurrentFont(currentFont);
    }
}

void SummaryWindow::on_buttonBullets_clicked()
{
    createList(QTextListFormat::ListDisc);
    on_textEditor_currentCharFormatChanged();
}

void SummaryWindow::on_buttonUndo_clicked()
{
    ui->textEditor->undo();
}

void SummaryWindow::on_buttonRedo_clicked()
{
    ui->textEditor->redo();
}

void SummaryWindow::on_buttonClearText_clicked()
{
    ui->textEditor->clear();
}

void SummaryWindow::on_buttonOrder_clicked()
{
    createList(QTextListFormat::ListDecimal);
    on_textEditor_currentCharFormatChanged();
}

void SummaryWindow::on_buttonIncreaseFontSize_clicked()
{
    ui->spinBoxFontSize->setValue(ui->spinBoxFontSize->value() + 1);
}

void SummaryWindow::on_buttonDecreaseFontSize_clicked()
{
    ui->spinBoxFontSize->setValue(ui->spinBoxFontSize->value() - 1);
}

void SummaryWindow::on_buttonIncreaseIndent_clicked()
{
    changeListIndentation(1);
}

void SummaryWindow::on_buttonDecreaseIndent_clicked()
{
    changeListIndentation(-1);
}

void SummaryWindow::on_comboBoxAlignment_currentIndexChanged(int index)
{
    switch (index)
    {
        case 0:
            ui->textEditor->setAlignment(Qt::AlignLeft);
            break;
        case 1:
            ui->textEditor->setAlignment(Qt::AlignHCenter);
            break;
        case 2:
            ui->textEditor->setAlignment(Qt::AlignRight);
            break;
        case 3:
            ui->textEditor->setAlignment(Qt::AlignJustify);
            break;
        default:
            ui->textEditor->setAlignment(Qt::AlignLeft);
            break;
    }
}

void SummaryWindow::on_buttonSubscript_clicked()
{
    QTextCharFormat currentFormat = ui->textEditor->currentCharFormat();
    currentFormat.setVerticalAlignment(QTextCharFormat::AlignSubScript);
    ui->textEditor->setCurrentCharFormat(currentFormat);
}

void SummaryWindow::on_buttonSuperscript_clicked()
{
    QTextCharFormat currentFormat = ui->textEditor->currentCharFormat();
    currentFormat.setVerticalAlignment(QTextCharFormat::AlignSuperScript);
    ui->textEditor->setCurrentCharFormat(currentFormat);
}

void SummaryWindow::on_buttonCode_clicked()
{
    QTextFrameFormat frameFormat;
    frameFormat.setBackground(QBrush(QColor(QString("#FBFAF8"))));
    frameFormat.setBorder(1);
    frameFormat.setBorderBrush(QBrush(QColor(QString("#D6D5D3"))));
    frameFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
    frameFormat.setMargin(0);
    frameFormat.setPadding(4);

    int fontSize = ui->textEditor->currentFont().pointSize();
    QFont font = QFont("Consolas");
    font.setPointSize(fontSize);

    ui->textEditor->textCursor().insertFrame(frameFormat);
    ui->textEditor->setCurrentFont(font);

}

void SummaryWindow::on_textEditor_currentCharFormatChanged()
{
    on_fontComboBox_currentTextChanged(ui->textEditor->currentFont().family(), false);
    on_spinBoxFontSize_valueChanged(ui->textEditor->currentFont().pointSize(), false);
    ui->buttonEditorFontColor->setStyleSheet("background-color: " + ui->textEditor->textColor().name());

    ui->buttonBold->setStyleSheet("background-color: " + QString(ui->textEditor->currentFont().bold() ? "blue" : "#2D2D30"));
    ui->buttonItalic->setStyleSheet("background-color: " + QString(ui->textEditor->currentFont().italic() ? "blue" : "#2D2D30"));
    ui->buttonUnderline->setStyleSheet("background-color: " + QString(ui->textEditor->currentFont().underline() ? "blue" : "#2D2D30"));
    ui->buttonThrough->setStyleSheet("background-color: " + QString(ui->textEditor->currentFont().strikeOut() ? "blue" : "#2D2D30"));
    ui->buttonHighlight->setStyleSheet("background-color: " + QString(ui->textEditor->currentCharFormat().background() == Qt::yellow ? "blue" : "#2D2D30"));
}

void SummaryWindow::on_buttonHighlight_clicked()
{
    QTextCharFormat currentFormat =  ui->textEditor->currentCharFormat();
    currentFormat.setBackground(currentFormat.background() == Qt::yellow ? Qt::white : Qt::yellow);
    ui->textEditor->setCurrentCharFormat(currentFormat);
    on_textEditor_currentCharFormatChanged();
}

void SummaryWindow::on_buttonEditorFontColor_clicked()
{
    QColorDialog dialog(this);
    common::openDialog(&dialog, ":/summarystyle.qss");
    QString colorSheet = "background-color: " + dialog.selectedColor().name();
    ui->buttonEditorFontColor->setStyleSheet(colorSheet);
    ui->buttonEditorFontColor->update();

    ui->textEditor->setTextColor(dialog.selectedColor());
}

void SummaryWindow::on_actionHideSearchBar_triggered()
{
    ui->textSearch->setHidden(!ui->textSearch->isHidden());
    ui->actionHideSearchBar->setText(ui->textSearch->isHidden() ? "Show Search Bar" : "Hide Search Bar");
}

void SummaryWindow::on_actionHideListWidget_triggered()
{
    ui->listWidget->setHidden(!ui->listWidget->isHidden());
    ui->actionHideListWidget->setText(ui->listWidget->isHidden() ? "Show List Widget" : "Hide List Widget");
}

void SummaryWindow::on_actionHideLeftPane_triggered()
{
    ui->frameListWidget->setHidden(!ui->frameListWidget->isHidden());
    ui->mainSplitterPane->refresh();
    ui->actionHideLeftPane->setText(ui->frameListWidget->isHidden() ? "Show Left Pane" : "Hide Left Pane");
}

void SummaryWindow::on_actionHideTopToolbar_triggered()
{
    ui->frameEditorToolBox->setHidden(!ui->frameEditorToolBox->isHidden());
    ui->actionHideTopToolbar->setText(ui->frameEditorToolBox->isHidden() ? "Show Top Toolbox" : "Hide Top Toolbox");
}

void SummaryWindow::on_actionHideRightToolbar_triggered()
{
    ui->frameRightToolBar->setHidden(!ui->frameRightToolBar->isHidden());
    ui->actionHideRightToolbar->setText(ui->frameRightToolBar->isHidden() ? "Show Right Toolbox" : "Hide Right Toolbox");

}

void SummaryWindow::on_actionHideRightPane_triggered()
{
    ui->frameEditor->setHidden(!ui->frameEditor->isHidden());
    ui->mainSplitterPane->refresh();
    on_actionHideRightToolbar_triggered();
    ui->actionHideRightPane->setText(ui->frameEditor->isHidden() ? "Show Right Pane" : "Hide Right Pane");

}

void SummaryWindow::on_buttonInsertLine_clicked()
{
    QTextCursor cursor = ui->textEditor->textCursor();
    cursor.insertHtml(QString("<hr><br>"));
    cursor.movePosition(QTextCursor::Up, QTextCursor::MoveAnchor, 1);
    ui->textEditor->setTextCursor(cursor);
}

void SummaryWindow::on_actionResetFormat_triggered()
{
    QFont currentFont = ui->textEditor->currentFont();
    QTextCharFormat currentFormat =  ui->textEditor->currentCharFormat();

    currentFont.setBold(false);
    currentFont.setItalic(false);
    currentFont.setStrikeOut(false);
    currentFont.setUnderline(false);
    currentFormat.setBackground(Qt::white);
    currentFormat.setVerticalAlignment(QTextCharFormat::AlignNormal);

    ui->textEditor->setCurrentCharFormat(currentFormat);
    ui->textEditor->setCurrentFont(currentFont);

    on_textEditor_currentCharFormatChanged();
}

void SummaryWindow::on_buttonInsertImage_clicked()
{
    QString file = QFileDialog::getOpenFileName(this,
        tr("Select Image"), "/", tr("Image Files (*.jpeg *.jpg *.png *.gif *.apng *.svg *.bmp *.ico)"));

      ui->textEditor->textCursor().insertHtml(QString(R"(<img src="%1" alt = ""/>)").arg( file ));
}

void SummaryWindow::on_actionPaste_triggered()
{
    ui->textEditor->paste();
}

void SummaryWindow::on_actionSearchText_triggered()
{
    if (ui->listWidget->selectedItems().count() != 0)
    {
        queries::updateSummary(ui->labelTitle->text(), ui->textEditor->toHtml());
    }

    QString stringToSearch = ui->textSearch->text();
    searchEbooks(stringToSearch);

}

void SummaryWindow::on_actionClearSearch_triggered()
{
    ui->textSearch->clear();
    ui->listWidget->clear();
}

void SummaryWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    if(!ui->labelTitle->text().isEmpty())
    {
        queries::updateSummary(ui->labelTitle->text(), ui->textEditor->toHtml());
    }
    ui->textEditor->clear();
    ui->labelTitle->setText(item->text());
    queries::selectSummaryBasedonName(item->text());
    queries::query.next();
    QString summary = queries::query.value(0).toString();
    if (summary != "N/A"){ui->textEditor->insertHtml(summary);}

}

void SummaryWindow::on_actionPrint_triggered()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save File"), "/", tr("*.pdf"));

    QPrinter printer(QPrinter::HighResolution);
    printer.setPrintRange(QPrinter::CurrentPage);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);
    ui->textEditor->print(&printer);
    QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
}

void SummaryWindow::on_buttonTable_clicked()
{
    insertTableDialog dialog(this);
    common::openDialog(&dialog, ":/style.qss");

    if (dialog.rowCount != 0 && dialog.columnCount != 0)
    {
        QTextTableFormat tableFormat;
        tableFormat.setBorder(1);
        tableFormat.setCellSpacing(5);
        tableFormat.setCellPadding(1);
        tableFormat.setBorderCollapse(false);
        tableFormat.setBorderStyle(QTextTableFormat::BorderStyle_Solid);
        ui->textEditor->textCursor().insertTable(dialog.rowCount, dialog.columnCount, tableFormat);
    }
}

void SummaryWindow::on_buttonLink_clicked()
{
    insertLinkDialog dialog(this);
    common::openDialog(&dialog, ":/style.qss");

    if (!dialog.title.isEmpty() && !dialog.link.isEmpty())
    {
       QTextCharFormat charFormat;
       charFormat.setFontUnderline(true);
       charFormat.setForeground(QColor("blue"));
       charFormat.setAnchor(true);
       charFormat.setAnchorHref(dialog.link);
       charFormat.setToolTip(dialog.link);
       ui->textEditor->textCursor().insertText(dialog.title, charFormat);
    }
}

void SummaryWindow::on_actionCopyFormatting_triggered()
{
    currentCopiedFormat = ui->textEditor->currentCharFormat();
    currentCopiedFont = ui->textEditor->currentFont();
}

void SummaryWindow::on_actionPasteMatchFormat_triggered()
{
    ui->textEditor->setCurrentCharFormat(currentCopiedFormat);
    ui->textEditor->setCurrentFont(currentCopiedFont);
}

void SummaryWindow::on_actionSaveSummary_triggered()
{
    if (!ui->labelTitle->text().isEmpty())
    {
        queries::updateSummary(ui->labelTitle->text(), ui->textEditor->toHtml());
    }
}

void SummaryWindow::on_actionFullscreen_triggered()
{
    !this->isFullScreen() ? this->showFullScreen() : this->showNormal();
}

void SummaryWindow::on_actionExportHtml_triggered()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save File"), "/", tr("*.html"));
    std::ofstream htmlFile(filePath.toStdString());
    htmlFile << ui->textEditor->toHtml().toStdString();
    htmlFile.close();
    QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
}


void SummaryWindow::on_buttonEditorBackColor_clicked()
{
    QColorDialog dialog(this);
    common::openDialog(&dialog, ":/summarystyle.qss");
    QString styleSheet = "background-color: " + dialog.selectedColor().name();

    QTextFrame *currentFrame = ui->textEditor->textCursor().currentFrame();
    QTextFrameFormat currentFormat = currentFrame->frameFormat();
    // Apply only to code box in case cursor in code box
    if (currentFormat.border() == 1)
    {
        currentFormat.setBackground(QBrush(dialog.selectedColor()));
        currentFrame->setFrameFormat(currentFormat);
        return;
    }
    // Apply to editor background
    ui->textEditor->setStyleSheet(styleSheet);
    ui->buttonEditorBackColor->setStyleSheet(styleSheet);
    if (dialog.selectedColor() != Qt::white)
    {
        ui->buttonEditorBackColor->setIcon(QIcon(":/icons/background_fill_white.png"));
    }
    else
    {
        ui->buttonEditorBackColor->setIcon(QIcon(":/icons/background_fill_black.png"));
    }
    ui->buttonEditorBackColor->update();

}


void SummaryWindow::on_actionSentenceCase_triggered()
{
    QTextCursor cursor = ui->textEditor->textCursor();
    if(cursor.selectedText().isEmpty())
    {
        return;
    }
    QStringList tempList = cursor.selectedText().split(" ");
    QStringList newText;
    newText.append(tempList[0].at(0).toUpper() + tempList[0].mid(1).toLower());
    for (int i = 1; i < tempList.size(); i++)
    {
        newText.append(tempList[i].toLower());
    }

    cursor.removeSelectedText();
    cursor.insertText(newText.join(" "));
}


void SummaryWindow::on_actionUpperCase_triggered()
{
    QTextCursor cursor = ui->textEditor->textCursor();
    if(cursor.selectedText().isEmpty())
    {
        return;
    }
    QStringList tempList = cursor.selectedText().split(" ");
    QStringList newText;
    for (int i = 1; i < tempList.size(); i++)
    {
        newText.append(tempList[i].toUpper());
    }

    cursor.removeSelectedText();
    cursor.insertText(newText.join(" "));
}


void SummaryWindow::on_actionLowerCase_triggered()
{
    QTextCursor cursor = ui->textEditor->textCursor();
    if(cursor.selectedText().isEmpty())
    {
        return;
    }
    QStringList tempList = cursor.selectedText().split(" ");
    QStringList newText;
    for (int i = 1; i < tempList.size(); i++)
    {
        newText.append(tempList[i].toLower());
    }

    cursor.removeSelectedText();
    cursor.insertText(newText.join(" "));
}


void SummaryWindow::on_actionCapitalCase_triggered()
{
    QTextCursor cursor = ui->textEditor->textCursor();
    if(cursor.selectedText().isEmpty())
    {
        return;
    }
    QStringList tempList = cursor.selectedText().split(" ");
    QStringList newText;
    for (int i = 1; i < tempList.size(); i++)
    {
        if (tempList[i].size() >  1)
        {
            newText.append(tempList[i].at(0).toUpper() + tempList[i].mid(1).toLower());
        }
        else
        {
            newText.append(tempList[i].at(0).toLower());
        }
    }

    cursor.removeSelectedText();
    cursor.insertText(newText.join(" "));
}

