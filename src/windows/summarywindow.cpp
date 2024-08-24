#include "include/common.h"
#include "include/queries.h"
#include "include/widgets/texteditor.h"
#include "include/windows/summarywindow.h"
#include "include/dialogs/insertlinkdialog.h"
#include "include/widgets/booksearchwidget.h"
#include "include/dialogs/inserttabledialog.h"
#include "include/widgets/colorpickerwidget.h"

#include "fstream"

#include <QDate>
#include <QLocale>
#include <QCalendar>
#include <QTextList>
#include <QTextTable>
#include <QBoxLayout>
#include <QSpacerItem>
#include <QFileDialog>
#include <QColorDialog>
#include <QImageReader>
#include <QDesktopServices>
#include <QtPrintSupport/QPrinter>

SummaryWindow::SummaryWindow(QWidget *parent) : QMainWindow(parent) {
    queries::connectToDatabase();

    setupInterface();
    setupConnections();
    setupTabOrder();
    setupShortcuts();
}

void SummaryWindow::setupInterface() {
    // Window
    setWindowTitle("Summaries");
    resize(1035, 600);

    // Widgets
    _centralWidget = new QWidget(this);
    _horLayMain = new QHBoxLayout(_centralWidget);

    _mainSplitterPane = new QSplitter(_centralWidget);
    _mainSplitterPane->setStyleSheet(QString("QSplitter {border: 0px;}"));
    _mainSplitterPane->setLineWidth(0);
    _mainSplitterPane->setOrientation(Qt::Horizontal);
    _mainSplitterPane->setHandleWidth(0);

    _bookSearchWidget = new BookSearchWidget(this);
    _bookSearchWidget->setMainLayoutMargin(-1, 41, -1, -1);
    _bookSearchWidget->setMainLayoutSpacing(15);
    _mainSplitterPane->addWidget(_bookSearchWidget);

    _frameEditor = new QFrame(_mainSplitterPane);
    _frameEditor->setStyleSheet(QString("QFrame { border: 0px; }"));

    _vertLayEditor = new QVBoxLayout(_frameEditor);
    _vertLayEditor->setContentsMargins(0, -1, 0, -1);

    _labelTitle = new QLabel(_frameEditor);
    _labelTitle->setMinimumSize(QSize(0, 19));

    _frameEditorToolBox = new QFrame(_frameEditor);
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(_frameEditorToolBox->sizePolicy().hasHeightForWidth());

    _frameEditorToolBox->setSizePolicy(sizePolicy);
    _frameEditorToolBox->setFrameShape(QFrame::StyledPanel);
    _frameEditorToolBox->setFrameShadow(QFrame::Raised);

    _horLayToolBox = new QHBoxLayout(_frameEditorToolBox);
    _horLayToolBox->setContentsMargins(0, 7, 0, -1);

    _fontComboBox = new QFontComboBox(_frameEditorToolBox);
    _fontComboBox->setMinimumSize(QSize(0, 25));
    _fontComboBox->setStyleSheet(QString("QFontComboBox{ border: 1px solid grey; }"));
    _fontComboBox->setEditable(false);

    _spinBoxFontSize = new QSpinBox(_frameEditorToolBox);
    _spinBoxFontSize->setMinimumSize(QSize(0, 25));
    _spinBoxFontSize->setMaximumSize(QSize(40, 999999));
    _spinBoxFontSize->setMinimum(1);
    _spinBoxFontSize->setMaximum(70);
    _spinBoxFontSize->setValue(12);

    _buttonEditorFontColor = new QToolButton(_frameEditorToolBox);
    _buttonEditorFontColor->setMinimumSize(QSize(30, 25));
    _buttonEditorFontColor->setMaximumSize(QSize(30, 16777215));
    _buttonEditorFontColor->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonEditorFontColor->setFocusPolicy(Qt::NoFocus);
    _buttonEditorFontColor->setStyleSheet(QString::fromUtf8(""));
    _buttonEditorFontColor->setIcon(QIcon(":/icons/font_color_icon.png"));

    _buttonEditorBackColor = new QToolButton(_frameEditorToolBox);
    _buttonEditorBackColor->setMinimumSize(QSize(30, 25));
    _buttonEditorBackColor->setMaximumSize(QSize(30, 16777215));
    _buttonEditorBackColor->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonEditorBackColor->setIcon(QIcon(":/icons/background_fill_white.png"));

    _lineVert1 = new QFrame(_frameEditorToolBox);
    _lineVert1->setStyleSheet(QString("color: grey; background-color: grey;"));
    _lineVert1->setFrameShape(QFrame::VLine);
    _lineVert1->setFrameShadow(QFrame::Sunken);

    _buttonBold = new QToolButton(_frameEditorToolBox);
    _buttonBold->setMinimumSize(QSize(30, 25));
    _buttonBold->setMaximumSize(QSize(30, 16777215));
    _buttonBold->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonBold->setFocusPolicy(Qt::NoFocus);
    _buttonBold->setIcon(QIcon(":/icons/bold_icon.png"));

    _buttonItalic = new QToolButton(_frameEditorToolBox);
    _buttonItalic->setMinimumSize(QSize(30, 25));
    _buttonItalic->setMaximumSize(QSize(30, 16777215));
    _buttonItalic->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonItalic->setFocusPolicy(Qt::NoFocus);
    _buttonItalic->setIcon(QIcon(":/icons/italic_icon.png"));

    _buttonUnderline = new QToolButton(_frameEditorToolBox);
    _buttonUnderline->setMinimumSize(QSize(30, 25));
    _buttonUnderline->setMaximumSize(QSize(30, 16777215));
    _buttonUnderline->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonUnderline->setFocusPolicy(Qt::NoFocus);
    _buttonUnderline->setIcon(QIcon(":/icons/underline_icon.png"));

    _buttonThrough = new QToolButton(_frameEditorToolBox);
    _buttonThrough->setMinimumSize(QSize(30, 25));
    _buttonThrough->setMaximumSize(QSize(30, 16777215));
    _buttonThrough->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonThrough->setFocusPolicy(Qt::NoFocus);
    _buttonThrough->setIcon(QIcon(":/icons/strikethrough_icon.png"));

    _buttonHighlight = new QToolButton(_frameEditorToolBox);
    _buttonHighlight->setMinimumSize(QSize(30, 25));
    _buttonHighlight->setMaximumSize(QSize(30, 16777215));
    _buttonHighlight->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonHighlight->setFocusPolicy(Qt::NoFocus);
    _buttonHighlight->setIcon(QIcon(":/icons/highlight_icon.png"));

    _buttonCodeBlock = new QToolButton(_frameEditorToolBox);
    _buttonCodeBlock->setMinimumSize(QSize(30, 25));
    _buttonCodeBlock->setMaximumSize(QSize(30, 16777215));
    _buttonCodeBlock->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonCodeBlock->setFocusPolicy(Qt::NoFocus);
    _buttonCodeBlock->setIcon(QIcon(":/icons/code_icon.png"));

    _lineVert2 = new QFrame(_frameEditorToolBox);
    _lineVert2->setStyleSheet(QString::fromUtf8("color: grey; background-color: grey;"));
    _lineVert2->setFrameShadow(QFrame::Plain);
    _lineVert2->setLineWidth(0);
    _lineVert2->setMidLineWidth(0);
    _lineVert2->setFrameShape(QFrame::VLine);

    _buttonBulletedList = new QToolButton(_frameEditorToolBox);
    _buttonBulletedList->setMinimumSize(QSize(30, 25));
    _buttonBulletedList->setMaximumSize(QSize(30, 16777215));
    _buttonBulletedList->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonBulletedList->setFocusPolicy(Qt::NoFocus);
    _buttonBulletedList->setIcon(QIcon(":/icons/bullets_icon.png"));

    _buttonOrderedList = new QToolButton(_frameEditorToolBox);
    _buttonOrderedList->setMinimumSize(QSize(30, 25));
    _buttonOrderedList->setMaximumSize(QSize(30, 16777215));
    _buttonOrderedList->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonOrderedList->setFocusPolicy(Qt::NoFocus);
    _buttonOrderedList->setIcon(QIcon(":/icons/numbers_icon.png"));

    _comboBoxAlignment = new QComboBox(_frameEditorToolBox);
    QIcon iconAlignLeft(":/icons/alignleft_icon.png");
    _comboBoxAlignment->addItem(iconAlignLeft, QString("Left"));
    QIcon iconAlignCenter(":/icons/aligncenter_icon.png");
    _comboBoxAlignment->addItem(iconAlignCenter, QString("Center"));
    QIcon iconAlignRight(":/icons/alignright_icon.png");
    _comboBoxAlignment->addItem(iconAlignRight, QString("Right"));
    QIcon iconAlignJustify(":/icons/justifytext_icon.png");
    _comboBoxAlignment->addItem(iconAlignJustify, QString("Justify"));
    _comboBoxAlignment->setMinimumSize(QSize(0, 25));
    _comboBoxAlignment->setMaximumSize(QSize(40, 16777215));
    _comboBoxAlignment->setFocusPolicy(Qt::NoFocus);
    _comboBoxAlignment->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    _lineVert3 = new QFrame(_frameEditorToolBox);
    _lineVert3->setStyleSheet(QString("color: grey; background-color: grey;"));
    _lineVert3->setFrameShadow(QFrame::Sunken);
    _lineVert3->setLineWidth(1);
    _lineVert3->setFrameShape(QFrame::VLine);

    _buttonInsertTable = new QToolButton(_frameEditorToolBox);
    _buttonInsertTable->setMinimumSize(QSize(30, 25));
    _buttonInsertTable->setMaximumSize(QSize(30, 16777215));
    _buttonInsertTable->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonInsertTable->setFocusPolicy(Qt::NoFocus);
    _buttonInsertTable->setIcon(QIcon(":/icons/table_icon.png"));

    _buttonInsertImage = new QToolButton(_frameEditorToolBox);
    _buttonInsertImage->setMinimumSize(QSize(30, 25));
    _buttonInsertImage->setMaximumSize(QSize(30, 25));
    _buttonInsertImage->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonInsertImage->setFocusPolicy(Qt::NoFocus);
    _buttonInsertImage->setIcon(QIcon(":/icons/image_icon.png"));

    _buttonInsertLine = new QToolButton(_frameEditorToolBox);
    _buttonInsertLine->setMinimumSize(QSize(30, 25));
    _buttonInsertLine->setMaximumSize(QSize(30, 25));
    _buttonInsertLine->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonInsertLine->setFocusPolicy(Qt::NoFocus);
    _buttonInsertLine->setIcon(QIcon(":/icons/hr_icon.png"));

    _buttonInsertLink = new QToolButton(_frameEditorToolBox);
    _buttonInsertLink->setMinimumSize(QSize(30, 25));
    _buttonInsertLink->setMaximumSize(QSize(30, 16777215));
    _buttonInsertLink->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonInsertLink->setFocusPolicy(Qt::NoFocus);
    _buttonInsertLink->setIcon(QIcon(":/icons/link_icon.png"));

    _horSpacerTopToolBar = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    _horLayToolBox->addWidget(_fontComboBox);
    _horLayToolBox->addWidget(_spinBoxFontSize);
    _horLayToolBox->addWidget(_buttonEditorFontColor);
    _horLayToolBox->addWidget(_buttonEditorBackColor);
    _horLayToolBox->addWidget(_lineVert1);
    _horLayToolBox->addWidget(_buttonBold);
    _horLayToolBox->addWidget(_buttonItalic);
    _horLayToolBox->addWidget(_buttonUnderline);
    _horLayToolBox->addWidget(_buttonThrough);
    _horLayToolBox->addWidget(_buttonHighlight);
    _horLayToolBox->addWidget(_buttonCodeBlock);
    _horLayToolBox->addWidget(_lineVert2);
    _horLayToolBox->addWidget(_buttonBulletedList);
    _horLayToolBox->addWidget(_buttonOrderedList);
    _horLayToolBox->addWidget(_comboBoxAlignment);
    _horLayToolBox->addWidget(_lineVert3);
    _horLayToolBox->addWidget(_buttonInsertTable);
    _horLayToolBox->addWidget(_buttonInsertImage);
    _horLayToolBox->addWidget(_buttonInsertLine);
    _horLayToolBox->addWidget(_buttonInsertLink);
    _horLayToolBox->addItem(_horSpacerTopToolBar);

    _textEditor = new TextEditor(_frameEditor);
    QFont fontTextEditor;
    fontTextEditor.setPointSize(12);
    _textEditor->setFont(fontTextEditor);
    _textEditor->viewport()->setProperty("cursor", QVariant(QCursor(Qt::IBeamCursor)));
    _textEditor->setStyleSheet(common::openSheet(":/styles/textEditorStyle.qss"));
    _textEditor->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    _textEditor->setAutoFormatting(QTextEdit::AutoAll);
    _textEditor->setUndoRedoEnabled(true);
    _textEditor->setLineWrapMode(QTextEdit::WidgetWidth);
    _textEditor->setReadOnly(false);
    _textEditor->setTextInteractionFlags(
        Qt::LinksAccessibleByMouse | Qt::TextEditable | Qt::TextEditorInteraction | Qt::TextSelectableByKeyboard |
        Qt::TextSelectableByMouse);
    _textEditor->setOpenExternalLinks(true);
    _textEditor->setOpenLinks(true);

    _frameFind = new QFrame(_frameEditor);
    _frameFind->setStyleSheet(QString::fromUtf8("QFrame { \n"
        "	border: none;\n"
        "	padding: 0px;\n"
        "	margin: 0px;\n"
        "}"));
    _frameFind->setFrameShape(QFrame::StyledPanel);
    _frameFind->setFrameShadow(QFrame::Raised);
    _frameFind->setHidden(true);
    _horLayFind = new QHBoxLayout(_frameFind);
    _horLayFind->setContentsMargins(0, 1, 0, 1);

    _textFind = new QLineEdit(_frameFind);
    _textFind->setMinimumSize(QSize(300, 25));
    _textFind->setMaximumSize(QSize(300, 25));
    _textFind->setClearButtonEnabled(true);

    _buttonNext = new QPushButton("Next", _frameFind);
    _buttonNext->setMinimumSize(QSize(25, 25));
    _buttonNext->setMaximumSize(QSize(16777215, 25));
    _buttonNext->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonNext->setStyleSheet(QString("padding: 5px;"));

    _buttonPrevious = new QPushButton("Previous", _frameFind);
    _buttonPrevious->setMinimumSize(QSize(25, 25));
    _buttonPrevious->setMaximumSize(QSize(16777215, 25));
    _buttonPrevious->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonPrevious->setStyleSheet(QString("padding: 5px;"));

    _buttonCloseFind = new QPushButton(_frameFind);
    _buttonCloseFind->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonCloseFind->setStyleSheet(QString::fromUtf8("border: none;"));
    _buttonCloseFind->setIcon(QIcon(":/icons/close_icon.png"));
    _buttonCloseFind->setFlat(true);

    _labelFindMatchNum = new QLabel(_frameFind);

    _horSpacerFind = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    _horLayFind->addWidget(_textFind);
    _horLayFind->addWidget(_buttonNext);
    _horLayFind->addWidget(_buttonPrevious);
    _horLayFind->addWidget(_buttonCloseFind);
    _horLayFind->addWidget(_labelFindMatchNum);
    _horLayFind->addItem(_horSpacerFind);

    _vertLayEditor->addWidget(_labelTitle);
    _vertLayEditor->addWidget(_frameEditorToolBox);
    _vertLayEditor->addWidget(_textEditor);
    _vertLayEditor->addWidget(_frameFind);

    _mainSplitterPane->addWidget(_frameEditor);
    _horLayMain->addWidget(_mainSplitterPane);

    _frameRightToolBar = new QFrame(_centralWidget);
    _frameRightToolBar->setMinimumSize(QSize(32, 0));
    _frameRightToolBar->setStyleSheet(QString("QFrame { border: 0px; }"));
    _frameRightToolBar->setLineWidth(0);
    _vertLayToolBar = new QVBoxLayout(_frameRightToolBar);
    _vertLayToolBar->setContentsMargins(1, 82, 1, -1);

    _buttonUndo = new QToolButton(_frameRightToolBar);
    _buttonUndo->setMinimumSize(QSize(30, 25));
    _buttonUndo->setMaximumSize(QSize(30, 16777215));
    _buttonUndo->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonUndo->setFocusPolicy(Qt::NoFocus);
    _buttonUndo->setIcon(QIcon(":/icons/undo_icon.png"));

    _buttonRedo = new QToolButton(_frameRightToolBar);
    _buttonRedo->setMinimumSize(QSize(30, 25));
    _buttonRedo->setMaximumSize(QSize(30, 16777215));
    _buttonRedo->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonRedo->setFocusPolicy(Qt::NoFocus);
    _buttonRedo->setIcon(QIcon(":/icons/redo.png"));

    _buttonClearAll = new QToolButton(_frameRightToolBar);
    _buttonClearAll->setMinimumSize(QSize(30, 25));
    _buttonClearAll->setMaximumSize(QSize(30, 16777215));
    _buttonClearAll->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonClearAll->setFocusPolicy(Qt::NoFocus);
    _buttonClearAll->setIcon(QIcon(":/icons/cleartext_icon.png"));

    _horLine1 = new QFrame(_frameRightToolBar);
    _horLine1->setStyleSheet(QString::fromUtf8("color: grey;\n"
        "background-color: grey;"));
    _horLine1->setFrameShape(QFrame::HLine);
    _horLine1->setFrameShadow(QFrame::Sunken);

    _buttonSuperscript = new QToolButton(_frameRightToolBar);
    _buttonSuperscript->setMinimumSize(QSize(30, 25));
    _buttonSuperscript->setMaximumSize(QSize(30, 25));
    _buttonSuperscript->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonSuperscript->setFocusPolicy(Qt::NoFocus);
    _buttonSuperscript->setIcon(QIcon(":/icons/superscript_icon.png"));

    _buttonSubscript = new QToolButton(_frameRightToolBar);
    _buttonSubscript->setMinimumSize(QSize(30, 25));
    _buttonSubscript->setMaximumSize(QSize(30, 25));
    _buttonSubscript->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonSubscript->setFocusPolicy(Qt::NoFocus);
    _buttonSubscript->setIcon(QIcon(":/icons/subscript_icon.png"));

    _horLine2 = new QFrame(_frameRightToolBar);
    _horLine2->setStyleSheet(QString::fromUtf8("color: grey; background-color: grey;"));
    _horLine2->setFrameShape(QFrame::HLine);
    _horLine2->setFrameShadow(QFrame::Sunken);

    _buttonIncFontSize = new QToolButton(_frameRightToolBar);
    _buttonIncFontSize->setMinimumSize(QSize(30, 25));
    _buttonIncFontSize->setMaximumSize(QSize(30, 25));
    _buttonIncFontSize->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonIncFontSize->setFocusPolicy(Qt::NoFocus);
    _buttonIncFontSize->setIcon(QIcon(":/icons/increasefont_icon.png"));

    _buttonDecFontSize = new QToolButton(_frameRightToolBar);
    _buttonDecFontSize->setMinimumSize(QSize(30, 25));
    _buttonDecFontSize->setMaximumSize(QSize(30, 25));
    _buttonDecFontSize->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonDecFontSize->setFocusPolicy(Qt::NoFocus);
    _buttonDecFontSize->setIcon(QIcon(":/icons/decreasefont_icon.png"));

    _horLine3 = new QFrame(_frameRightToolBar);
    _horLine3->setStyleSheet(QString("color: grey; background-color: grey;"));
    _horLine3->setFrameShape(QFrame::HLine);
    _horLine3->setFrameShadow(QFrame::Sunken);

    _buttonIncIndent = new QToolButton(_frameRightToolBar);
    _buttonIncIndent->setMinimumSize(QSize(30, 25));
    _buttonIncIndent->setMaximumSize(QSize(30, 25));
    _buttonIncIndent->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonIncIndent->setFocusPolicy(Qt::NoFocus);
    _buttonIncIndent->setIcon(QIcon(":/icons/increaseindent_icon.png"));

    _buttonDecIndent = new QToolButton(_frameRightToolBar);
    _buttonDecIndent->setMinimumSize(QSize(30, 25));
    _buttonDecIndent->setMaximumSize(QSize(30, 25));
    _buttonDecIndent->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonDecIndent->setFocusPolicy(Qt::NoFocus);
    _buttonDecIndent->setIcon(QIcon(":/icons/decreaseindent_icon.png"));

    _horLine4 = new QFrame(_frameRightToolBar);
    _horLine4->setStyleSheet(QString("color: grey; background-color: grey;"));
    _horLine4->setFrameShape(QFrame::HLine);
    _horLine4->setFrameShadow(QFrame::Sunken);

    _vertLayToolBar->addWidget(_buttonUndo);
    _vertLayToolBar->addWidget(_buttonRedo);
    _vertLayToolBar->addWidget(_buttonClearAll);
    _vertLayToolBar->addWidget(_horLine1);
    _vertLayToolBar->addWidget(_buttonSuperscript);
    _vertLayToolBar->addWidget(_buttonSubscript);
    _vertLayToolBar->addWidget(_horLine2);
    _vertLayToolBar->addWidget(_buttonIncFontSize);
    _vertLayToolBar->addWidget(_buttonDecFontSize);
    _vertLayToolBar->addWidget(_horLine3);
    _vertLayToolBar->addWidget(_buttonIncIndent);
    _vertLayToolBar->addWidget(_buttonDecIndent);
    _vertLayToolBar->addWidget(_horLine4);
    _vertSpacerRightToolBar = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    _vertLayToolBar->addItem(_vertSpacerRightToolBar);
    _horLayMain->addWidget(_frameRightToolBar);

    // Actions
    setupActions();

    // MenuBar
    _menuBar = new QMenuBar(this);
    _menuBar->setGeometry(QRect(0, 0, 1035, 21));
    setMenuBar(_menuBar);

    // StatusBar
    _statusBar = new QStatusBar(this);
    setStatusBar(_statusBar);

    // Menus
    setupMenus();

    // Central Widget
    setCentralWidget(_centralWidget);
}

