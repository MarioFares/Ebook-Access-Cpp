#include "ui_summarywindow.h"

#include "include/common.h"
#include "include/queries.h"
#include "include/summarywindow.h"
#include "include/insertlinkdialog.h"
#include "include/inserttabledialog.h"

#include "include/colorpickerwidget.h"
#include "include/findhighlighter.h"

#include <fstream>
#include <iostream>

#include <QFile>
#include <QTextList>
#include <QTextTable>
#include <QFileDialog>
#include <QColorDialog>
#include <QDesktopServices>
#include <QtPrintSupport/QPrinter>
#include <QDate>
#include <QLocale>
#include <QCalendar>
#include <QImageReader>

SummaryWindow::SummaryWindow(QWidget *parent):
    QMainWindow(parent), ui(new Ui::SummaryWindow)
{
    ui->setupUi(this);
    queries::connectToDatabase();
    ui->listWidget->setSortingEnabled(true);
    ui->textEditor->installEventFilter(this);
    ui->textFind->installEventFilter(this);
    ui->textEditor->setOpenExternalLinks(true);

    charPairs['('] = ')';
    charPairs['"'] = '"';
    charPairs['['] = ']';
    charPairs['{'] = '}';

    textHighlighter.setDocument(ui->textEditor->document());

    ui->frameFind->setHidden(true);
    ui->textEditor->setStyleSheet(common::openSheet(":/styles/textEditorStyle.qss"));
    searchEbooks("");
    setupConnections();
}

SummaryWindow::~SummaryWindow()
{
    if (!ui->labelTitle->text().isEmpty())
    {
        queries::updateSummary(ui->labelTitle->text(), ui->textEditor->toHtml());
    }
    delete ui;
}