void SummaryWindow::setupActions() {
    // File Menu
    _actionSaveSummary = new QAction("Save Summary", this);
    _actionSaveSummary->setIcon(QIcon(":/icons/save_icon.png"));
    _actionPrintToPDF = new QAction("Print to PDF", this);
    _actionPrintToPDF->setIcon(QIcon(":/icons/pdf_icon.png"));
    _actionExportHtml = new QAction("Export to HTML", this);
    _actionExportHtml->setIcon(QIcon(":/icons/html_icon.png"));
    _actionClose = new QAction("Close", this);

    // Edit Menu
    _actionUndo = new QAction("Undo", this);
    _actionUndo->setIcon(QIcon(":/icons/undo_icon.png"));
    _actionRedo = new QAction("Redo", this);
    _actionRedo->setIcon(QIcon(":/icons/redo.png"));
    _actionCut = new QAction("Cut", this);
    _actionCut->setIcon(QIcon(":/icons/cut_icon.png"));
    _actionCopy = new QAction("Copy", this);
    _actionCopy->setIcon(QIcon(":/icons/copy_icon.png"));
    _actionCopyFormatting = new QAction("Copy Formatting", this);
    _actionPaste = new QAction("Paste", this);
    _actionPaste->setIcon(QIcon(":/icons/paste_icon.png"));
    _actionPasteMatchFormat = new QAction("Paste and Match Format", this);
    _actionSelectAll = new QAction("Select All", this);
    _actionClearAll = new QAction("Clear All", this);
    _actionClearAll->setIcon(QIcon(":/icons/cleartext_icon.png"));
    _actionSelectCurrentLine = new QAction("Select Current Line", this);
    _actionDuplicateCurrentLine = new QAction("Duplicate Current Line", this);
    _actionDeleteCurrentLine = new QAction("Delete Current Line", this);
    _actionFind = new QAction("Find", this);

    // View Menu
    _actionFullscreen = new QAction("Fullscreen", this);
    _actionFullscreen->setIcon(QIcon(":/icons/fullscreen_icon.png"));
    _actionMaximize = new QAction("Maximize", this);
    _actionMinimize = new QAction("Minimize", this);
    _actionMinimize->setIcon(QIcon(":/icons/minimize_icon.png"));
    _actionZoomIn = new QAction("Zoom In", this);
    _actionZoomIn->setIcon(QIcon(":/icons/zoo_in_icon.png"));
    _actionZoomOut = new QAction("Zoom Out", this);
    _actionZoomOut->setIcon(QIcon(":/icons/zoo_out_icon.png"));
    _actionHideSearchBar = new QAction("Hide Search Bar", this);
    _actionHideListWidget = new QAction("Hide List Widget", this);
    _actionHideLeftPane = new QAction("Hide Left Pane", this);
    _actionHideTopToolbar = new QAction("Hide Top Toolbar", this);
    _actionHideRightToolbar = new QAction("Hide Right Toolbar", this);
    _actionHideRightPane = new QAction("Hide Right Pane", this);

    // Insert Menu
    _actionInsertLine = new QAction("Line", this);
    _actionInsertLine->setIcon(QIcon(":/icons/hr_icon.png"));
    _actionInsertLink = new QAction("Link", this);
    _actionInsertLink->setIcon(QIcon(":/icons/link_icon.png"));
    _actionInsertImage = new QAction("Image", this);
    _actionInsertImage->setIcon(QIcon(":/icons/image_icon.png"));
    _actionInsertTable = new QAction("Table", this);
    _actionInsertTable->setIcon(QIcon(":/icons/table_icon.png"));
    _actionTableRow = new QAction("Table Row", this);
    _actionTableColumn = new QAction("Table Column", this);
    _actionDateNormal = new QAction("Normal Date", this);
    _actionDateShort = new QAction("Short Date", this);

    // Format Menu
    _actionBold = new QAction("Bold", this);
    _actionBold->setIcon(QIcon(":/icons/bold_icon.png"));
    _actionItalic = new QAction("Italic", this);
    _actionItalic->setIcon(QIcon(":/icons/italic_icon.png"));
    _actionUnderline = new QAction("Underline", this);
    _actionUnderline->setIcon(QIcon(":/icons/underline_icon.png"));
    _actionThrough = new QAction("Through", this);
    _actionThrough->setIcon(QIcon(":/icons/strikethrough_icon.png"));
    _actionHighlight = new QAction("Highlight", this);
    _actionHighlight->setIcon(QIcon(":/icons/highlight_icon.png"));
    _actionCodeBlock = new QAction("Code", this);
    _actionCodeBlock->setIcon(QIcon(":/icons/code_icon.png"));
    _actionBulletedList = new QAction("Bulleted List", this);
    _actionBulletedList->setIcon(QIcon(":/icons/bullets_icon.png"));
    _actionOrderedList = new QAction("Ordered List", this);
    _actionOrderedList->setIcon(QIcon(":/icons/numbers_icon.png"));
    _actionIncFontSize = new QAction("+ Font Size", this);
    _actionIncFontSize->setIcon(QIcon(":/icons/increasefont_icon.png"));
    _actionDecFontSize = new QAction("- Font Size", this);
    _actionDecFontSize->setIcon(QIcon(":/icons/decreasefont_icon.png"));
    _actionIncIndent = new QAction("+ Indent", this);
    _actionIncIndent->setIcon(QIcon(":/icons/increaseindent_icon.png"));
    _actionDecIndent = new QAction("- Indent", this);
    _actionDecIndent->setIcon(QIcon(":/icons/decreaseindent_icon.png"));
    _actionSuperscript = new QAction("Superscript", this);
    _actionSuperscript->setIcon(QIcon(":/icons/superscript_icon.png"));
    _actionSubscript = new QAction("Subscript", this);
    _actionSubscript->setIcon(QIcon(":/icons/subscript_icon.png"));
    _actionResetFormat = new QAction("Reset Format", this);
    _actionResetFormat->setIcon(QIcon(":/icons/restore_icon.png"));
    _actionSentenceCase = new QAction("Sentence Case", this);
    _actionSentenceCase->setIcon(QIcon(":/icons/sentencecase_icon.png"));
    _actionUpperCase = new QAction("Uppercase", this);
    _actionUpperCase->setIcon(QIcon(":/icons/uppercase_icon.png"));
    _actionLowerCase = new QAction("Lowercase", this);
    _actionLowerCase->setIcon(QIcon(":/icons/lowercase_icon.png"));
    _actionCapitalCase = new QAction("Capitalcase", this);

    // Search Menu
    _actionSearchText = new QAction("Search Text", this);
    _actionSearchText->setIcon(QIcon(":/icons/searchbar_icon.png"));
    _actionClearSearch = new QAction("Clear Search", this);
    _actionClearSearch->setIcon(QIcon(":/icons/clear_icon.png"));
}

void SummaryWindow::setupMenus() {
    _menuFile = _menuBar->addMenu(tr("&File"));
    _menuEdit = _menuBar->addMenu(tr("&Edit"));
    _menuView = _menuBar->addMenu(tr("&View"));
    _menuInsert = _menuBar->addMenu(tr("&Insert"));
    _menuFormat = _menuBar->addMenu(tr("&Format"));
    _menuSearch = _menuBar->addMenu(tr("&Search"));

    _menuDateTime = new QMenu(tr("Date"), _menuInsert);
    _menuChangeCase = new QMenu(tr("Change Case"), _menuFormat);

    // Add Actions to Menus
    _menuBar->addAction(_menuFile->menuAction());
    _menuBar->addAction(_menuEdit->menuAction());
    _menuBar->addAction(_menuView->menuAction());
    _menuBar->addAction(_menuInsert->menuAction());
    _menuBar->addAction(_menuFormat->menuAction());
    _menuBar->addAction(_menuSearch->menuAction());
    _menuFile->addAction(_actionSaveSummary);
    _menuFile->addAction(_actionPrintToPDF);
    _menuFile->addAction(_actionExportHtml);
    _menuFile->addSeparator();
    _menuFile->addAction(_actionClose);
    _menuEdit->addAction(_actionUndo);
    _menuEdit->addAction(_actionRedo);
    _menuEdit->addSeparator();
    _menuEdit->addAction(_actionCut);
    _menuEdit->addAction(_actionCopy);
    _menuEdit->addAction(_actionCopyFormatting);
    _menuEdit->addSeparator();
    _menuEdit->addAction(_actionPaste);
    _menuEdit->addAction(_actionPasteMatchFormat);
    _menuEdit->addSeparator();
    _menuEdit->addAction(_actionSelectAll);
    _menuEdit->addAction(_actionClearAll);
    _menuEdit->addSeparator();
    _menuEdit->addAction(_actionSelectCurrentLine);
    _menuEdit->addAction(_actionDuplicateCurrentLine);
    _menuEdit->addAction(_actionDeleteCurrentLine);
    _menuEdit->addSeparator();
    _menuEdit->addAction(_actionFind);
    _menuView->addAction(_actionFullscreen);
    _menuView->addAction(_actionMaximize);
    _menuView->addAction(_actionMinimize);
    _menuView->addSeparator();
    _menuView->addAction(_actionZoomIn);
    _menuView->addAction(_actionZoomOut);
    _menuView->addSeparator();
    _menuView->addAction(_actionHideSearchBar);
    _menuView->addAction(_actionHideLeftPane);
    _menuView->addSeparator();
    _menuView->addAction(_actionHideTopToolbar);
    _menuView->addAction(_actionHideRightToolbar);
    _menuView->addAction(_actionHideRightPane);
    _menuInsert->addAction(_actionInsertLine);
    _menuInsert->addAction(_actionInsertLink);
    _menuInsert->addAction(_actionInsertImage);
    _menuInsert->addAction(_actionInsertTable);
    _menuInsert->addSeparator();
    _menuInsert->addAction(_actionTableRow);
    _menuInsert->addAction(_actionTableColumn);
    _menuInsert->addSeparator();
    _menuInsert->addAction(_menuDateTime->menuAction());
    _menuDateTime->addAction(_actionDateShort);
    _menuDateTime->addAction(_actionDateNormal);
    _menuFormat->addAction(_actionBold);
    _menuFormat->addAction(_actionItalic);
    _menuFormat->addAction(_actionUnderline);
    _menuFormat->addAction(_actionThrough);
    _menuFormat->addAction(_actionHighlight);
    _menuFormat->addAction(_actionCodeBlock);
    _menuFormat->addSeparator();
    _menuFormat->addAction(_actionBulletedList);
    _menuFormat->addAction(_actionOrderedList);
    _menuFormat->addSeparator();
    _menuFormat->addAction(_actionIncFontSize);
    _menuFormat->addAction(_actionDecFontSize);
    _menuFormat->addSeparator();
    _menuFormat->addAction(_actionIncIndent);
    _menuFormat->addAction(_actionDecIndent);
    _menuFormat->addSeparator();
    _menuFormat->addAction(_actionSuperscript);
    _menuFormat->addAction(_actionSubscript);
    _menuFormat->addSeparator();
    _menuFormat->addAction(_menuChangeCase->menuAction());
    _menuFormat->addSeparator();
    _menuFormat->addAction(_actionResetFormat);
    _menuChangeCase->addAction(_actionSentenceCase);
    _menuChangeCase->addAction(_actionUpperCase);
    _menuChangeCase->addAction(_actionLowerCase);
    _menuChangeCase->addAction(_actionCapitalCase);
    _menuSearch->addAction(_actionSearchText);
    _menuSearch->addAction(_actionClearSearch);
}