void SummaryWindow::setupConnections()
{
    connect(ui->buttonBold, &QPushButton::clicked, this, &SummaryWindow::toggleBold);
    connect(ui->buttonItalic, &QPushButton::clicked, this, &SummaryWindow::toggleItalic);
    connect(ui->buttonUnderline, &QPushButton::clicked, this, &SummaryWindow::toggleUnderline);
    connect(ui->buttonThrough, &QPushButton::clicked, this, &SummaryWindow::toggleStrikethrough);
    connect(ui->buttonBullets, &QPushButton::clicked, this, &SummaryWindow::toggleBulletedList);
    connect(ui->buttonUndo, &QPushButton::clicked, ui->textEditor, &QTextBrowser::undo);
    connect(ui->buttonRedo, &QPushButton::clicked, ui->textEditor, &QTextBrowser::redo);
    connect(ui->buttonClearText, &QPushButton::clicked, ui->textEditor, &QTextBrowser::clear);
    connect(ui->buttonOrder, &QPushButton::clicked, this, &SummaryWindow::toggleOrderedList);
    connect(ui->buttonSubscript, &QPushButton::clicked, this, &SummaryWindow::setSubscript);
    connect(ui->buttonSuperscript, &QPushButton::clicked, this, &SummaryWindow::setSuperscript);
    connect(ui->buttonCode, &QPushButton::clicked, this, &SummaryWindow::insertCodeBlock);
    connect(ui->buttonHighlight, &QPushButton::clicked, this, &SummaryWindow::toggleHighlight);
    connect(ui->buttonEditorFontColor, &QPushButton::clicked, this, &SummaryWindow::setFontColor);
    connect(ui->buttonInsertLine, &QPushButton::clicked, this, &SummaryWindow::insertLine);
    connect(ui->buttonInsertImage, &QPushButton::clicked, this, &SummaryWindow::insertImage);
    connect(ui->buttonTable, &QPushButton::clicked, this, &SummaryWindow::insertTable);
    connect(ui->buttonLink, &QPushButton::clicked, this, &SummaryWindow::insertLink);
    connect(ui->buttonEditorBackColor, &QPushButton::clicked, this, &SummaryWindow::setBackColor);
    connect(ui->buttonNext, &QPushButton::clicked, this, &SummaryWindow::findNext);
    connect(ui->buttonPrevious, &QPushButton::clicked, this, &SummaryWindow::findPrevious);
    connect(ui->buttonIncreaseFontSize, &QPushButton::clicked, this, &SummaryWindow::increaseFontSize);
    connect(ui->buttonDecreaseFontSize, &QPushButton::clicked,  this, &SummaryWindow::decreaseFontSize);
    connect(ui->buttonIncreaseIndent, &QPushButton::clicked,  this, &SummaryWindow::increaseIndent);
    connect(ui->buttonDecreaseIndent, &QPushButton::clicked,  this, &SummaryWindow::decreaseIndent);
    connect(ui->buttonCloseFind, &QPushButton::clicked, [this]
    {
        toggleFindWidget(false);

    });
    connect(ui->fontComboBox, &QFontComboBox::currentTextChanged, [this]
    {
        setTextFontFamily(ui->fontComboBox->currentText(), true);
    });
    connect(ui->spinBoxFontSize, &QSpinBox::valueChanged, [this]
    {
        setTextFontSize(ui->spinBoxFontSize->value(), true);
    });

    connect(ui->textFind, &QLineEdit::textChanged, this, &SummaryWindow::findText);
    connect(ui->comboBoxAlignment, &QComboBox::currentIndexChanged, this, &SummaryWindow::changeTextAlignment);
    connect(ui->listWidget, &QListWidget::itemClicked, this, &SummaryWindow::openEbookSummary);
    connect(ui->textSearch, &QLineEdit::textChanged, this, &SummaryWindow::searchText);
    connect(ui->textSearch, &QLineEdit::returnPressed, this, &SummaryWindow::searchText);
    connect(ui->textEditor, &QTextBrowser::currentCharFormatChanged, this, &SummaryWindow::textFormatChanged);
    connect(ui->textEditor, &QTextBrowser::selectionChanged, this, &SummaryWindow::textFormatChanged);

    connect(ui->actionClose, &QAction::triggered, this, &SummaryWindow::close);
    connect(ui->actionMaximize, &QAction::triggered, [this] { common::toggleMaximized(this); });
    connect(ui->actionFullscreen, &QAction::triggered, [this] { common::toggleFullscreen(this); });
    connect(ui->actionMinimize, &QAction::triggered, this, &SummaryWindow::showMinimized);
    connect(ui->actionHideSearchBar, &QAction::triggered, this, &SummaryWindow::hideSearchBar);
    connect(ui->actionHideLeftPane, &QAction::triggered, this, &SummaryWindow::hideLeftPane);
    connect(ui->actionHideTopToolbar, &QAction::triggered, this, &SummaryWindow::hideTopToolbar);
    connect(ui->actionHideRightToolbar, &QAction::triggered, this, &SummaryWindow::hideRightToolbar);
    connect(ui->actionHideRightPane, &QAction::triggered, this, &SummaryWindow::hideRightPane);
    connect(ui->actionResetFormat, &QAction::triggered, this, &SummaryWindow::resetFormat);
    connect(ui->actionPaste, &QAction::triggered, ui->textEditor, &QTextBrowser::paste);
    connect(ui->actionSearchText, &QAction::triggered, this, &SummaryWindow::searchText);
    connect(ui->actionClearSearch, &QAction::triggered, this, &SummaryWindow::clearSearch);
    connect(ui->actionPrint, &QAction::triggered, this, &SummaryWindow::printSummaryToPdf);
    connect(ui->actionCopyFormatting, &QAction::triggered, this, &SummaryWindow::copyFormatting);
    connect(ui->actionPasteMatchFormat, &QAction::triggered, this, &SummaryWindow::pasteMatchFormat);
    connect(ui->actionSaveSummary, &QAction::triggered, this, &SummaryWindow::saveSummary);
    connect(ui->actionExportHtml, &QAction::triggered, this, &SummaryWindow::exportSummaryToHtml);
    connect(ui->actionSentenceCase, &QAction::triggered, this, &SummaryWindow::sentenceCase);
    connect(ui->actionUpperCase, &QAction::triggered, this, &SummaryWindow::upperCase);
    connect(ui->actionLowerCase, &QAction::triggered, this, &SummaryWindow::lowerCase);
    connect(ui->actionCapitalCase, &QAction::triggered, this, &SummaryWindow::capitalCase);
    connect(ui->actionSelectCurrentLine, &QAction::triggered, this, &SummaryWindow::selectCurrentLine);
    connect(ui->actionDuplicateCurrentLine, &QAction::triggered, this, &SummaryWindow::duplicateCurrentLine);
    connect(ui->actionDeleteCurrentLine, &QAction::triggered, this, &SummaryWindow::deleteCurrentLine);
    connect(ui->actionZoomIn, &QAction::triggered, this, &SummaryWindow::zoomIn);
    connect(ui->actionZoomOut, &QAction::triggered, this, &SummaryWindow::zoomOut);
    connect(ui->actionDateNormal, &QAction::triggered, this, &SummaryWindow::insertNormalDate);
    connect(ui->actionDateShort, &QAction::triggered, this, &SummaryWindow::insertShortDate);
    connect(ui->actionTableRow, &QAction::triggered, this, &SummaryWindow::addTableRow);
    connect(ui->actionTableColumn, &QAction::triggered, this, &SummaryWindow::addTableColumn);
    connect(ui->actionFind, &QAction::triggered, [this]
    {
        toggleFindWidget(!ui->frameFind->isVisible());
    });
    connect(ui->actionBold, &QAction::triggered, this, &SummaryWindow::toggleBold);
    connect(ui->actionItalic, &QAction::triggered, this, &SummaryWindow::toggleItalic);
    connect(ui->actionUnderline, &QAction::triggered, this, &SummaryWindow::toggleUnderline);
    connect(ui->actionThrough, &QAction::triggered, this, &SummaryWindow::toggleStrikethrough);
    connect(ui->actionBulletedList, &QAction::triggered, this, &SummaryWindow::toggleBulletedList);
    connect(ui->actionOrderedList, &QAction::triggered, this, &SummaryWindow::toggleOrderedList);
    connect(ui->actionSubscript, &QAction::triggered, this, &SummaryWindow::setSubscript);
    connect(ui->actionSuperscript, &QAction::triggered, this, &SummaryWindow::setSuperscript);
    connect(ui->actionCodeBlock, &QAction::triggered, this, &SummaryWindow::insertCodeBlock);
    connect(ui->actionHighlight, &QAction::triggered, this, &SummaryWindow::toggleHighlight);
    connect(ui->actionIncFontSize, &QAction::triggered, this, &SummaryWindow::increaseFontSize);
    connect(ui->actionDecFontSize, &QAction::triggered, this, &SummaryWindow::decreaseFontSize);
    connect(ui->actionIncIndent, &QAction::triggered, this, &SummaryWindow::increaseIndent);
    connect(ui->actionDecIndent, &QAction::triggered, this, &SummaryWindow::decreaseIndent);
    connect(ui->actionUndo, &QAction::triggered, ui->textEditor, &QTextBrowser::undo);
    connect(ui->actionRedo, &QAction::triggered, ui->textEditor, &QTextBrowser::redo);
    connect(ui->actionClearAll, &QAction::triggered, ui->textEditor, &QTextBrowser::clear);
    connect(ui->actionCut, &QAction::triggered, ui->textEditor, &QTextBrowser::cut);
    connect(ui->actionCopy, &QAction::triggered, ui->textEditor, &QTextBrowser::copy);
    connect(ui->actionSelectAll, &QAction::triggered, ui->textEditor, &QTextBrowser::selectAll);
    connect(ui->actionInsertLine, &QAction::triggered, this, &SummaryWindow::insertLine);
    connect(ui->actionInsertLink, &QAction::triggered, this, &SummaryWindow::insertLink);
    connect(ui->actionInsertImage, &QAction::triggered, this, &SummaryWindow::insertImage);
    connect(ui->actionInsertTable, &QAction::triggered, this, &SummaryWindow::insertTable);
}