void SummaryWindow::setupConnections() {
    connect(_buttonBold, &QPushButton::clicked, this, &SummaryWindow::toggleBold);
    connect(_buttonItalic, &QPushButton::clicked, this, &SummaryWindow::toggleItalic);
    connect(_buttonUnderline, &QPushButton::clicked, this, &SummaryWindow::toggleUnderline);
    connect(_buttonThrough, &QPushButton::clicked, this, &SummaryWindow::toggleStrikethrough);
    connect(_buttonBulletedList, &QPushButton::clicked, this, &SummaryWindow::toggleBulletedList);
    connect(_buttonUndo, &QPushButton::clicked, _textEditor, &TextEditor::undo);
    connect(_buttonRedo, &QPushButton::clicked, _textEditor, &TextEditor::redo);
    connect(_buttonClearAll, &QPushButton::clicked, _textEditor, &TextEditor::clear);
    connect(_buttonOrderedList, &QPushButton::clicked, this, &SummaryWindow::toggleOrderedList);
    connect(_buttonSubscript, &QPushButton::clicked, this, &SummaryWindow::setSubscript);
    connect(_buttonSuperscript, &QPushButton::clicked, this, &SummaryWindow::setSuperscript);
    connect(_buttonCodeBlock, &QPushButton::clicked, this, &SummaryWindow::insertCodeBlock);
    connect(_buttonHighlight, &QPushButton::clicked, this, &SummaryWindow::toggleHighlight);
    connect(_buttonEditorFontColor, &QPushButton::clicked, this, &SummaryWindow::setFontColor);
    connect(_buttonInsertLine, &QPushButton::clicked, this, &SummaryWindow::insertLine);
    connect(_buttonInsertImage, &QPushButton::clicked, this, &SummaryWindow::insertImage);
    connect(_buttonInsertTable, &QPushButton::clicked, this, &SummaryWindow::insertTable);
    connect(_buttonInsertLink, &QPushButton::clicked, this, &SummaryWindow::insertLink);
    connect(_buttonEditorBackColor, &QPushButton::clicked, this, &SummaryWindow::setBackColor);
    connect(_buttonNext, &QPushButton::clicked, this, &SummaryWindow::findNext);
    connect(_buttonPrevious, &QPushButton::clicked, this, &SummaryWindow::findPrevious);
    connect(_buttonIncFontSize, &QPushButton::clicked, this, &SummaryWindow::increaseFontSize);
    connect(_buttonDecFontSize, &QPushButton::clicked, this, &SummaryWindow::decreaseFontSize);
    connect(_buttonIncIndent, &QPushButton::clicked, this, &SummaryWindow::increaseIndent);
    connect(_buttonDecIndent, &QPushButton::clicked, this, &SummaryWindow::decreaseIndent);
    connect(_buttonCloseFind, &QPushButton::clicked, [this] {
        toggleFindWidget(false);
    });
    connect(_fontComboBox, &QFontComboBox::currentTextChanged, [this] {
        setTextFontFamily(_fontComboBox->currentText(), true);
    });
    connect(_spinBoxFontSize, &QSpinBox::valueChanged, [this] {
        setTextFontSize(_spinBoxFontSize->value(), true);
    });

    connect(_textFind, &QLineEdit::textChanged, this, &SummaryWindow::findText);
    connect(_comboBoxAlignment, &QComboBox::currentIndexChanged, this, &SummaryWindow::changeTextAlignment);
    connect(_bookSearchWidget, &BookSearchWidget::selectionChanged, this, &SummaryWindow::selectEbookSummary);
    connect(_bookSearchWidget, &BookSearchWidget::itemClicked, this, &SummaryWindow::selectEbookSummary);
    connect(_textEditor, &TextEditor::currentCharFormatChanged, this, &SummaryWindow::textFormatChanged);
    connect(_textEditor, &TextEditor::selectionChanged, this, &SummaryWindow::textFormatChanged);

    connect(_actionClose, &QAction::triggered, this, &SummaryWindow::close);
    connect(_actionMaximize, &QAction::triggered, [this] { common::toggleMaximized(this); });
    connect(_actionFullscreen, &QAction::triggered, [this] { common::toggleFullscreen(this); });
    connect(_actionMinimize, &QAction::triggered, this, &SummaryWindow::showMinimized);
    connect(_actionHideSearchBar, &QAction::triggered, this, &SummaryWindow::hideSearchBar);
    connect(_actionHideLeftPane, &QAction::triggered, this, &SummaryWindow::hideLeftPane);
    connect(_actionHideTopToolbar, &QAction::triggered, this, &SummaryWindow::hideTopToolbar);
    connect(_actionHideRightToolbar, &QAction::triggered, this, &SummaryWindow::hideRightToolbar);
    connect(_actionHideRightPane, &QAction::triggered, this, &SummaryWindow::hideRightPane);
    connect(_actionResetFormat, &QAction::triggered, this, &SummaryWindow::resetFormat);
    connect(_actionPaste, &QAction::triggered, _textEditor, &TextEditor::paste);
    connect(_actionSearchText, &QAction::triggered, this, &SummaryWindow::searchText);
    connect(_actionClearSearch, &QAction::triggered, this, &SummaryWindow::clearSearch);
    connect(_actionPrintToPDF, &QAction::triggered, this, &SummaryWindow::printSummaryToPdf);
    connect(_actionCopyFormatting, &QAction::triggered, this, &SummaryWindow::copyFormatting);
    connect(_actionPasteMatchFormat, &QAction::triggered, this, &SummaryWindow::pasteMatchFormat);
    connect(_actionSaveSummary, &QAction::triggered, this, &SummaryWindow::saveSummary);
    connect(_actionExportHtml, &QAction::triggered, this, &SummaryWindow::exportSummaryToHtml);
    connect(_actionSentenceCase, &QAction::triggered, this, &SummaryWindow::sentenceCase);
    connect(_actionUpperCase, &QAction::triggered, this, &SummaryWindow::upperCase);
    connect(_actionLowerCase, &QAction::triggered, this, &SummaryWindow::lowerCase);
    connect(_actionCapitalCase, &QAction::triggered, this, &SummaryWindow::capitalCase);
    connect(_actionSelectCurrentLine, &QAction::triggered, this, &SummaryWindow::selectCurrentLine);
    connect(_actionDuplicateCurrentLine, &QAction::triggered, this, &SummaryWindow::duplicateCurrentLine);
    connect(_actionDeleteCurrentLine, &QAction::triggered, this, &SummaryWindow::deleteCurrentLine);
    connect(_actionZoomIn, &QAction::triggered, this, &SummaryWindow::zoomIn);
    connect(_actionZoomOut, &QAction::triggered, this, &SummaryWindow::zoomOut);
    connect(_actionDateNormal, &QAction::triggered, this, &SummaryWindow::insertNormalDate);
    connect(_actionDateShort, &QAction::triggered, this, &SummaryWindow::insertShortDate);
    connect(_actionTableRow, &QAction::triggered, this, &SummaryWindow::addTableRow);
    connect(_actionTableColumn, &QAction::triggered, this, &SummaryWindow::addTableColumn);
    connect(_actionFind, &QAction::triggered, [this] {
        toggleFindWidget(!_frameFind->isVisible());
    });
    connect(_actionBold, &QAction::triggered, this, &SummaryWindow::toggleBold);
    connect(_actionItalic, &QAction::triggered, this, &SummaryWindow::toggleItalic);
    connect(_actionUnderline, &QAction::triggered, this, &SummaryWindow::toggleUnderline);
    connect(_actionThrough, &QAction::triggered, this, &SummaryWindow::toggleStrikethrough);
    connect(_actionBulletedList, &QAction::triggered, this, &SummaryWindow::toggleBulletedList);
    connect(_actionOrderedList, &QAction::triggered, this, &SummaryWindow::toggleOrderedList);
    connect(_actionSubscript, &QAction::triggered, this, &SummaryWindow::setSubscript);
    connect(_actionSuperscript, &QAction::triggered, this, &SummaryWindow::setSuperscript);
    connect(_actionCodeBlock, &QAction::triggered, this, &SummaryWindow::insertCodeBlock);
    connect(_actionHighlight, &QAction::triggered, this, &SummaryWindow::toggleHighlight);
    connect(_actionIncFontSize, &QAction::triggered, this, &SummaryWindow::increaseFontSize);
    connect(_actionDecFontSize, &QAction::triggered, this, &SummaryWindow::decreaseFontSize);
    connect(_actionIncIndent, &QAction::triggered, this, &SummaryWindow::increaseIndent);
    connect(_actionDecIndent, &QAction::triggered, this, &SummaryWindow::decreaseIndent);
    connect(_actionUndo, &QAction::triggered, _textEditor, &TextEditor::undo);
    connect(_actionRedo, &QAction::triggered, _textEditor, &TextEditor::redo);
    connect(_actionClearAll, &QAction::triggered, _textEditor, &TextEditor::clear);
    connect(_actionCut, &QAction::triggered, _textEditor, &TextEditor::cut);
    connect(_actionCopy, &QAction::triggered, _textEditor, &TextEditor::copy);
    connect(_actionSelectAll, &QAction::triggered, _textEditor, &TextEditor::selectAll);
    connect(_actionInsertLine, &QAction::triggered, this, &SummaryWindow::insertLine);
    connect(_actionInsertLink, &QAction::triggered, this, &SummaryWindow::insertLink);
    connect(_actionInsertImage, &QAction::triggered, this, &SummaryWindow::insertImage);
    connect(_actionInsertTable, &QAction::triggered, this, &SummaryWindow::insertTable);
}