void SummaryWindow::closeEvent(QCloseEvent *event)
{
    if (!ui->labelTitle->text().isEmpty())
    {
        queries::updateSummary(ui->labelTitle->text(), ui->textEditor->toHtml());
    }
    event->accept();
}

bool SummaryWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*> (event);
        if (obj == ui->textEditor)
        {
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
        else if (obj == ui->textFind)
        {
            bool shiftModKeyPressed = keyEvent->modifiers() & Qt::ShiftModifier;
            switch (keyEvent->key())
            {
                case Qt::Key_Escape:
                    toggleFindWidget(false);
                    break;
                case Qt::Key_Enter:
                case Qt::Key_Return:
                    shiftModKeyPressed ? findPrevious() : findNext();
                    break;
                default:
                    break;
            }
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

void SummaryWindow::insertClosingChar(const char &openChar, const QString &selectedText)
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

    if (cursor.currentList() != nullptr && currentBlock.text().isEmpty())
    {
        if (changeListIndentation(-1) == 0)
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

    if (cursor.currentList() != nullptr && currentBlock.text().isEmpty())
    {
        if (changeListIndentation(-1) == 0)
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

void SummaryWindow::selectEbookSummary(const QString &name)
{
    if (!name.isEmpty())
    {
        QList listItems = ui->listWidget->findItems(name, Qt::MatchExactly);
        openEbookSummary(listItems[0]);
        ui->listWidget->setCurrentItem(listItems[0]);
    }
}

void SummaryWindow::searchEbooks(const QString &textToSearch)
{
    ui->listWidget->clear();
    queries::selectNameBasedOnString(textToSearch);
    while (queries::query.next())
    {
        ui->listWidget->addItem(queries::query.value(0).toString());
    }
}

void SummaryWindow::createList(QTextListFormat::Style listStyle)
{
    QTextCursor cursor = ui->textEditor->textCursor();
    cursor.setKeepPositionOnInsert(true);
    QTextList *currentList = cursor.currentList();
    if (currentList != nullptr)
    {
        QTextListFormat listFormat;
        listFormat.setIndent(0);
        listFormat.setStyle(listStyle);
        currentList->setFormat(listFormat);
        for (int i = currentList->count() - 1; i >= 0; --i)
        {
            currentList->removeItem(i);
        }
    }
    else
    {
        QTextListFormat::Style style = listStyle;
        QTextListFormat listFormat;
        listFormat.setStyle(style);
        cursor.createList(listFormat);
    }
}

int SummaryWindow::changeListIndentation(const int &increment)
{
    QTextCursor cursor = ui->textEditor->textCursor();
    QTextList *currentList = cursor.currentList();

    if (currentList != nullptr)
    {
        QTextListFormat listFormat;
        QTextListFormat::Style currentStyle = currentList->format().style();
        listFormat.setIndent(cursor.currentList()->format().indent() + increment);
        if (currentStyle == QTextListFormat::ListDisc || currentStyle == QTextListFormat::ListCircle || currentStyle == QTextListFormat::ListSquare)
        {
            if (listFormat.indent() == 1 || listFormat.indent() % 3 == 1)
            {
                listFormat.setStyle(QTextListFormat::ListDisc);
            }

            if (listFormat.indent() == 2 || listFormat.indent() % 3 == 2)
            {
                listFormat.setStyle(QTextListFormat::ListCircle);
            }

            if (listFormat.indent() % 3 == 0)
            {
                listFormat.setStyle(QTextListFormat::ListSquare);
            }
        }

        if (currentStyle == QTextListFormat::ListDecimal || currentStyle == QTextListFormat::ListLowerAlpha || currentStyle == QTextListFormat::ListLowerRoman)
        {
            if (listFormat.indent() == 1 || listFormat.indent() % 3 == 1)
            {
                listFormat.setStyle(QTextListFormat::ListDecimal);
            }

            if (listFormat.indent() == 2 || listFormat.indent() % 3 == 2)
            {
                listFormat.setStyle(QTextListFormat::ListLowerAlpha);
            }

            if (listFormat.indent() % 3 == 0)
            {
                listFormat.setStyle(QTextListFormat::ListLowerRoman);
            }
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

void SummaryWindow::toggleBold()
{
    QFont currentFont = ui->textEditor->currentFont();
    currentFont.setBold(!currentFont.bold());
    ui->textEditor->setCurrentFont(currentFont);
    textFormatChanged();
}

void SummaryWindow::toggleItalic()
{
    QFont currentFont = ui->textEditor->currentFont();
    currentFont.setItalic(!currentFont.italic());
    ui->textEditor->setCurrentFont(currentFont);
    textFormatChanged();
}

void SummaryWindow::toggleUnderline()
{
    QFont currentFont = ui->textEditor->currentFont();
    currentFont.setUnderline(!currentFont.underline());
    ui->textEditor->setCurrentFont(currentFont);
    textFormatChanged();
}

void SummaryWindow::toggleStrikethrough()
{
    QFont currentFont = ui->textEditor->currentFont();
    currentFont.setStrikeOut(!currentFont.strikeOut());
    ui->textEditor->setCurrentFont(currentFont);
    textFormatChanged();
}

void SummaryWindow::setTextFontFamily(const QString &arg1, bool change)
{
    if (change)
    {
        ui->textEditor->setCurrentFont(arg1);
    }
    else
    {
        ui->fontComboBox->blockSignals(true);
        ui->fontComboBox->setCurrentFont(arg1);
        ui->fontComboBox->blockSignals(false);
    }
}

void SummaryWindow::setTextFontSize(int arg1, bool change)
{
    if (change)
    {
        QFont currentFont = ui->textEditor->currentFont();
        currentFont.setPointSize(arg1);
        ui->textEditor->setCurrentFont(currentFont);
    }
    else
    {
        ui->spinBoxFontSize->blockSignals(true);
        ui->spinBoxFontSize->setValue(arg1);
        ui->spinBoxFontSize->blockSignals(false);
    }
}

void SummaryWindow::toggleBulletedList()
{
    createList(QTextListFormat::ListDisc);
    textFormatChanged();
}

void SummaryWindow::toggleOrderedList()
{
    createList(QTextListFormat::ListDecimal);
    textFormatChanged();
}

void SummaryWindow::changeTextAlignment(int index)
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

void SummaryWindow::setSubscript()
{
    QTextCharFormat currentFormat = ui->textEditor->currentCharFormat();
    currentFormat.setVerticalAlignment(QTextCharFormat::AlignSubScript);
    ui->textEditor->setCurrentCharFormat(currentFormat);
}

void SummaryWindow::setSuperscript()
{
    QTextCharFormat currentFormat = ui->textEditor->currentCharFormat();
    currentFormat.setVerticalAlignment(QTextCharFormat::AlignSuperScript);
    ui->textEditor->setCurrentCharFormat(currentFormat);
}

void SummaryWindow::insertCodeBlock()
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

void SummaryWindow::textFormatChanged()
{
    setTextFontFamily(ui->textEditor->currentFont().family(), false);
    setTextFontSize(ui->textEditor->currentFont().pointSize(), false);
    ui->buttonEditorFontColor->setStyleSheet("background-color: " + ui->textEditor->textColor().name());

    ui->buttonBold->setStyleSheet("background-color: " + QString(ui->textEditor->currentFont().bold() ? "#1C3387" : "#2D2D30"));
    ui->buttonItalic->setStyleSheet("background-color: " + QString(ui->textEditor->currentFont().italic() ? "#1C3387" : "#2D2D30"));
    ui->buttonUnderline->setStyleSheet("background-color: " + QString(ui->textEditor->currentFont().underline() ? "#1C3387" : "#2D2D30"));
    ui->buttonThrough->setStyleSheet("background-color: " + QString(ui->textEditor->currentFont().strikeOut() ? "#1C3387" : "#2D2D30"));
    ui->buttonHighlight->setStyleSheet("background-color: " + QString(ui->textEditor->currentCharFormat().background() == Qt::yellow ? "#1C3387" : "#2D2D30"));
}

void SummaryWindow::toggleHighlight()
{
    QTextCharFormat currentFormat = ui->textEditor->currentCharFormat();
    currentFormat.setBackground(currentFormat.background() == Qt::yellow ? Qt::white : Qt::yellow);
    ui->textEditor->setCurrentCharFormat(currentFormat);
    textFormatChanged();
}

void SummaryWindow::setFontColor()
{
    QPoint bottom = ui->buttonEditorFontColor->rect().bottomLeft();
    QPoint globalPos = ui->buttonEditorFontColor->mapToGlobal(bottom);
    globalPos.setY(globalPos.y() + 2);

    colorPickerWidget *widget = new colorPickerWidget(this, Qt::black);
    widget->move(globalPos);
    common::openDialog(widget, ":/styles/colorpickerstyle.qss");

    QString colorSheet = "background-color: " + widget->getCurrentColor().name();
    ui->buttonEditorFontColor->setStyleSheet(colorSheet);
    ui->buttonEditorFontColor->update();

    ui->textEditor->setTextColor(widget->getCurrentColor());
}

void SummaryWindow::hideSearchBar()
{
    common::changeWidgetVisibility(ui->textSearch, ui->actionHideSearchBar);
}

void SummaryWindow::hideLeftPane()
{
    ui->frameListWidget->setHidden(!ui->frameListWidget->isHidden());
    ui->mainSplitterPane->refresh();
    ui->actionHideLeftPane->setText(ui->frameListWidget->isHidden() ? "Show Left Pane" : "Hide Left Pane");
}

void SummaryWindow::hideTopToolbar()
{
    common::changeWidgetVisibility(ui->frameEditorToolBox, ui->actionHideTopToolbar);
}

void SummaryWindow::hideRightToolbar()
{
    common::changeWidgetVisibility(ui->frameRightToolBar, ui->actionHideRightToolbar);
}

void SummaryWindow::hideRightPane()
{
    ui->frameEditor->setHidden(!ui->frameEditor->isHidden());
    ui->mainSplitterPane->refresh();
    hideRightToolbar();
    ui->actionHideRightPane->setText(ui->frameEditor->isHidden() ? "Show Right Pane" : "Hide Right Pane");

}

void SummaryWindow::insertLine()
{
    QTextCursor cursor = ui->textEditor->textCursor();
    cursor.insertHtml(QString("<hr><br>"));
    cursor.movePosition(QTextCursor::Up, QTextCursor::MoveAnchor, 1);
    ui->textEditor->setTextCursor(cursor);
}

void SummaryWindow::resetFormat()
{
    QFont currentFont = ui->textEditor->currentFont();
    currentFont.setBold(false);
    currentFont.setItalic(false);
    currentFont.setStrikeOut(false);
    currentFont.setUnderline(false);

    QTextCharFormat currentFormat = ui->textEditor->currentCharFormat();
    currentFormat.setBackground(ui->textEditor->palette().base().color());
    currentFormat.setVerticalAlignment(QTextCharFormat::AlignNormal);

    ui->textEditor->setCurrentCharFormat(currentFormat);
    ui->textEditor->setCurrentFont(currentFont);

    textFormatChanged();
}

void SummaryWindow::insertImage()
{
    QString file = QFileDialog::getOpenFileName(this,
        tr("Select Image"), "/", tr("Image Files (*.jpeg *.jpg *.png *.gif *.apng *.svg *.bmp *.ico)"));

    QUrl Uri(QString("%1").arg(file));
    QImage image = QImageReader(file).read();

    QTextDocument *textDocument = ui->textEditor->document();
    textDocument->addResource(QTextDocument::ImageResource, Uri, QVariant(image));
    QTextCursor cursor = ui->textEditor->textCursor();
    QTextImageFormat imageFormat;
    imageFormat.setWidth(image.width());
    imageFormat.setHeight(image.height());
    imageFormat.setName(Uri.toString());
    cursor.insertImage(imageFormat);
}

void SummaryWindow::searchText()
{
    if (ui->listWidget->selectedItems().count() != 0)
    {
        queries::updateSummary(ui->labelTitle->text(), ui->textEditor->toHtml());
    }

    QString stringToSearch = ui->textSearch->text();
    searchEbooks(stringToSearch);
}

void SummaryWindow::clearSearch()
{
    ui->textSearch->clear();
    ui->listWidget->clear();
}

void SummaryWindow::openEbookSummary(QListWidgetItem *item)
{
    if (!ui->labelTitle->text().isEmpty())
    {
        queries::updateSummary(ui->labelTitle->text(), ui->textEditor->toHtml());
    }

    ui->textEditor->clear();
    ui->labelTitle->setText(item->text());
    queries::selectSummaryBasedonName(item->text());
    queries::query.next();
    QString summary = queries::query.value(0).toString();
    if (summary != "N/A")
    {
        ui->textEditor->insertHtml(summary);
    }
}

void SummaryWindow::printSummaryToPdf()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save File"), "/", tr("*.pdf"));

    QPrinter printer(QPrinter::HighResolution);
    printer.setPrintRange(QPrinter::CurrentPage);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);
    ui->textEditor->print(&printer);
    QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
}

void SummaryWindow::insertTable()
{
    insertTableDialog dialog(this);
    common::openDialog(&dialog, ":/styles/style.qss");

    if (dialog.rowCount != 0 && dialog.columnCount != 0)
    {
        QTextTableFormat tableFormat;
        tableFormat.setBorder(1);
        tableFormat.setCellSpacing(5);
        tableFormat.setCellPadding(1);
        tableFormat.setBorderCollapse(true);
        tableFormat.setBorderStyle(QTextTableFormat::BorderStyle_Solid);
        ui->textEditor->textCursor().insertTable(dialog.rowCount, dialog.columnCount, tableFormat);
    }
}

void SummaryWindow::insertLink()
{
    insertLinkDialog dialog(this);
    common::openDialog(&dialog, ":/styles/style.qss");

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

void SummaryWindow::copyFormatting()
{
    currentCopiedFormat = ui->textEditor->currentCharFormat();
    currentCopiedFont = ui->textEditor->currentFont();
}

void SummaryWindow::pasteMatchFormat()
{
    ui->textEditor->setCurrentCharFormat(currentCopiedFormat);
    ui->textEditor->setCurrentFont(currentCopiedFont);
}

void SummaryWindow::saveSummary()
{
    if (!ui->labelTitle->text().isEmpty())
    {
        queries::updateSummary(ui->labelTitle->text(), ui->textEditor->toHtml());
    }
}

void SummaryWindow::exportSummaryToHtml()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save File"), "/", tr("*.html"));
    std::ofstream htmlFile(filePath.toStdString());
    htmlFile << ui->textEditor->toHtml().toStdString();
    htmlFile.close();
    QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
}

void SummaryWindow::setBackColor()
{
    QPoint bottom = ui->buttonEditorBackColor->rect().bottomLeft();
    QPoint globalPos = ui->buttonEditorBackColor->mapToGlobal(bottom);
    globalPos.setY(globalPos.y() + 2);

    colorPickerWidget *widget = new colorPickerWidget(this, Qt::white);
    widget->move(globalPos);
    common::openDialog(widget, ":/styles/colorpickerstyle.qss");

    QString styleSheet = "background-color: " + widget->getCurrentColor().name();

    QTextFrame *currentFrame = ui->textEditor->textCursor().currentFrame();
    QTextFrameFormat currentFormat = currentFrame->frameFormat();
    // Apply only to code box in case cursor in code box
    if (currentFormat.border() == 1)
    {
        currentFormat.setBackground(QBrush(widget->getCurrentColor()));
        currentFrame->setFrameFormat(currentFormat);
        return;
    }

    // Apply to editor background
    ui->textEditor->setStyleSheet(styleSheet);
    ui->buttonEditorBackColor->setStyleSheet(styleSheet);
    if (widget->getCurrentColor() != Qt::white)
    {
        ui->buttonEditorBackColor->setIcon(QIcon(":/icons/background_fill_white.png"));
    }
    else
    {
        ui->buttonEditorBackColor->setIcon(QIcon(":/icons/background_fill_black.png"));
    }

    ui->buttonEditorBackColor->update();
}

void SummaryWindow::sentenceCase()
{
    QTextCursor cursor = ui->textEditor->textCursor();
    if (cursor.selectedText().isEmpty())
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

void SummaryWindow::upperCase()
{
    QTextCursor cursor = ui->textEditor->textCursor();
    if (cursor.selectedText().isEmpty())
    {
        return;
    }

    QStringList tempList = cursor.selectedText().split(" ");
    QStringList newText;
    for (const QString &word : tempList)
    {
        newText.append(word.toUpper());
    }

    cursor.removeSelectedText();
    cursor.insertText(newText.join(" "));
}

void SummaryWindow::lowerCase()
{
    QTextCursor cursor = ui->textEditor->textCursor();
    if (cursor.selectedText().isEmpty())
    {
        return;
    }

    QStringList tempList = cursor.selectedText().split(" ");
    QStringList newText;
    for (const QString &word : tempList)
    {
        newText.append(word.toLower());
    }

    cursor.removeSelectedText();
    cursor.insertText(newText.join(" "));
}

void SummaryWindow::capitalCase()
{
    QTextCursor cursor = ui->textEditor->textCursor();
    if (cursor.selectedText().isEmpty())
    {
        return;
    }

    QStringList tempList = cursor.selectedText().split(" ");
    QStringList newText;    
    for (const QString &word : tempList)
    {
        if (word.size() > 1)
        {
            newText.append(word.at(0).toUpper() + word.mid(1).toLower());
        }
        else
        {
            newText.append(word.at(0).toLower());
        }
    }
    cursor.removeSelectedText();
    cursor.insertText(newText.join(" "));
}

void SummaryWindow::selectCurrentLine()
{
    QTextCursor cursor = ui->textEditor->textCursor();
    cursor.movePosition(QTextCursor::StartOfLine);
    cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
    ui->textEditor->setTextCursor(cursor);
}

void SummaryWindow::duplicateCurrentLine()
{
    QTextCursor cursor = ui->textEditor->textCursor();
    cursor.movePosition(QTextCursor::StartOfLine);
    cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
    QString currentLine = cursor.selectedText();
    cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::MoveAnchor);
    cursor.insertText("\n");
    cursor.insertText(currentLine);
}

void SummaryWindow::deleteCurrentLine()
{
    QTextCursor cursor = ui->textEditor->textCursor();
    cursor.movePosition(QTextCursor::StartOfLine);
    cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
    cursor.deletePreviousChar();
}

void SummaryWindow::zoomIn()
{
    ui->textEditor->zoomIn(5);
}

void SummaryWindow::zoomOut()
{
    ui->textEditor->zoomOut(5);
}

void SummaryWindow::insertNormalDate()
{
    QDate currDate = QDate::currentDate();
    QLocale locale;
    QCalendar cal;
    QString dayName = locale.dayName(currDate.dayOfWeek(), QLocale::LongFormat);
    quint32 dayNum = currDate.day();
    quint32 yearNum = currDate.year();
    QString monthName = cal.monthName(locale, currDate.month(), yearNum);
    QString dateStr = dayName + ", " + monthName + " " + QString::number(dayNum) + ", " + QString::number(yearNum);
    ui->textEditor->textCursor().insertText(dateStr);
}

void SummaryWindow::insertShortDate()
{
    QDate currDate = QDate::currentDate();
    QString day = QString::number(currDate.day());
    QString month = QString::number(currDate.month());
    QString year = QString::number(currDate.year());
    QString dateStr = day + "/" + month + "/" + year;
    ui->textEditor->textCursor().insertText(dateStr);
}

void SummaryWindow::addTableRow()
{
    QTextCursor cursor = ui->textEditor->textCursor();
    QTextTable *currTable = cursor.currentTable();
    if (currTable != nullptr)
    {
        currTable->insertRows(currTable->cellAt(cursor).row(), 1);
    }
}

void SummaryWindow::addTableColumn()
{
    QTextCursor cursor = ui->textEditor->textCursor();
    QTextTable *currTable = cursor.currentTable();
    if (currTable != nullptr)
    {
        currTable->insertColumns(currTable->cellAt(cursor).column(), 1);
    }
}

// Find functionality specific
void SummaryWindow::toggleFindWidget(bool visible)
{
    ui->frameFind->setVisible(visible);
    highlightText();

    if (visible)
    {
        ui->textFind->setFocus();
    }
}

void SummaryWindow::highlightText()
{
    if (ui->frameFind->isVisible())
    {
        textHighlighter.setWordPattern(ui->textFind->text());
    }
    else
    {
        textHighlighter.setWordPattern("");
    }

    if (sender() == ui->textEditor)
    {
        ui->textEditor->blockSignals(true);
        textHighlighter.customRehighlight();
        ui->textEditor->blockSignals(false);
    }
    else
    {
        textHighlighter.customRehighlight();
    }
}

void SummaryWindow::textEditRefreshHighlighter(int cursorIndex)
{
    if (cursorIndex >= 0)
    {
        QTextCursor currentCursor = ui->textEditor->textCursor();
        currentCursor.setPosition(cursorIndex);
        ui->textEditor->setTextCursor(currentCursor);
        ui->textEditor->ensureCursorVisible();
    }
}

void SummaryWindow::findText(const QString &arg1)
{
    highlightText();
    if (arg1.isEmpty())
    {
        ui->labelFindMatchNum->setText("");
        return;
    }
    QString labelText = QString("Found %1 matches").arg(QString::number(textHighlighter.matchNumber()));
    ui->labelFindMatchNum->setText(labelText);
    QFont customFont;
    customFont.setItalic(true);
    customFont.setPointSize(12);
    customFont.setFamily("Segoe UI");
    ui->labelFindMatchNum->setFont(customFont);
    ui->labelFindMatchNum->setStyleSheet("color: grey");
}

void SummaryWindow::findNext()
{
    int cursorIndex = textHighlighter.setNextMatchStateActive();
    if (textHighlighter.matchIndex() == textHighlighter.matchNumber() - 1)
    {
        textHighlighter.setMatchIndex(-1);
    }
    textEditRefreshHighlighter(cursorIndex);
}

void SummaryWindow::findPrevious()
{
    int cursorIndex = textHighlighter.setPrevMatchStateActive();
    if (textHighlighter.matchIndex() == 0)
    {
        textHighlighter.setMatchIndex(textHighlighter.matchNumber() - 1);
    }
    textEditRefreshHighlighter(cursorIndex);
}

void SummaryWindow::increaseFontSize()
{
    ui->spinBoxFontSize->setValue(ui->spinBoxFontSize->value() + 1);
}

void SummaryWindow::decreaseFontSize()
{
    ui->spinBoxFontSize->setValue(ui->spinBoxFontSize->value() - 1);
}

void SummaryWindow::increaseIndent()
{
    changeListIndentation(1);
}

void SummaryWindow::decreaseIndent()
{
    changeListIndentation(-1);
}