void SummaryWindow::setupTabOrder() {
    QWidget::setTabOrder(_bookSearchWidget, _textEditor);
    QWidget::setTabOrder(_textEditor, _fontComboBox);
    QWidget::setTabOrder(_fontComboBox, _spinBoxFontSize);
    QWidget::setTabOrder(_spinBoxFontSize, _buttonEditorFontColor);
    QWidget::setTabOrder(_buttonEditorFontColor, _buttonEditorBackColor);
    QWidget::setTabOrder(_buttonEditorBackColor, _buttonBold);
    QWidget::setTabOrder(_buttonBold, _buttonItalic);
    QWidget::setTabOrder(_buttonItalic, _buttonUnderline);
    QWidget::setTabOrder(_buttonUnderline, _buttonThrough);
    QWidget::setTabOrder(_buttonThrough, _buttonHighlight);
    QWidget::setTabOrder(_buttonHighlight, _buttonCodeBlock);
    QWidget::setTabOrder(_buttonCodeBlock, _buttonBulletedList);
    QWidget::setTabOrder(_buttonBulletedList, _buttonOrderedList);
    QWidget::setTabOrder(_buttonOrderedList, _comboBoxAlignment);
    QWidget::setTabOrder(_comboBoxAlignment, _buttonInsertTable);
    QWidget::setTabOrder(_buttonInsertTable, _buttonInsertImage);
    QWidget::setTabOrder(_buttonInsertImage, _buttonInsertLine);
    QWidget::setTabOrder(_buttonInsertLine, _buttonInsertLink);
    QWidget::setTabOrder(_buttonInsertLink, _buttonUndo);
    QWidget::setTabOrder(_buttonUndo, _buttonRedo);
    QWidget::setTabOrder(_buttonRedo, _buttonClearAll);
    QWidget::setTabOrder(_buttonClearAll, _buttonSuperscript);
    QWidget::setTabOrder(_buttonSuperscript, _buttonSubscript);
    QWidget::setTabOrder(_buttonSubscript, _buttonIncFontSize);
    QWidget::setTabOrder(_buttonIncFontSize, _buttonDecFontSize);
    QWidget::setTabOrder(_buttonDecFontSize, _buttonIncIndent);
    QWidget::setTabOrder(_buttonIncIndent, _buttonDecIndent);
}

void SummaryWindow::setupShortcuts() {
    // File Menu
    _actionClose->setShortcut(QKeySequence("Ctrl+Q"));

    // Edit Menu
    _actionUndo->setShortcut(QKeySequence("Ctrl+Z"));
    _actionRedo->setShortcut(QKeySequence("Ctrl+Y"));
    _actionCut->setShortcut(QKeySequence("Ctrl+C"));
    _actionCopy->setShortcut(QKeySequence("Ctrl+X"));
    _actionCopyFormatting->setShortcut(QKeySequence("Ctrl+Shift+C"));
    _actionPaste->setShortcut(QKeySequence("Ctrl+V"));
    _actionPasteMatchFormat->setShortcut(QKeySequence("Ctrl+Shift+V"));
    _actionSelectAll->setShortcut(QKeySequence("Ctrl+A"));
    _actionClearAll->setShortcut(QKeySequence("Ctrl+Shift+Del"));
    _actionDeleteCurrentLine->setShortcut(QKeySequence("Ctrl+D, Ctrl+D"));
    _actionDuplicateCurrentLine->setShortcut(QKeySequence("Ctrl+D, Ctrl+L"));
    _actionFind->setShortcut(QKeySequence("Ctrl+Shift+F"));

    // View Menu
    _actionFullscreen->setShortcut(QKeySequence("Ctrl+F, Ctrl+S"));
    _actionMaximize->setShortcut(QKeySequence("Ctrl+M, Ctrl+X"));
    _actionMinimize->setShortcut(QKeySequence("Ctrl+M, Ctrl+N"));
    _actionZoomIn->setShortcut(QKeySequence("Ctrl+="));
    _actionZoomOut->setShortcut(QKeySequence("Ctrl+-"));
    _actionHideSearchBar->setShortcut(QKeySequence("Ctrl+H, Ctrl+S"));
    _actionHideLeftPane->setShortcut(QKeySequence("Ctrl+L, Ctrl+P"));
    _actionHideTopToolbar->setShortcut(QKeySequence("Ctrl+H, Ctrl+T"));
    _actionHideRightToolbar->setShortcut(QKeySequence("Ctrl+H, Ctrl+R"));
    _actionHideRightPane->setShortcut(QKeySequence("Ctrl+R, Ctrl+P"));

    // Insert Menu
    _actionTableRow->setShortcut(QKeySequence("Ctrl+R, Ctrl+R"));
    _actionTableColumn->setShortcut(QKeySequence("Ctrl+C, Ctrl+C"));

    // Format Menu
    _actionBold->setShortcut(QKeySequence("Ctrl+B"));
    _actionItalic->setShortcut(QKeySequence("Ctrl+I"));
    _actionUnderline->setShortcut(QKeySequence("Ctrl+U"));
    _actionThrough->setShortcut(QKeySequence("Ctrl+T"));
    _actionHighlight->setShortcut(QKeySequence("Ctrl+Shift+H"));
    _actionCodeBlock->setShortcut(QKeySequence("Ctrl+Shift+L"));
    _actionBulletedList->setShortcut(QKeySequence("Ctrl+Shift+B"));
    _actionOrderedList->setShortcut(QKeySequence("Ctrl+Shift+O"));
    _actionIncFontSize->setShortcut(QKeySequence("Ctrl+Shift+="));
    _actionDecFontSize->setShortcut(QKeySequence("Ctrl+Shift+-"));
    _actionIncIndent->setShortcut(QKeySequence("Ctrl+Shift+["));
    _actionDecIndent->setShortcut(QKeySequence("Ctrl+Shift+]"));
    _actionSuperscript->setShortcut(QKeySequence("Ctrl+Shift+Up"));
    _actionSubscript->setShortcut(QKeySequence("Ctrl+Shift+Down"));
    _actionResetFormat->setShortcut(QKeySequence("Ctrl+Space"));

    // Search Menu
    _actionSearchText->setShortcut(QKeySequence("Ctrl+S, Ctrl+T"));
    _actionClearSearch->setShortcut(QKeySequence("Ctrl+C, Ctrl+S"));
}

void SummaryWindow::closeEvent(QCloseEvent *event) {
    if (!_labelTitle->text().isEmpty()) {
        queries::updateSummary(_labelTitle->text(), _textEditor->toHtml());
    }
    event->accept();
}

void SummaryWindow::selectEbookSummary(const QString &name) {
    if (!name.isEmpty()) {
        openEbookSummary(_bookSearchWidget->findItem(name));
    }
}

void SummaryWindow::toggleBold() {
    QFont currentFont = _textEditor->currentFont();
    currentFont.setBold(!currentFont.bold());
    _textEditor->setCurrentFont(currentFont);
    textFormatChanged();
}

void SummaryWindow::toggleItalic() {
    QFont currentFont = _textEditor->currentFont();
    currentFont.setItalic(!currentFont.italic());
    _textEditor->setCurrentFont(currentFont);
    textFormatChanged();
}

void SummaryWindow::toggleUnderline() {
    QFont currentFont = _textEditor->currentFont();
    currentFont.setUnderline(!currentFont.underline());
    _textEditor->setCurrentFont(currentFont);
    textFormatChanged();
}

void SummaryWindow::toggleStrikethrough() {
    QFont currentFont = _textEditor->currentFont();
    currentFont.setStrikeOut(!currentFont.strikeOut());
    _textEditor->setCurrentFont(currentFont);
    textFormatChanged();
}

void SummaryWindow::setTextFontFamily(const QString &arg1, bool change) {
    if (change) {
        _textEditor->setCurrentFont(arg1);
    } else {
        _fontComboBox->blockSignals(true);
        _fontComboBox->setCurrentFont(arg1);
        _fontComboBox->blockSignals(false);
    }
}

void SummaryWindow::setTextFontSize(int arg1, bool change) {
    if (change) {
        QFont currentFont = _textEditor->currentFont();
        currentFont.setPointSize(arg1);
        _textEditor->setCurrentFont(currentFont);
    } else {
        _spinBoxFontSize->blockSignals(true);
        _spinBoxFontSize->setValue(arg1);
        _spinBoxFontSize->blockSignals(false);
    }
}

void SummaryWindow::toggleBulletedList() {
    _textEditor->createList(QTextListFormat::ListDisc);
    textFormatChanged();
}

void SummaryWindow::toggleOrderedList() {
    _textEditor->createList(QTextListFormat::ListDecimal);
    textFormatChanged();
}

void SummaryWindow::changeTextAlignment(int index) {
    switch (index) {
        case 0:
            _textEditor->setAlignment(Qt::AlignLeft);
            break;
        case 1:
            _textEditor->setAlignment(Qt::AlignHCenter);
            break;
        case 2:
            _textEditor->setAlignment(Qt::AlignRight);
            break;
        case 3:
            _textEditor->setAlignment(Qt::AlignJustify);
            break;
        default:
            _textEditor->setAlignment(Qt::AlignLeft);
            break;
    }
}

void SummaryWindow::setSubscript() {
    QTextCharFormat currentFormat = _textEditor->currentCharFormat();
    currentFormat.setVerticalAlignment(QTextCharFormat::AlignSubScript);
    _textEditor->setCurrentCharFormat(currentFormat);
}

void SummaryWindow::setSuperscript() {
    QTextCharFormat currentFormat = _textEditor->currentCharFormat();
    currentFormat.setVerticalAlignment(QTextCharFormat::AlignSuperScript);
    _textEditor->setCurrentCharFormat(currentFormat);
}

void SummaryWindow::insertCodeBlock() {
    QTextFrameFormat frameFormat;
    frameFormat.setBackground(QBrush(QColor(QString("#FBFAF8"))));
    frameFormat.setBorder(1);
    frameFormat.setBorderBrush(QBrush(QColor(QString("#D6D5D3"))));
    frameFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
    frameFormat.setMargin(0);
    frameFormat.setPadding(4);

    int fontSize = _textEditor->currentFont().pointSize();
    QFont font = QFont("Consolas");
    font.setPointSize(fontSize);

    _textEditor->textCursor().insertFrame(frameFormat);
    _textEditor->setCurrentFont(font);
}

void SummaryWindow::textFormatChanged() {
    setTextFontFamily(_textEditor->currentFont().family(), false);
    setTextFontSize(_textEditor->currentFont().pointSize(), false);
    _buttonEditorFontColor->setStyleSheet("background-color: " + _textEditor->textColor().name());

    _buttonBold->setStyleSheet(
        "background-color: " + QString(_textEditor->currentFont().bold() ? "#1C3387" : "#2D2D30"));
    _buttonItalic->setStyleSheet(
        "background-color: " + QString(_textEditor->currentFont().italic() ? "#1C3387" : "#2D2D30"));
    _buttonUnderline->setStyleSheet(
        "background-color: " + QString(_textEditor->currentFont().underline() ? "#1C3387" : "#2D2D30"));
    _buttonThrough->setStyleSheet(
        "background-color: " + QString(_textEditor->currentFont().strikeOut() ? "#1C3387" : "#2D2D30"));
    _buttonHighlight->setStyleSheet("background-color: " +
                                     QString(_textEditor->currentCharFormat().background() == Qt::yellow
                                                 ? "#1C3387"
                                                 : "#2D2D30"));
}

void SummaryWindow::toggleHighlight() {
    QTextCharFormat currentFormat = _textEditor->currentCharFormat();
    currentFormat.setBackground(currentFormat.background() == Qt::yellow ? Qt::white : Qt::yellow);
    _textEditor->setCurrentCharFormat(currentFormat);
    textFormatChanged();
}

void SummaryWindow::setFontColor() {
    QPoint bottom = _buttonEditorFontColor->rect().bottomLeft();
    QPoint globalPos = _buttonEditorFontColor->mapToGlobal(bottom);
    globalPos.setY(globalPos.y() + 2);

    ColorPickerWidget *widget = new ColorPickerWidget(this, Qt::black);
    widget->move(globalPos);
    common::openDialog(widget, ":/styles/colorpickerstyle.qss");

    if (!widget->colorSelected()) {
        return;
    }

    QString colorSheet = "background-color: " + widget->getCurrentColor().name();
    _buttonEditorFontColor->setStyleSheet(colorSheet);
    _buttonEditorFontColor->update();

    _textEditor->setTextColor(widget->getCurrentColor());
}

void SummaryWindow::hideSearchBar() {
    _bookSearchWidget->setHideSearchBar(!_bookSearchWidget->searchBarHidden());
    _actionHideSearchBar->setText(!_bookSearchWidget->searchBarHidden() ? "Show Search Bar" : "Hide Search Bar");
}

void SummaryWindow::hideLeftPane() {
    _bookSearchWidget->setHideWidget(!_bookSearchWidget->widgetHidden());
    _mainSplitterPane->refresh();
    _actionHideLeftPane->setText(_bookSearchWidget->widgetHidden() ? "Show Left Pane" : "Hide Left Pane");
}

void SummaryWindow::hideTopToolbar() {
    common::changeWidgetVisibility(_frameEditorToolBox, _actionHideTopToolbar);
}

void SummaryWindow::hideRightToolbar() {
    common::changeWidgetVisibility(_frameRightToolBar, _actionHideRightToolbar);
}

void SummaryWindow::hideRightPane() {
    _frameEditor->setHidden(!_frameEditor->isHidden());
    _mainSplitterPane->refresh();
    hideRightToolbar();
    _actionHideRightPane->setText(_frameEditor->isHidden() ? "Show Right Pane" : "Hide Right Pane");
}

void SummaryWindow::insertLine() {
    QTextCursor cursor = _textEditor->textCursor();
    cursor.insertHtml(QString("<hr><br>"));
    cursor.movePosition(QTextCursor::Up, QTextCursor::MoveAnchor, 1);
    _textEditor->setTextCursor(cursor);
}

void SummaryWindow::resetFormat() {
    QFont currentFont = _textEditor->currentFont();
    currentFont.setBold(false);
    currentFont.setItalic(false);
    currentFont.setStrikeOut(false);
    currentFont.setUnderline(false);

    QTextCharFormat currentFormat = _textEditor->currentCharFormat();
    currentFormat.setBackground(_textEditor->palette().base().color());
    currentFormat.setVerticalAlignment(QTextCharFormat::AlignNormal);

    _textEditor->setCurrentCharFormat(currentFormat);
    _textEditor->setCurrentFont(currentFont);

    textFormatChanged();
}

void SummaryWindow::insertImage() {
    QString file = QFileDialog::getOpenFileName(this,
                                                tr("Select Image"), "/",
                                                tr("Image Files (*.jpeg *.jpg *.png *.gif *.apng *.svg *.bmp *.ico)"));

    QUrl Uri(QString("%1").arg(file));
    QImage image = QImageReader(file).read();

    QTextDocument *textDocument = _textEditor->document();
    textDocument->addResource(QTextDocument::ImageResource, Uri, QVariant(image));
    QTextCursor cursor = _textEditor->textCursor();
    QTextImageFormat imageFormat;
    imageFormat.setWidth(image.width());
    imageFormat.setHeight(image.height());
    imageFormat.setName(Uri.toString());
    cursor.insertImage(imageFormat);
}

void SummaryWindow::searchText() {
    if (!_labelTitle->text().isEmpty()) {
        queries::updateSummary(_labelTitle->text(), _textEditor->toHtml());
    }
    _bookSearchWidget->searchString();
}

void SummaryWindow::clearSearch() {
    _bookSearchWidget->clearSearchText();
    _bookSearchWidget->clearListWidget();
}

void SummaryWindow::openEbookSummary(const QString &name) {
    if (!_labelTitle->text().isEmpty()) {
        queries::updateSummary(_labelTitle->text(), _textEditor->toHtml());
    }

    _textEditor->clear();
    _labelTitle->setText(name);
    queries::selectSummaryBasedonName(name);
    queries::query.next();
    QString summary = queries::query.value(0).toString();
    if (summary != "N/A") {
        _textEditor->insertHtml(summary);
    }
}

void SummaryWindow::printSummaryToPdf() {
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save File"), "/", tr("*.pdf"));

    QPrinter printer(QPrinter::HighResolution);
    printer.setPrintRange(QPrinter::CurrentPage);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);
    _textEditor->print(&printer);
    QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
}

void SummaryWindow::insertTable() {
    InsertTableDialog dialog(this);
    common::openDialog(&dialog, ":/styles/style.qss");

    if (dialog._rowCount != 0 && dialog._columnCount != 0) {
        QTextTableFormat tableFormat;
        tableFormat.setBorder(1);
        tableFormat.setCellSpacing(5);
        tableFormat.setCellPadding(1);
        tableFormat.setBorderCollapse(true);
        tableFormat.setBorderStyle(QTextTableFormat::BorderStyle_Solid);
        _textEditor->textCursor().insertTable(dialog._rowCount, dialog._columnCount, tableFormat);
    }
}

void SummaryWindow::insertLink() {
    InsertLinkDialog dialog(this);
    common::openDialog(&dialog, ":/styles/style.qss");

    if (!dialog._title.isEmpty() && !dialog._link.isEmpty()) {
        QTextCharFormat charFormat;
        charFormat.setFontUnderline(true);
        charFormat.setForeground(QColor("blue"));
        charFormat.setAnchor(true);
        charFormat.setAnchorHref(dialog._link);
        charFormat.setToolTip(dialog._link);
        _textEditor->textCursor().insertText(dialog._title, charFormat);
    }
}

void SummaryWindow::copyFormatting() {
    _currentCopiedFormat = _textEditor->currentCharFormat();
    _currentCopiedFont = _textEditor->currentFont();
}

void SummaryWindow::pasteMatchFormat() {
    _textEditor->setCurrentCharFormat(_currentCopiedFormat);
    _textEditor->setCurrentFont(_currentCopiedFont);
}

void SummaryWindow::saveSummary() {
    if (!_labelTitle->text().isEmpty()) {
        queries::updateSummary(_labelTitle->text(), _textEditor->toHtml());
    }
}

void SummaryWindow::exportSummaryToHtml() {
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save File"), "/", tr("*.html"));
    std::ofstream htmlFile(filePath.toStdString());
    htmlFile << _textEditor->toHtml().toStdString();
    htmlFile.close();
    QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
}

void SummaryWindow::setBackColor() {
    QPoint bottom = _buttonEditorBackColor->rect().bottomLeft();
    QPoint globalPos = _buttonEditorBackColor->mapToGlobal(bottom);
    globalPos.setY(globalPos.y() + 2);

    ColorPickerWidget *widget = new ColorPickerWidget(this, Qt::white);
    widget->move(globalPos);
    common::openDialog(widget, ":/styles/colorpickerstyle.qss");

    if (!widget->colorSelected()) {
        return;
    }

    QString styleSheet = "background-color: " + widget->getCurrentColor().name();

    QTextFrame *currentFrame = _textEditor->textCursor().currentFrame();
    QTextFrameFormat currentFormat = currentFrame->frameFormat();
    // Apply only to code box in case cursor in code box
    if (currentFormat.border() == 1) {
        currentFormat.setBackground(QBrush(widget->getCurrentColor()));
        currentFrame->setFrameFormat(currentFormat);
        return;
    }

    // Apply to editor background
    _textEditor->setStyleSheet(styleSheet);
    _buttonEditorBackColor->setStyleSheet(styleSheet);
    if (widget->getCurrentColor() != Qt::white) {
        _buttonEditorBackColor->setIcon(QIcon(":/icons/background_fill_white.png"));
    } else {
        _buttonEditorBackColor->setIcon(QIcon(":/icons/background_fill_black.png"));
    }

    _buttonEditorBackColor->update();
}

void SummaryWindow::sentenceCase() {
    QTextCursor cursor = _textEditor->textCursor();
    if (cursor.selectedText().isEmpty()) {
        return;
    }

    QStringList tempList = cursor.selectedText().split(" ");
    QStringList newText;
    newText.append(tempList[0].at(0).toUpper() + tempList[0].mid(1).toLower());
    for (int i = 1; i < tempList.size(); i++) {
        newText.append(tempList[i].toLower());
    }

    cursor.removeSelectedText();
    cursor.insertText(newText.join(" "));
}

void SummaryWindow::upperCase() {
    QTextCursor cursor = _textEditor->textCursor();
    if (cursor.selectedText().isEmpty()) {
        return;
    }

    QStringList tempList = cursor.selectedText().split(" ");
    QStringList newText;
    for (const QString &word: tempList) {
        newText.append(word.toUpper());
    }

    cursor.removeSelectedText();
    cursor.insertText(newText.join(" "));
}

void SummaryWindow::lowerCase() {
    QTextCursor cursor = _textEditor->textCursor();
    if (cursor.selectedText().isEmpty()) {
        return;
    }

    QStringList tempList = cursor.selectedText().split(" ");
    QStringList newText;
    for (const QString &word: tempList) {
        newText.append(word.toLower());
    }

    cursor.removeSelectedText();
    cursor.insertText(newText.join(" "));
}

void SummaryWindow::capitalCase() {
    QTextCursor cursor = _textEditor->textCursor();
    if (cursor.selectedText().isEmpty()) {
        return;
    }

    QStringList tempList = cursor.selectedText().split(" ");
    QStringList newText;
    for (const QString &word: tempList) {
        if (word.size() > 1) {
            newText.append(word.at(0).toUpper() + word.mid(1).toLower());
        } else {
            newText.append(word.at(0).toLower());
        }
    }
    cursor.removeSelectedText();
    cursor.insertText(newText.join(" "));
}

void SummaryWindow::selectCurrentLine() {
    QTextCursor cursor = _textEditor->textCursor();
    cursor.movePosition(QTextCursor::StartOfLine);
    cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
    _textEditor->setTextCursor(cursor);
}

void SummaryWindow::duplicateCurrentLine() {
    QTextCursor cursor = _textEditor->textCursor();
    cursor.movePosition(QTextCursor::StartOfLine);
    cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
    QString currentLine = cursor.selectedText();
    cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::MoveAnchor);
    cursor.insertText("\n");
    cursor.insertText(currentLine);
}

void SummaryWindow::deleteCurrentLine() {
    QTextCursor cursor = _textEditor->textCursor();
    cursor.movePosition(QTextCursor::StartOfLine);
    cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
    cursor.deletePreviousChar();
}

void SummaryWindow::zoomIn() {
    _textEditor->zoomIn(5);
}

void SummaryWindow::zoomOut() {
    _textEditor->zoomOut(5);
}

void SummaryWindow::insertNormalDate() {
    QDate currDate = QDate::currentDate();
    QLocale locale;
    QCalendar cal;
    QString dayName = locale.dayName(currDate.dayOfWeek(), QLocale::LongFormat);
    quint32 dayNum = currDate.day();
    quint32 yearNum = currDate.year();
    QString monthName = cal.monthName(locale, currDate.month(), yearNum);
    QString dateStr = dayName + ", " + monthName + " " + QString::number(dayNum) + ", " + QString::number(yearNum);
    _textEditor->textCursor().insertText(dateStr);
}

void SummaryWindow::insertShortDate() {
    QDate currDate = QDate::currentDate();
    QString day = QString::number(currDate.day());
    QString month = QString::number(currDate.month());
    QString year = QString::number(currDate.year());
    QString dateStr = day + "/" + month + "/" + year;
    _textEditor->textCursor().insertText(dateStr);
}

void SummaryWindow::addTableRow() {
    QTextCursor cursor = _textEditor->textCursor();
    QTextTable *currTable = cursor.currentTable();
    if (currTable != nullptr) {
        currTable->insertRows(currTable->cellAt(cursor).row(), 1);
    }
}

void SummaryWindow::addTableColumn() {
    QTextCursor cursor = _textEditor->textCursor();
    QTextTable *currTable = cursor.currentTable();
    if (currTable != nullptr) {
        currTable->insertColumns(currTable->cellAt(cursor).column(), 1);
    }
}

// Find functionality specific
void SummaryWindow::toggleFindWidget(bool visible) {
    _frameFind->setVisible(visible);
    highlightText();

    if (visible) {
        _textFind->setFocus();
    }
}

void SummaryWindow::highlightText() {
    if (_frameFind->isVisible()) {
        _textHighlighter.setWordPattern(_textFind->text());
    } else {
        _textHighlighter.setWordPattern("");
    }

    if (sender() == _textEditor) {
        _textEditor->blockSignals(true);
        _textHighlighter.customRehighlight();
        _textEditor->blockSignals(false);
    } else {
        _textHighlighter.customRehighlight();
    }
}

void SummaryWindow::textEditRefreshHighlighter(int cursorIndex) {
    if (cursorIndex >= 0) {
        QTextCursor currentCursor = _textEditor->textCursor();
        currentCursor.setPosition(cursorIndex);
        _textEditor->setTextCursor(currentCursor);
        _textEditor->ensureCursorVisible();
    }
}

void SummaryWindow::findText(const QString &arg1) {
    highlightText();
    if (arg1.isEmpty()) {
        _labelFindMatchNum->setText("");
        return;
    }
    QString labelText = QString("Found %1 _matches").arg(QString::number(_textHighlighter.matchNumber()));
    _labelFindMatchNum->setText(labelText);
    QFont customFont;
    customFont.setItalic(true);
    customFont.setPointSize(12);
    customFont.setFamily("Segoe UI");
    _labelFindMatchNum->setFont(customFont);
    _labelFindMatchNum->setStyleSheet("color: grey");
}

void SummaryWindow::findNext() {
    int cursorIndex = _textHighlighter.setNextMatchStateActive();
    if (_textHighlighter.matchIndex() == _textHighlighter.matchNumber() - 1) {
        _textHighlighter.setMatchIndex(-1);
    }
    textEditRefreshHighlighter(cursorIndex);
}

void SummaryWindow::findPrevious() {
    int cursorIndex = _textHighlighter.setPrevMatchStateActive();
    if (_textHighlighter.matchIndex() == 0) {
        _textHighlighter.setMatchIndex(_textHighlighter.matchNumber() - 1);
    }
    textEditRefreshHighlighter(cursorIndex);
}

void SummaryWindow::increaseFontSize() {
    _spinBoxFontSize->setValue(_spinBoxFontSize->value() + 1);
}

void SummaryWindow::decreaseFontSize() {
    _spinBoxFontSize->setValue(_spinBoxFontSize->value() - 1);
}

void SummaryWindow::increaseIndent() {
    _textEditor->changeListIndentation(1);
}

void SummaryWindow::decreaseIndent() {
    _textEditor->changeListIndentation(-1);
}
