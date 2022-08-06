#include "include/common.h"
#include "include/queries.h"
#include "include/texteditor.h"
#include "include/summarywindow.h"
#include "include/insertlinkdialog.h"
#include "include/booksearchwidget.h"
#include "include/inserttabledialog.h"
#include "include/colorpickerwidget.h"

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

SummaryWindow::SummaryWindow(QWidget* parent) : QMainWindow(parent)
{
	queries::connectToDatabase();

	setupInterface();
	setupConnections();
	setupTabOrder();
}

void SummaryWindow::setupInterface()
{
	// Window
	setWindowTitle("Summaries");
	resize(1035, 600);

	// Widgets
	m_centralWidget = new QWidget(this);
	m_horLayMain = new QHBoxLayout(m_centralWidget);

	m_mainSplitterPane = new QSplitter(m_centralWidget);
	m_mainSplitterPane->setStyleSheet(QString("QSplitter {border: 0px;}"));
	m_mainSplitterPane->setLineWidth(0);
	m_mainSplitterPane->setOrientation(Qt::Horizontal);
	m_mainSplitterPane->setHandleWidth(0);

	m_bookSearchWidget = new BookSearchWidget(this);
	m_bookSearchWidget->setMainLayoutMargin(-1, 41, -1, -1);
	m_bookSearchWidget->setMainLayoutSpacing(15);
	m_mainSplitterPane->addWidget(m_bookSearchWidget);

	m_frameEditor = new QFrame(m_mainSplitterPane);
	m_frameEditor->setStyleSheet(QString("QFrame { border: 0px; }"));

	m_vertLayEditor = new QVBoxLayout(m_frameEditor);
	m_vertLayEditor->setContentsMargins(0, -1, 0, -1);

	m_labelTitle = new QLabel(m_frameEditor);
	m_labelTitle->setMinimumSize(QSize(0, 19));

	m_frameEditorToolBox = new QFrame(m_frameEditor);
	QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);
	sizePolicy.setHeightForWidth(m_frameEditorToolBox->sizePolicy().hasHeightForWidth());

	m_frameEditorToolBox->setSizePolicy(sizePolicy);
	m_frameEditorToolBox->setFrameShape(QFrame::StyledPanel);
	m_frameEditorToolBox->setFrameShadow(QFrame::Raised);

	m_horLayToolBox = new QHBoxLayout(m_frameEditorToolBox);
	m_horLayToolBox->setContentsMargins(0, 7, 0, -1);

	m_fontComboBox = new QFontComboBox(m_frameEditorToolBox);
	m_fontComboBox->setMinimumSize(QSize(0, 25));
	m_fontComboBox->setStyleSheet(QString("QFontComboBox{ border: 1px solid grey; }"));
	m_fontComboBox->setEditable(false);

	m_spinBoxFontSize = new QSpinBox(m_frameEditorToolBox);
	m_spinBoxFontSize->setMinimumSize(QSize(0, 25));
	m_spinBoxFontSize->setMaximumSize(QSize(40, 999999));
	m_spinBoxFontSize->setMinimum(1);
	m_spinBoxFontSize->setMaximum(70);
	m_spinBoxFontSize->setValue(12);

	m_buttonEditorFontColor = new QToolButton(m_frameEditorToolBox);
	m_buttonEditorFontColor->setMinimumSize(QSize(30, 25));
	m_buttonEditorFontColor->setMaximumSize(QSize(30, 16777215));
	m_buttonEditorFontColor->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonEditorFontColor->setFocusPolicy(Qt::NoFocus);
	m_buttonEditorFontColor->setStyleSheet(QString::fromUtf8(""));
	m_buttonEditorFontColor->setIcon(QIcon(":/icons/font_color_icon.png"));

	m_buttonEditorBackColor = new QToolButton(m_frameEditorToolBox);
	m_buttonEditorBackColor->setMinimumSize(QSize(30, 25));
	m_buttonEditorBackColor->setMaximumSize(QSize(30, 16777215));
	m_buttonEditorBackColor->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonEditorBackColor->setIcon(QIcon(":/icons/background_fill_white.png"));

	m_lineVert1 = new QFrame(m_frameEditorToolBox);
	m_lineVert1->setStyleSheet(QString("color: grey; background-color: grey;"));
	m_lineVert1->setFrameShape(QFrame::VLine);
	m_lineVert1->setFrameShadow(QFrame::Sunken);

	m_buttonBold = new QToolButton(m_frameEditorToolBox);
	m_buttonBold->setMinimumSize(QSize(30, 25));
	m_buttonBold->setMaximumSize(QSize(30, 16777215));
	m_buttonBold->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonBold->setFocusPolicy(Qt::NoFocus);
	m_buttonBold->setIcon(QIcon(":/icons/bold_icon.png"));

	m_buttonItalic = new QToolButton(m_frameEditorToolBox);
	m_buttonItalic->setMinimumSize(QSize(30, 25));
	m_buttonItalic->setMaximumSize(QSize(30, 16777215));
	m_buttonItalic->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonItalic->setFocusPolicy(Qt::NoFocus);
	m_buttonItalic->setIcon(QIcon(":/icons/italic_icon.png"));

	m_buttonUnderline = new QToolButton(m_frameEditorToolBox);
	m_buttonUnderline->setMinimumSize(QSize(30, 25));
	m_buttonUnderline->setMaximumSize(QSize(30, 16777215));
	m_buttonUnderline->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonUnderline->setFocusPolicy(Qt::NoFocus);
	m_buttonUnderline->setIcon(QIcon(":/icons/underline_icon.png"));

	m_buttonThrough = new QToolButton(m_frameEditorToolBox);
	m_buttonThrough->setMinimumSize(QSize(30, 25));
	m_buttonThrough->setMaximumSize(QSize(30, 16777215));
	m_buttonThrough->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonThrough->setFocusPolicy(Qt::NoFocus);
	m_buttonThrough->setIcon(QIcon(":/icons/strikethrough_icon.png"));

	m_buttonHighlight = new QToolButton(m_frameEditorToolBox);
	m_buttonHighlight->setMinimumSize(QSize(30, 25));
	m_buttonHighlight->setMaximumSize(QSize(30, 16777215));
	m_buttonHighlight->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonHighlight->setFocusPolicy(Qt::NoFocus);
	m_buttonHighlight->setIcon(QIcon(":/icons/highlight_icon.png"));

	m_buttonCodeBlock = new QToolButton(m_frameEditorToolBox);
	m_buttonCodeBlock->setMinimumSize(QSize(30, 25));
	m_buttonCodeBlock->setMaximumSize(QSize(30, 16777215));
	m_buttonCodeBlock->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonCodeBlock->setFocusPolicy(Qt::NoFocus);
	m_buttonCodeBlock->setIcon(QIcon(":/icons/code_icon.png"));

	m_lineVert2 = new QFrame(m_frameEditorToolBox);
	m_lineVert2->setStyleSheet(QString::fromUtf8("color: grey; background-color: grey;"));
	m_lineVert2->setFrameShadow(QFrame::Plain);
	m_lineVert2->setLineWidth(0);
	m_lineVert2->setMidLineWidth(0);
	m_lineVert2->setFrameShape(QFrame::VLine);

	m_buttonBulletedList = new QToolButton(m_frameEditorToolBox);
	m_buttonBulletedList->setMinimumSize(QSize(30, 25));
	m_buttonBulletedList->setMaximumSize(QSize(30, 16777215));
	m_buttonBulletedList->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonBulletedList->setFocusPolicy(Qt::NoFocus);
	m_buttonBulletedList->setIcon(QIcon(":/icons/bullets_icon.png"));

	m_buttonOrderedList = new QToolButton(m_frameEditorToolBox);
	m_buttonOrderedList->setMinimumSize(QSize(30, 25));
	m_buttonOrderedList->setMaximumSize(QSize(30, 16777215));
	m_buttonOrderedList->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonOrderedList->setFocusPolicy(Qt::NoFocus);
	m_buttonOrderedList->setIcon(QIcon(":/icons/numbers_icon.png"));

	m_comboBoxAlignment = new QComboBox(m_frameEditorToolBox);
	QIcon iconAlignLeft(":/icons/alignleft_icon.png");
	m_comboBoxAlignment->addItem(iconAlignLeft, QString("Left"));
	QIcon iconAlignCenter(":/icons/aligncenter_icon.png");
	m_comboBoxAlignment->addItem(iconAlignCenter, QString("Center"));
	QIcon iconAlignRight(":/icons/alignright_icon.png");
	m_comboBoxAlignment->addItem(iconAlignRight, QString("Right"));
	QIcon iconAlignJustify(":/icons/justifytext_icon.png");
	m_comboBoxAlignment->addItem(iconAlignJustify, QString("Justify"));
	m_comboBoxAlignment->setMinimumSize(QSize(0, 25));
	m_comboBoxAlignment->setMaximumSize(QSize(40, 16777215));
	m_comboBoxAlignment->setFocusPolicy(Qt::NoFocus);
	m_comboBoxAlignment->setSizeAdjustPolicy(QComboBox::AdjustToContents);

	m_lineVert3 = new QFrame(m_frameEditorToolBox);
	m_lineVert3->setStyleSheet(QString("color: grey; background-color: grey;"));
	m_lineVert3->setFrameShadow(QFrame::Sunken);
	m_lineVert3->setLineWidth(1);
	m_lineVert3->setFrameShape(QFrame::VLine);

	m_buttonInsertTable = new QToolButton(m_frameEditorToolBox);
	m_buttonInsertTable->setMinimumSize(QSize(30, 25));
	m_buttonInsertTable->setMaximumSize(QSize(30, 16777215));
	m_buttonInsertTable->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonInsertTable->setFocusPolicy(Qt::NoFocus);
	m_buttonInsertTable->setIcon(QIcon(":/icons/table_icon.png"));

	m_buttonInsertImage = new QToolButton(m_frameEditorToolBox);
	m_buttonInsertImage->setMinimumSize(QSize(30, 25));
	m_buttonInsertImage->setMaximumSize(QSize(30, 25));
	m_buttonInsertImage->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonInsertImage->setFocusPolicy(Qt::NoFocus);
	m_buttonInsertImage->setIcon(QIcon(":/icons/image_icon.png"));

	m_buttonInsertLine = new QToolButton(m_frameEditorToolBox);
	m_buttonInsertLine->setMinimumSize(QSize(30, 25));
	m_buttonInsertLine->setMaximumSize(QSize(30, 25));
	m_buttonInsertLine->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonInsertLine->setFocusPolicy(Qt::NoFocus);
	m_buttonInsertLine->setIcon(QIcon(":/icons/hr_icon.png"));

	m_buttonInsertLink = new QToolButton(m_frameEditorToolBox);
	m_buttonInsertLink->setMinimumSize(QSize(30, 25));
	m_buttonInsertLink->setMaximumSize(QSize(30, 16777215));
	m_buttonInsertLink->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonInsertLink->setFocusPolicy(Qt::NoFocus);
	m_buttonInsertLink->setIcon(QIcon(":/icons/link_icon.png"));

	m_horSpacerTopToolBar = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

	m_horLayToolBox->addWidget(m_fontComboBox);
	m_horLayToolBox->addWidget(m_spinBoxFontSize);
	m_horLayToolBox->addWidget(m_buttonEditorFontColor);
	m_horLayToolBox->addWidget(m_buttonEditorBackColor);
	m_horLayToolBox->addWidget(m_lineVert1);
	m_horLayToolBox->addWidget(m_buttonBold);
	m_horLayToolBox->addWidget(m_buttonItalic);
	m_horLayToolBox->addWidget(m_buttonUnderline);
	m_horLayToolBox->addWidget(m_buttonThrough);
	m_horLayToolBox->addWidget(m_buttonHighlight);
	m_horLayToolBox->addWidget(m_buttonCodeBlock);
	m_horLayToolBox->addWidget(m_lineVert2);
	m_horLayToolBox->addWidget(m_buttonBulletedList);
	m_horLayToolBox->addWidget(m_buttonOrderedList);
	m_horLayToolBox->addWidget(m_comboBoxAlignment);
	m_horLayToolBox->addWidget(m_lineVert3);
	m_horLayToolBox->addWidget(m_buttonInsertTable);
	m_horLayToolBox->addWidget(m_buttonInsertImage);
	m_horLayToolBox->addWidget(m_buttonInsertLine);
	m_horLayToolBox->addWidget(m_buttonInsertLink);
	m_horLayToolBox->addItem(m_horSpacerTopToolBar);

	m_textEditor = new TextEditor(m_frameEditor);
	QFont fontTextEditor;
	fontTextEditor.setPointSize(12);
	m_textEditor->setFont(fontTextEditor);
	m_textEditor->viewport()->setProperty("cursor", QVariant(QCursor(Qt::IBeamCursor)));
	m_textEditor->setStyleSheet(common::openSheet(":/styles/textEditorStyle.qss"));
	m_textEditor->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	m_textEditor->setAutoFormatting(QTextEdit::AutoAll);
	m_textEditor->setUndoRedoEnabled(true);
	m_textEditor->setLineWrapMode(QTextEdit::WidgetWidth);
	m_textEditor->setReadOnly(false);
	m_textEditor->setTextInteractionFlags(
			Qt::LinksAccessibleByMouse | Qt::TextEditable | Qt::TextEditorInteraction | Qt::TextSelectableByKeyboard |
					Qt::TextSelectableByMouse);
	m_textEditor->setOpenExternalLinks(true);
	m_textEditor->setOpenLinks(true);

	m_frameFind = new QFrame(m_frameEditor);
	m_frameFind->setStyleSheet(QString::fromUtf8("QFrame { \n"
												 "	border: none;\n"
												 "	padding: 0px;\n"
												 "	margin: 0px;\n"
												 "}"));
	m_frameFind->setFrameShape(QFrame::StyledPanel);
	m_frameFind->setFrameShadow(QFrame::Raised);
	m_frameFind->setHidden(true);
	m_horLayFind = new QHBoxLayout(m_frameFind);
	m_horLayFind->setContentsMargins(0, 1, 0, 1);

	m_textFind = new QLineEdit(m_frameFind);
	m_textFind->setMinimumSize(QSize(300, 25));
	m_textFind->setMaximumSize(QSize(300, 25));
	m_textFind->setClearButtonEnabled(true);

	m_buttonNext = new QPushButton("Next", m_frameFind);
	m_buttonNext->setMinimumSize(QSize(25, 25));
	m_buttonNext->setMaximumSize(QSize(16777215, 25));
	m_buttonNext->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonNext->setStyleSheet(QString("padding: 5px;"));

	m_buttonPrevious = new QPushButton("Previous", m_frameFind);
	m_buttonPrevious->setMinimumSize(QSize(25, 25));
	m_buttonPrevious->setMaximumSize(QSize(16777215, 25));
	m_buttonPrevious->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonPrevious->setStyleSheet(QString("padding: 5px;"));

	m_buttonCloseFind = new QPushButton(m_frameFind);
	m_buttonCloseFind->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonCloseFind->setStyleSheet(QString::fromUtf8("border: none;"));
	m_buttonCloseFind->setIcon(QIcon(":/icons/close_icon.png"));
	m_buttonCloseFind->setFlat(true);

	m_labelFindMatchNum = new QLabel(m_frameFind);

	m_horSpacerFind = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

	m_horLayFind->addWidget(m_textFind);
	m_horLayFind->addWidget(m_buttonNext);
	m_horLayFind->addWidget(m_buttonPrevious);
	m_horLayFind->addWidget(m_buttonCloseFind);
	m_horLayFind->addWidget(m_labelFindMatchNum);
	m_horLayFind->addItem(m_horSpacerFind);

	m_vertLayEditor->addWidget(m_labelTitle);
	m_vertLayEditor->addWidget(m_frameEditorToolBox);
	m_vertLayEditor->addWidget(m_textEditor);
	m_vertLayEditor->addWidget(m_frameFind);

	m_mainSplitterPane->addWidget(m_frameEditor);
	m_horLayMain->addWidget(m_mainSplitterPane);

	m_frameRightToolBar = new QFrame(m_centralWidget);
	m_frameRightToolBar->setMinimumSize(QSize(32, 0));
	m_frameRightToolBar->setStyleSheet(QString("QFrame { border: 0px; }"));
	m_frameRightToolBar->setLineWidth(0);
	m_vertLayToolBar = new QVBoxLayout(m_frameRightToolBar);
	m_vertLayToolBar->setContentsMargins(1, 82, 1, -1);

	m_buttonUndo = new QToolButton(m_frameRightToolBar);
	m_buttonUndo->setMinimumSize(QSize(30, 25));
	m_buttonUndo->setMaximumSize(QSize(30, 16777215));
	m_buttonUndo->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonUndo->setFocusPolicy(Qt::NoFocus);
	m_buttonUndo->setIcon(QIcon(":/icons/undo_icon.png"));

	m_buttonRedo = new QToolButton(m_frameRightToolBar);
	m_buttonRedo->setMinimumSize(QSize(30, 25));
	m_buttonRedo->setMaximumSize(QSize(30, 16777215));
	m_buttonRedo->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonRedo->setFocusPolicy(Qt::NoFocus);
	m_buttonRedo->setIcon(QIcon(":/icons/redo.png"));

	m_buttonClearAll = new QToolButton(m_frameRightToolBar);
	m_buttonClearAll->setMinimumSize(QSize(30, 25));
	m_buttonClearAll->setMaximumSize(QSize(30, 16777215));
	m_buttonClearAll->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonClearAll->setFocusPolicy(Qt::NoFocus);
	m_buttonClearAll->setIcon(QIcon(":/icons/cleartext_icon.png"));

	m_horLine1 = new QFrame(m_frameRightToolBar);
	m_horLine1->setStyleSheet(QString::fromUtf8("color: grey;\n"
												"background-color: grey;"));
	m_horLine1->setFrameShape(QFrame::HLine);
	m_horLine1->setFrameShadow(QFrame::Sunken);

	m_buttonSuperscript = new QToolButton(m_frameRightToolBar);
	m_buttonSuperscript->setMinimumSize(QSize(30, 25));
	m_buttonSuperscript->setMaximumSize(QSize(30, 25));
	m_buttonSuperscript->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonSuperscript->setFocusPolicy(Qt::NoFocus);
	m_buttonSuperscript->setIcon(QIcon(":/icons/superscript_icon.png"));

	m_buttonSubscript = new QToolButton(m_frameRightToolBar);
	m_buttonSubscript->setMinimumSize(QSize(30, 25));
	m_buttonSubscript->setMaximumSize(QSize(30, 25));
	m_buttonSubscript->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonSubscript->setFocusPolicy(Qt::NoFocus);
	m_buttonSubscript->setIcon(QIcon(":/icons/subscript_icon.png"));

	m_horLine2 = new QFrame(m_frameRightToolBar);
	m_horLine2->setStyleSheet(QString::fromUtf8("color: grey; background-color: grey;"));
	m_horLine2->setFrameShape(QFrame::HLine);
	m_horLine2->setFrameShadow(QFrame::Sunken);

	m_buttonIncFontSize = new QToolButton(m_frameRightToolBar);
	m_buttonIncFontSize->setMinimumSize(QSize(30, 25));
	m_buttonIncFontSize->setMaximumSize(QSize(30, 25));
	m_buttonIncFontSize->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonIncFontSize->setFocusPolicy(Qt::NoFocus);
	m_buttonIncFontSize->setIcon(QIcon(":/icons/increasefont_icon.png"));

	m_buttonDecFontSize = new QToolButton(m_frameRightToolBar);
	m_buttonDecFontSize->setMinimumSize(QSize(30, 25));
	m_buttonDecFontSize->setMaximumSize(QSize(30, 25));
	m_buttonDecFontSize->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonDecFontSize->setFocusPolicy(Qt::NoFocus);
	m_buttonDecFontSize->setIcon(QIcon(":/icons/decreasefont_icon.png"));

	m_horLine3 = new QFrame(m_frameRightToolBar);
	m_horLine3->setStyleSheet(QString("color: grey; background-color: grey;"));
	m_horLine3->setFrameShape(QFrame::HLine);
	m_horLine3->setFrameShadow(QFrame::Sunken);

	m_buttonIncIndent = new QToolButton(m_frameRightToolBar);
	m_buttonIncIndent->setMinimumSize(QSize(30, 25));
	m_buttonIncIndent->setMaximumSize(QSize(30, 25));
	m_buttonIncIndent->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonIncIndent->setFocusPolicy(Qt::NoFocus);
	m_buttonIncIndent->setIcon(QIcon(":/icons/increaseindent_icon.png"));

	m_buttonDecIndent = new QToolButton(m_frameRightToolBar);
	m_buttonDecIndent->setMinimumSize(QSize(30, 25));
	m_buttonDecIndent->setMaximumSize(QSize(30, 25));
	m_buttonDecIndent->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonDecIndent->setFocusPolicy(Qt::NoFocus);
	m_buttonDecIndent->setIcon(QIcon(":/icons/decreaseindent_icon.png"));

	m_horLine4 = new QFrame(m_frameRightToolBar);
	m_horLine4->setStyleSheet(QString("color: grey; background-color: grey;"));
	m_horLine4->setFrameShape(QFrame::HLine);
	m_horLine4->setFrameShadow(QFrame::Sunken);

	m_vertLayToolBar->addWidget(m_buttonUndo);
	m_vertLayToolBar->addWidget(m_buttonRedo);
	m_vertLayToolBar->addWidget(m_buttonClearAll);
	m_vertLayToolBar->addWidget(m_horLine1);
	m_vertLayToolBar->addWidget(m_buttonSuperscript);
	m_vertLayToolBar->addWidget(m_buttonSubscript);
	m_vertLayToolBar->addWidget(m_horLine2);
	m_vertLayToolBar->addWidget(m_buttonIncFontSize);
	m_vertLayToolBar->addWidget(m_buttonDecFontSize);
	m_vertLayToolBar->addWidget(m_horLine3);
	m_vertLayToolBar->addWidget(m_buttonIncIndent);
	m_vertLayToolBar->addWidget(m_buttonDecIndent);
	m_vertLayToolBar->addWidget(m_horLine4);
	m_vertSpacerRightToolBar = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
	m_vertLayToolBar->addItem(m_vertSpacerRightToolBar);
	m_horLayMain->addWidget(m_frameRightToolBar);

	// Actions
	setupActions();

	// MenuBar
	m_menuBar = new QMenuBar(this);
	m_menuBar->setGeometry(QRect(0, 0, 1035, 21));
	setMenuBar(m_menuBar);

	// StatusBar
	m_statusBar = new QStatusBar(this);
	setStatusBar(m_statusBar);

	// Menus
	setupMenus();

	// Central Widget
	setCentralWidget(m_centralWidget);
}

void SummaryWindow::setupActions()
{
	// File Menu
	m_actionSaveSummary = new QAction("Save Summary", this);
	m_actionSaveSummary->setIcon(QIcon(":/icons/save_icon.png"));
	m_actionPrintToPDF = new QAction("Print to PDF", this);
	m_actionPrintToPDF->setIcon(QIcon(":/icons/pdf_icon.png"));
	m_actionExportHtml = new QAction("Export to HTML", this);
	m_actionExportHtml->setIcon(QIcon(":/icons/html_icon.png"));
	m_actionClose = new QAction("Close", this);

	// Edit Menu
	m_actionUndo = new QAction("Undo", this);
	m_actionUndo->setIcon(QIcon(":/icons/undo_icon.png"));
	m_actionRedo = new QAction("Redo", this);
	m_actionRedo->setIcon(QIcon(":/icons/redo.png"));
	m_actionCut = new QAction("Cut", this);
	m_actionCut->setIcon(QIcon(":/icons/cut_icon.png"));
	m_actionCopy = new QAction("Copy", this);
	m_actionCopy->setIcon(QIcon(":/icons/copy_icon.png"));
	m_actionCopyFormatting = new QAction("Copy Formatting", this);
	m_actionPaste = new QAction("Paste", this);
	m_actionPaste->setIcon(QIcon(":/icons/paste_icon.png"));
	m_actionPasteMatchFormat = new QAction("Paste and Match Format", this);
	m_actionSelectAll = new QAction("Select All", this);
	m_actionClearAll = new QAction("Clear All", this);
	m_actionClearAll->setIcon(QIcon(":/icons/cleartext_icon.png"));
	m_actionSelectCurrentLine = new QAction("Select Current Line", this);
	m_actionDuplicateCurrentLine = new QAction("Duplicate Current Line", this);
	m_actionDeleteCurrentLine = new QAction("Delete Current Line", this);
	m_actionFind = new QAction("Find", this);

	// View Menu
	m_actionFullscreen = new QAction("Fullscreen", this);
	m_actionFullscreen->setIcon(QIcon(":/icons/fullscreen_icon.png"));
	m_actionMaximize = new QAction("Maximize", this);
	m_actionMinimize = new QAction("Minimize", this);
	m_actionMinimize->setIcon(QIcon(":/icons/minimize_icon.png"));
	m_actionZoomIn = new QAction("Zoom In", this);
	m_actionZoomIn->setIcon(QIcon(":/icons/zoom_in_icon.png"));
	m_actionZoomOut = new QAction("Zoom Out", this);
	m_actionZoomOut->setIcon(QIcon(":/icons/zoom_out_icon.png"));
	m_actionHideSearchBar = new QAction("Hide Search Bar", this);
	m_actionHideListWidget = new QAction("Hide List Widget", this);
	m_actionHideLeftPane = new QAction("Hide Left Pane", this);
	m_actionHideTopToolbar = new QAction("Hide Top Toolbar", this);
	m_actionHideRightToolbar = new QAction("Hide Right Toolbar", this);
	m_actionHideRightPane = new QAction("Hide Right Pane", this);

	// Insert Menu
	m_actionInsertLine = new QAction("Line", this);
	m_actionInsertLine->setIcon(QIcon(":/icons/hr_icon.png"));
	m_actionInsertLink = new QAction("Link", this);
	m_actionInsertLink->setIcon(QIcon(":/icons/link_icon.png"));
	m_actionInsertImage = new QAction("Image", this);
	m_actionInsertImage->setIcon(QIcon(":/icons/image_icon.png"));
	m_actionInsertTable = new QAction("Table", this);
	m_actionInsertTable->setIcon(QIcon(":/icons/table_icon.png"));
	m_actionTableRow = new QAction("Table Row", this);
	m_actionTableColumn = new QAction("Table Column", this);
	m_actionDateNormal = new QAction("Normal Date", this);
	m_actionDateShort = new QAction("Short Date", this);

	// Format Menu
	m_actionBold = new QAction("Bold", this);
	m_actionBold->setIcon(QIcon(":/icons/bold_icon.png"));
	m_actionItalic = new QAction("Italic", this);
	m_actionItalic->setIcon(QIcon(":/icons/italic_icon.png"));
	m_actionUnderline = new QAction("Underline", this);
	m_actionUnderline->setIcon(QIcon(":/icons/underline_icon.png"));
	m_actionThrough = new QAction("Through", this);
	m_actionThrough->setIcon(QIcon(":/icons/strikethrough_icon.png"));
	m_actionHighlight = new QAction("Highlight", this);
	m_actionHighlight->setIcon(QIcon(":/icons/highlight_icon.png"));
	m_actionCodeBlock = new QAction("Code", this);
	m_actionCodeBlock->setIcon(QIcon(":/icons/code_icon.png"));
	m_actionBulletedList = new QAction("Bulleted List", this);
	m_actionBulletedList->setIcon(QIcon(":/icons/bullets_icon.png"));
	m_actionOrderedList = new QAction("Ordered List", this);
	m_actionOrderedList->setIcon(QIcon(":/icons/numbers_icon.png"));
	m_actionIncFontSize = new QAction("+ Font Size", this);
	m_actionIncFontSize->setIcon(QIcon(":/icons/increasefont_icon.png"));
	m_actionDecFontSize = new QAction("- Font Size", this);
	m_actionDecFontSize->setIcon(QIcon(":/icons/decreasefont_icon.png"));
	m_actionIncIndent = new QAction("+ Indent", this);
	m_actionIncIndent->setIcon(QIcon(":/icons/increaseindent_icon.png"));
	m_actionDecIndent = new QAction("- Indent", this);
	m_actionDecIndent->setIcon(QIcon(":/icons/decreaseindent_icon.png"));
	m_actionSuperscript = new QAction("Superscript", this);
	m_actionSuperscript->setIcon(QIcon(":/icons/superscript_icon.png"));
	m_actionSubscript = new QAction("Subscript", this);
	m_actionSubscript->setIcon(QIcon(":/icons/subscript_icon.png"));
	m_actionResetFormat = new QAction("Reset Format", this);
	m_actionResetFormat->setIcon(QIcon(":/icons/restore_icon.png"));
	m_actionSentenceCase = new QAction("Sentence Case", this);
	m_actionSentenceCase->setIcon(QIcon(":/icons/sentencecase_icon.png"));
	m_actionUpperCase = new QAction("Uppercase", this);
	m_actionUpperCase->setIcon(QIcon(":/icons/uppercase_icon.png"));
	m_actionLowerCase = new QAction("Lowercase", this);
	m_actionLowerCase->setIcon(QIcon(":/icons/lowercase_icon.png"));
	m_actionCapitalCase = new QAction("Capitalcase", this);

	// Search Menu
	m_actionSearchText = new QAction("Search Text", this);
	m_actionSearchText->setIcon(QIcon(":/icons/searchbar_icon.png"));
	m_actionClearSearch = new QAction("Clear Search", this);
	m_actionClearSearch->setIcon(QIcon(":/icons/clear_icon.png"));
}

void SummaryWindow::setupMenus()
{
	m_menuFile = m_menuBar->addMenu(tr("&File"));
	m_menuEdit = m_menuBar->addMenu(tr("&Edit"));
	m_menuView = m_menuBar->addMenu(tr("&View"));
	m_menuInsert = m_menuBar->addMenu(tr("&Insert"));
	m_menuFormat = m_menuBar->addMenu(tr("&Format"));
	m_menuSearch = m_menuBar->addMenu(tr("&Search"));

	m_menuDateTime = new QMenu(tr("Date"), m_menuInsert);
	m_menuChangeCase = new QMenu(tr("Change Case"), m_menuFormat);

	// Add Actions to Menus
	m_menuBar->addAction(m_menuFile->menuAction());
	m_menuBar->addAction(m_menuEdit->menuAction());
	m_menuBar->addAction(m_menuView->menuAction());
	m_menuBar->addAction(m_menuInsert->menuAction());
	m_menuBar->addAction(m_menuFormat->menuAction());
	m_menuBar->addAction(m_menuSearch->menuAction());
	m_menuFile->addAction(m_actionSaveSummary);
	m_menuFile->addAction(m_actionPrintToPDF);
	m_menuFile->addAction(m_actionExportHtml);
	m_menuFile->addSeparator();
	m_menuFile->addAction(m_actionClose);
	m_menuEdit->addAction(m_actionUndo);
	m_menuEdit->addAction(m_actionRedo);
	m_menuEdit->addSeparator();
	m_menuEdit->addAction(m_actionCut);
	m_menuEdit->addAction(m_actionCopy);
	m_menuEdit->addAction(m_actionCopyFormatting);
	m_menuEdit->addSeparator();
	m_menuEdit->addAction(m_actionPaste);
	m_menuEdit->addAction(m_actionPasteMatchFormat);
	m_menuEdit->addSeparator();
	m_menuEdit->addAction(m_actionSelectAll);
	m_menuEdit->addAction(m_actionClearAll);
	m_menuEdit->addSeparator();
	m_menuEdit->addAction(m_actionSelectCurrentLine);
	m_menuEdit->addAction(m_actionDuplicateCurrentLine);
	m_menuEdit->addAction(m_actionDeleteCurrentLine);
	m_menuEdit->addSeparator();
	m_menuEdit->addAction(m_actionFind);
	m_menuView->addAction(m_actionFullscreen);
	m_menuView->addAction(m_actionMaximize);
	m_menuView->addAction(m_actionMinimize);
	m_menuView->addSeparator();
	m_menuView->addAction(m_actionZoomIn);
	m_menuView->addAction(m_actionZoomOut);
	m_menuView->addSeparator();
	m_menuView->addAction(m_actionHideSearchBar);
	m_menuView->addAction(m_actionHideLeftPane);
	m_menuView->addSeparator();
	m_menuView->addAction(m_actionHideTopToolbar);
	m_menuView->addAction(m_actionHideRightToolbar);
	m_menuView->addAction(m_actionHideRightPane);
	m_menuInsert->addAction(m_actionInsertLine);
	m_menuInsert->addAction(m_actionInsertLink);
	m_menuInsert->addAction(m_actionInsertImage);
	m_menuInsert->addAction(m_actionInsertTable);
	m_menuInsert->addSeparator();
	m_menuInsert->addAction(m_actionTableRow);
	m_menuInsert->addAction(m_actionTableColumn);
	m_menuInsert->addSeparator();
	m_menuInsert->addAction(m_menuDateTime->menuAction());
	m_menuDateTime->addAction(m_actionDateShort);
	m_menuDateTime->addAction(m_actionDateNormal);
	m_menuFormat->addAction(m_actionBold);
	m_menuFormat->addAction(m_actionItalic);
	m_menuFormat->addAction(m_actionUnderline);
	m_menuFormat->addAction(m_actionThrough);
	m_menuFormat->addAction(m_actionHighlight);
	m_menuFormat->addAction(m_actionCodeBlock);
	m_menuFormat->addSeparator();
	m_menuFormat->addAction(m_actionBulletedList);
	m_menuFormat->addAction(m_actionOrderedList);
	m_menuFormat->addSeparator();
	m_menuFormat->addAction(m_actionIncFontSize);
	m_menuFormat->addAction(m_actionDecFontSize);
	m_menuFormat->addSeparator();
	m_menuFormat->addAction(m_actionIncIndent);
	m_menuFormat->addAction(m_actionDecIndent);
	m_menuFormat->addSeparator();
	m_menuFormat->addAction(m_actionSuperscript);
	m_menuFormat->addAction(m_actionSubscript);
	m_menuFormat->addSeparator();
	m_menuFormat->addAction(m_menuChangeCase->menuAction());
	m_menuFormat->addSeparator();
	m_menuFormat->addAction(m_actionResetFormat);
	m_menuChangeCase->addAction(m_actionSentenceCase);
	m_menuChangeCase->addAction(m_actionUpperCase);
	m_menuChangeCase->addAction(m_actionLowerCase);
	m_menuChangeCase->addAction(m_actionCapitalCase);
	m_menuSearch->addAction(m_actionSearchText);
	m_menuSearch->addAction(m_actionClearSearch);
}

void SummaryWindow::setupConnections()
{
	connect(m_buttonBold, &QPushButton::clicked, this, &SummaryWindow::toggleBold);
	connect(m_buttonItalic, &QPushButton::clicked, this, &SummaryWindow::toggleItalic);
	connect(m_buttonUnderline, &QPushButton::clicked, this, &SummaryWindow::toggleUnderline);
	connect(m_buttonThrough, &QPushButton::clicked, this, &SummaryWindow::toggleStrikethrough);
	connect(m_buttonBulletedList, &QPushButton::clicked, this, &SummaryWindow::toggleBulletedList);
	connect(m_buttonUndo, &QPushButton::clicked, m_textEditor, &TextEditor::undo);
	connect(m_buttonRedo, &QPushButton::clicked, m_textEditor, &TextEditor::redo);
	connect(m_buttonClearAll, &QPushButton::clicked, m_textEditor, &TextEditor::clear);
	connect(m_buttonOrderedList, &QPushButton::clicked, this, &SummaryWindow::toggleOrderedList);
	connect(m_buttonSubscript, &QPushButton::clicked, this, &SummaryWindow::setSubscript);
	connect(m_buttonSuperscript, &QPushButton::clicked, this, &SummaryWindow::setSuperscript);
	connect(m_buttonCodeBlock, &QPushButton::clicked, this, &SummaryWindow::insertCodeBlock);
	connect(m_buttonHighlight, &QPushButton::clicked, this, &SummaryWindow::toggleHighlight);
	connect(m_buttonEditorFontColor, &QPushButton::clicked, this, &SummaryWindow::setFontColor);
	connect(m_buttonInsertLine, &QPushButton::clicked, this, &SummaryWindow::insertLine);
	connect(m_buttonInsertImage, &QPushButton::clicked, this, &SummaryWindow::insertImage);
	connect(m_buttonInsertTable, &QPushButton::clicked, this, &SummaryWindow::insertTable);
	connect(m_buttonInsertLink, &QPushButton::clicked, this, &SummaryWindow::insertLink);
	connect(m_buttonEditorBackColor, &QPushButton::clicked, this, &SummaryWindow::setBackColor);
	connect(m_buttonNext, &QPushButton::clicked, this, &SummaryWindow::findNext);
	connect(m_buttonPrevious, &QPushButton::clicked, this, &SummaryWindow::findPrevious);
	connect(m_buttonIncFontSize, &QPushButton::clicked, this, &SummaryWindow::increaseFontSize);
	connect(m_buttonDecFontSize, &QPushButton::clicked, this, &SummaryWindow::decreaseFontSize);
	connect(m_buttonIncIndent, &QPushButton::clicked, this, &SummaryWindow::increaseIndent);
	connect(m_buttonDecIndent, &QPushButton::clicked, this, &SummaryWindow::decreaseIndent);
	connect(m_buttonCloseFind, &QPushButton::clicked, [this]
	{
		toggleFindWidget(false);

	});
	connect(m_fontComboBox, &QFontComboBox::currentTextChanged, [this]
	{
		setTextFontFamily(m_fontComboBox->currentText(), true);
	});
	connect(m_spinBoxFontSize, &QSpinBox::valueChanged, [this]
	{
		setTextFontSize(m_spinBoxFontSize->value(), true);
	});

	connect(m_textFind, &QLineEdit::textChanged, this, &SummaryWindow::findText);
	connect(m_comboBoxAlignment, &QComboBox::currentIndexChanged, this, &SummaryWindow::changeTextAlignment);
	connect(m_bookSearchWidget, &BookSearchWidget::selectionChanged, this, &SummaryWindow::selectEbookSummary);
	connect(m_bookSearchWidget, &BookSearchWidget::itemClicked, this, &SummaryWindow::selectEbookSummary);
	connect(m_textEditor, &TextEditor::currentCharFormatChanged, this, &SummaryWindow::textFormatChanged);
	connect(m_textEditor, &TextEditor::selectionChanged, this, &SummaryWindow::textFormatChanged);

	connect(m_actionClose, &QAction::triggered, this, &SummaryWindow::close);
	connect(m_actionMaximize, &QAction::triggered, [this]
	{ common::toggleMaximized(this); });
	connect(m_actionFullscreen, &QAction::triggered, [this]
	{ common::toggleFullscreen(this); });
	connect(m_actionMinimize, &QAction::triggered, this, &SummaryWindow::showMinimized);
	connect(m_actionHideSearchBar, &QAction::triggered, this, &SummaryWindow::hideSearchBar);
	connect(m_actionHideLeftPane, &QAction::triggered, this, &SummaryWindow::hideLeftPane);
	connect(m_actionHideTopToolbar, &QAction::triggered, this, &SummaryWindow::hideTopToolbar);
	connect(m_actionHideRightToolbar, &QAction::triggered, this, &SummaryWindow::hideRightToolbar);
	connect(m_actionHideRightPane, &QAction::triggered, this, &SummaryWindow::hideRightPane);
	connect(m_actionResetFormat, &QAction::triggered, this, &SummaryWindow::resetFormat);
	connect(m_actionPaste, &QAction::triggered, m_textEditor, &TextEditor::paste);
	connect(m_actionSearchText, &QAction::triggered, this, &SummaryWindow::searchText);
	connect(m_actionClearSearch, &QAction::triggered, this, &SummaryWindow::clearSearch);
	connect(m_actionPrintToPDF, &QAction::triggered, this, &SummaryWindow::printSummaryToPdf);
	connect(m_actionCopyFormatting, &QAction::triggered, this, &SummaryWindow::copyFormatting);
	connect(m_actionPasteMatchFormat, &QAction::triggered, this, &SummaryWindow::pasteMatchFormat);
	connect(m_actionSaveSummary, &QAction::triggered, this, &SummaryWindow::saveSummary);
	connect(m_actionExportHtml, &QAction::triggered, this, &SummaryWindow::exportSummaryToHtml);
	connect(m_actionSentenceCase, &QAction::triggered, this, &SummaryWindow::sentenceCase);
	connect(m_actionUpperCase, &QAction::triggered, this, &SummaryWindow::upperCase);
	connect(m_actionLowerCase, &QAction::triggered, this, &SummaryWindow::lowerCase);
	connect(m_actionCapitalCase, &QAction::triggered, this, &SummaryWindow::capitalCase);
	connect(m_actionSelectCurrentLine, &QAction::triggered, this, &SummaryWindow::selectCurrentLine);
	connect(m_actionDuplicateCurrentLine, &QAction::triggered, this, &SummaryWindow::duplicateCurrentLine);
	connect(m_actionDeleteCurrentLine, &QAction::triggered, this, &SummaryWindow::deleteCurrentLine);
	connect(m_actionZoomIn, &QAction::triggered, this, &SummaryWindow::zoomIn);
	connect(m_actionZoomOut, &QAction::triggered, this, &SummaryWindow::zoomOut);
	connect(m_actionDateNormal, &QAction::triggered, this, &SummaryWindow::insertNormalDate);
	connect(m_actionDateShort, &QAction::triggered, this, &SummaryWindow::insertShortDate);
	connect(m_actionTableRow, &QAction::triggered, this, &SummaryWindow::addTableRow);
	connect(m_actionTableColumn, &QAction::triggered, this, &SummaryWindow::addTableColumn);
	connect(m_actionFind, &QAction::triggered, [this]
	{
		toggleFindWidget(!m_frameFind->isVisible());
	});
	connect(m_actionBold, &QAction::triggered, this, &SummaryWindow::toggleBold);
	connect(m_actionItalic, &QAction::triggered, this, &SummaryWindow::toggleItalic);
	connect(m_actionUnderline, &QAction::triggered, this, &SummaryWindow::toggleUnderline);
	connect(m_actionThrough, &QAction::triggered, this, &SummaryWindow::toggleStrikethrough);
	connect(m_actionBulletedList, &QAction::triggered, this, &SummaryWindow::toggleBulletedList);
	connect(m_actionOrderedList, &QAction::triggered, this, &SummaryWindow::toggleOrderedList);
	connect(m_actionSubscript, &QAction::triggered, this, &SummaryWindow::setSubscript);
	connect(m_actionSuperscript, &QAction::triggered, this, &SummaryWindow::setSuperscript);
	connect(m_actionCodeBlock, &QAction::triggered, this, &SummaryWindow::insertCodeBlock);
	connect(m_actionHighlight, &QAction::triggered, this, &SummaryWindow::toggleHighlight);
	connect(m_actionIncFontSize, &QAction::triggered, this, &SummaryWindow::increaseFontSize);
	connect(m_actionDecFontSize, &QAction::triggered, this, &SummaryWindow::decreaseFontSize);
	connect(m_actionIncIndent, &QAction::triggered, this, &SummaryWindow::increaseIndent);
	connect(m_actionDecIndent, &QAction::triggered, this, &SummaryWindow::decreaseIndent);
	connect(m_actionUndo, &QAction::triggered, m_textEditor, &TextEditor::undo);
	connect(m_actionRedo, &QAction::triggered, m_textEditor, &TextEditor::redo);
	connect(m_actionClearAll, &QAction::triggered, m_textEditor, &TextEditor::clear);
	connect(m_actionCut, &QAction::triggered, m_textEditor, &TextEditor::cut);
	connect(m_actionCopy, &QAction::triggered, m_textEditor, &TextEditor::copy);
	connect(m_actionSelectAll, &QAction::triggered, m_textEditor, &TextEditor::selectAll);
	connect(m_actionInsertLine, &QAction::triggered, this, &SummaryWindow::insertLine);
	connect(m_actionInsertLink, &QAction::triggered, this, &SummaryWindow::insertLink);
	connect(m_actionInsertImage, &QAction::triggered, this, &SummaryWindow::insertImage);
	connect(m_actionInsertTable, &QAction::triggered, this, &SummaryWindow::insertTable);
}

void SummaryWindow::setupTabOrder()
{
	QWidget::setTabOrder(m_bookSearchWidget, m_textEditor);
	QWidget::setTabOrder(m_textEditor, m_fontComboBox);
	QWidget::setTabOrder(m_fontComboBox, m_spinBoxFontSize);
	QWidget::setTabOrder(m_spinBoxFontSize, m_buttonEditorFontColor);
	QWidget::setTabOrder(m_buttonEditorFontColor, m_buttonEditorBackColor);
	QWidget::setTabOrder(m_buttonEditorBackColor, m_buttonBold);
	QWidget::setTabOrder(m_buttonBold, m_buttonItalic);
	QWidget::setTabOrder(m_buttonItalic, m_buttonUnderline);
	QWidget::setTabOrder(m_buttonUnderline, m_buttonThrough);
	QWidget::setTabOrder(m_buttonThrough, m_buttonHighlight);
	QWidget::setTabOrder(m_buttonHighlight, m_buttonCodeBlock);
	QWidget::setTabOrder(m_buttonCodeBlock, m_buttonBulletedList);
	QWidget::setTabOrder(m_buttonBulletedList, m_buttonOrderedList);
	QWidget::setTabOrder(m_buttonOrderedList, m_comboBoxAlignment);
	QWidget::setTabOrder(m_comboBoxAlignment, m_buttonInsertTable);
	QWidget::setTabOrder(m_buttonInsertTable, m_buttonInsertImage);
	QWidget::setTabOrder(m_buttonInsertImage, m_buttonInsertLine);
	QWidget::setTabOrder(m_buttonInsertLine, m_buttonInsertLink);
	QWidget::setTabOrder(m_buttonInsertLink, m_buttonUndo);
	QWidget::setTabOrder(m_buttonUndo, m_buttonRedo);
	QWidget::setTabOrder(m_buttonRedo, m_buttonClearAll);
	QWidget::setTabOrder(m_buttonClearAll, m_buttonSuperscript);
	QWidget::setTabOrder(m_buttonSuperscript, m_buttonSubscript);
	QWidget::setTabOrder(m_buttonSubscript, m_buttonIncFontSize);
	QWidget::setTabOrder(m_buttonIncFontSize, m_buttonDecFontSize);
	QWidget::setTabOrder(m_buttonDecFontSize, m_buttonIncIndent);
	QWidget::setTabOrder(m_buttonIncIndent, m_buttonDecIndent);
}

void SummaryWindow::closeEvent(QCloseEvent* event)
{
	if (!m_labelTitle->text().isEmpty())
	{
		queries::updateSummary(m_labelTitle->text(), m_textEditor->toHtml());
	}
	event->accept();
}

void SummaryWindow::selectEbookSummary(const QString& name)
{
	if (!name.isEmpty())
	{
		openEbookSummary(m_bookSearchWidget->findItem(name));
	}
}

void SummaryWindow::toggleBold()
{
	QFont currentFont = m_textEditor->currentFont();
	currentFont.setBold(!currentFont.bold());
	m_textEditor->setCurrentFont(currentFont);
	textFormatChanged();
}

void SummaryWindow::toggleItalic()
{
	QFont currentFont = m_textEditor->currentFont();
	currentFont.setItalic(!currentFont.italic());
	m_textEditor->setCurrentFont(currentFont);
	textFormatChanged();
}

void SummaryWindow::toggleUnderline()
{
	QFont currentFont = m_textEditor->currentFont();
	currentFont.setUnderline(!currentFont.underline());
	m_textEditor->setCurrentFont(currentFont);
	textFormatChanged();
}

void SummaryWindow::toggleStrikethrough()
{
	QFont currentFont = m_textEditor->currentFont();
	currentFont.setStrikeOut(!currentFont.strikeOut());
	m_textEditor->setCurrentFont(currentFont);
	textFormatChanged();
}

void SummaryWindow::setTextFontFamily(const QString& arg1, bool change)
{
	if (change)
	{
		m_textEditor->setCurrentFont(arg1);
	}
	else
	{
		m_fontComboBox->blockSignals(true);
		m_fontComboBox->setCurrentFont(arg1);
		m_fontComboBox->blockSignals(false);
	}
}

void SummaryWindow::setTextFontSize(int arg1, bool change)
{
	if (change)
	{
		QFont currentFont = m_textEditor->currentFont();
		currentFont.setPointSize(arg1);
		m_textEditor->setCurrentFont(currentFont);
	}
	else
	{
		m_spinBoxFontSize->blockSignals(true);
		m_spinBoxFontSize->setValue(arg1);
		m_spinBoxFontSize->blockSignals(false);
	}
}

void SummaryWindow::toggleBulletedList()
{
	m_textEditor->createList(QTextListFormat::ListDisc);
	textFormatChanged();
}

void SummaryWindow::toggleOrderedList()
{
	m_textEditor->createList(QTextListFormat::ListDecimal);
	textFormatChanged();
}

void SummaryWindow::changeTextAlignment(int index)
{
	switch (index)
	{
	case 0:
		m_textEditor->setAlignment(Qt::AlignLeft);
		break;
	case 1:
		m_textEditor->setAlignment(Qt::AlignHCenter);
		break;
	case 2:
		m_textEditor->setAlignment(Qt::AlignRight);
		break;
	case 3:
		m_textEditor->setAlignment(Qt::AlignJustify);
		break;
	default:
		m_textEditor->setAlignment(Qt::AlignLeft);
		break;
	}
}

void SummaryWindow::setSubscript()
{
	QTextCharFormat currentFormat = m_textEditor->currentCharFormat();
	currentFormat.setVerticalAlignment(QTextCharFormat::AlignSubScript);
	m_textEditor->setCurrentCharFormat(currentFormat);
}

void SummaryWindow::setSuperscript()
{
	QTextCharFormat currentFormat = m_textEditor->currentCharFormat();
	currentFormat.setVerticalAlignment(QTextCharFormat::AlignSuperScript);
	m_textEditor->setCurrentCharFormat(currentFormat);
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

	int fontSize = m_textEditor->currentFont().pointSize();
	QFont font = QFont("Consolas");
	font.setPointSize(fontSize);

	m_textEditor->textCursor().insertFrame(frameFormat);
	m_textEditor->setCurrentFont(font);

}

void SummaryWindow::textFormatChanged()
{
	setTextFontFamily(m_textEditor->currentFont().family(), false);
	setTextFontSize(m_textEditor->currentFont().pointSize(), false);
	m_buttonEditorFontColor->setStyleSheet("background-color: " + m_textEditor->textColor().name());

	m_buttonBold->setStyleSheet(
			"background-color: " + QString(m_textEditor->currentFont().bold() ? "#1C3387" : "#2D2D30"));
	m_buttonItalic->setStyleSheet(
			"background-color: " + QString(m_textEditor->currentFont().italic() ? "#1C3387" : "#2D2D30"));
	m_buttonUnderline->setStyleSheet(
			"background-color: " + QString(m_textEditor->currentFont().underline() ? "#1C3387" : "#2D2D30"));
	m_buttonThrough->setStyleSheet(
			"background-color: " + QString(m_textEditor->currentFont().strikeOut() ? "#1C3387" : "#2D2D30"));
	m_buttonHighlight->setStyleSheet("background-color: " +
			QString(m_textEditor->currentCharFormat().background() == Qt::yellow ? "#1C3387"
																				 : "#2D2D30"));
}

void SummaryWindow::toggleHighlight()
{
	QTextCharFormat currentFormat = m_textEditor->currentCharFormat();
	currentFormat.setBackground(currentFormat.background() == Qt::yellow ? Qt::white : Qt::yellow);
	m_textEditor->setCurrentCharFormat(currentFormat);
	textFormatChanged();
}

void SummaryWindow::setFontColor()
{
	QPoint bottom = m_buttonEditorFontColor->rect().bottomLeft();
	QPoint globalPos = m_buttonEditorFontColor->mapToGlobal(bottom);
	globalPos.setY(globalPos.y() + 2);

	ColorPickerWidget* widget = new ColorPickerWidget(this, Qt::black);
	widget->move(globalPos);
	common::openDialog(widget, ":/styles/colorpickerstyle.qss");

	if (!widget->colorSelected())
	{
		return;
	}

	QString colorSheet = "background-color: " + widget->getCurrentColor().name();
	m_buttonEditorFontColor->setStyleSheet(colorSheet);
	m_buttonEditorFontColor->update();

	m_textEditor->setTextColor(widget->getCurrentColor());
}

void SummaryWindow::hideSearchBar()
{
	m_bookSearchWidget->setHideSearchBar(!m_bookSearchWidget->searchBarHidden());
	m_actionHideSearchBar->setText(!m_bookSearchWidget->searchBarHidden() ? "Show Search Bar" : "Hide Search Bar");
}

void SummaryWindow::hideLeftPane()
{
	m_bookSearchWidget->setHideWidget(!m_bookSearchWidget->widgetHidden());
	m_mainSplitterPane->refresh();
	m_actionHideLeftPane->setText(m_bookSearchWidget->widgetHidden() ? "Show Left Pane" : "Hide Left Pane");
}

void SummaryWindow::hideTopToolbar()
{
	common::changeWidgetVisibility(m_frameEditorToolBox, m_actionHideTopToolbar);
}

void SummaryWindow::hideRightToolbar()
{
	common::changeWidgetVisibility(m_frameRightToolBar, m_actionHideRightToolbar);
}

void SummaryWindow::hideRightPane()
{
	m_frameEditor->setHidden(!m_frameEditor->isHidden());
	m_mainSplitterPane->refresh();
	hideRightToolbar();
	m_actionHideRightPane->setText(m_frameEditor->isHidden() ? "Show Right Pane" : "Hide Right Pane");

}

void SummaryWindow::insertLine()
{
	QTextCursor cursor = m_textEditor->textCursor();
	cursor.insertHtml(QString("<hr><br>"));
	cursor.movePosition(QTextCursor::Up, QTextCursor::MoveAnchor, 1);
	m_textEditor->setTextCursor(cursor);
}

void SummaryWindow::resetFormat()
{
	QFont currentFont = m_textEditor->currentFont();
	currentFont.setBold(false);
	currentFont.setItalic(false);
	currentFont.setStrikeOut(false);
	currentFont.setUnderline(false);

	QTextCharFormat currentFormat = m_textEditor->currentCharFormat();
	currentFormat.setBackground(m_textEditor->palette().base().color());
	currentFormat.setVerticalAlignment(QTextCharFormat::AlignNormal);

	m_textEditor->setCurrentCharFormat(currentFormat);
	m_textEditor->setCurrentFont(currentFont);

	textFormatChanged();
}

void SummaryWindow::insertImage()
{
	QString file = QFileDialog::getOpenFileName(this,
			tr("Select Image"), "/", tr("Image Files (*.jpeg *.jpg *.png *.gif *.apng *.svg *.bmp *.ico)"));

	QUrl Uri(QString("%1").arg(file));
	QImage image = QImageReader(file).read();

	QTextDocument* textDocument = m_textEditor->document();
	textDocument->addResource(QTextDocument::ImageResource, Uri, QVariant(image));
	QTextCursor cursor = m_textEditor->textCursor();
	QTextImageFormat imageFormat;
	imageFormat.setWidth(image.width());
	imageFormat.setHeight(image.height());
	imageFormat.setName(Uri.toString());
	cursor.insertImage(imageFormat);
}

void SummaryWindow::searchText()
{
	if (!m_labelTitle->text().isEmpty())
	{
		queries::updateSummary(m_labelTitle->text(), m_textEditor->toHtml());
	}
	m_bookSearchWidget->searchString();
}

void SummaryWindow::clearSearch()
{
	m_bookSearchWidget->clearSearchText();
	m_bookSearchWidget->clearListWidget();
}

void SummaryWindow::openEbookSummary(const QString& name)
{
	if (!m_labelTitle->text().isEmpty())
	{
		queries::updateSummary(m_labelTitle->text(), m_textEditor->toHtml());
	}

	m_textEditor->clear();
	m_labelTitle->setText(name);
	queries::selectSummaryBasedonName(name);
	queries::query.next();
	QString summary = queries::query.value(0).toString();
	if (summary != "N/A")
	{
		m_textEditor->insertHtml(summary);
	}
}

void SummaryWindow::printSummaryToPdf()
{
	QString filePath = QFileDialog::getSaveFileName(this, tr("Save File"), "/", tr("*.pdf"));

	QPrinter printer(QPrinter::HighResolution);
	printer.setPrintRange(QPrinter::CurrentPage);
	printer.setOutputFormat(QPrinter::PdfFormat);
	printer.setOutputFileName(filePath);
	m_textEditor->print(&printer);
	QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
}

void SummaryWindow::insertTable()
{
	InsertTableDialog dialog(this);
	common::openDialog(&dialog, ":/styles/style.qss");

	if (dialog.m_rowCount != 0 && dialog.m_columnCount != 0)
	{
		QTextTableFormat tableFormat;
		tableFormat.setBorder(1);
		tableFormat.setCellSpacing(5);
		tableFormat.setCellPadding(1);
		tableFormat.setBorderCollapse(true);
		tableFormat.setBorderStyle(QTextTableFormat::BorderStyle_Solid);
		m_textEditor->textCursor().insertTable(dialog.m_rowCount, dialog.m_columnCount, tableFormat);
	}
}

void SummaryWindow::insertLink()
{
	InsertLinkDialog dialog(this);
	common::openDialog(&dialog, ":/styles/style.qss");

	if (!dialog.m_title.isEmpty() && !dialog.m_link.isEmpty())
	{
		QTextCharFormat charFormat;
		charFormat.setFontUnderline(true);
		charFormat.setForeground(QColor("blue"));
		charFormat.setAnchor(true);
		charFormat.setAnchorHref(dialog.m_link);
		charFormat.setToolTip(dialog.m_link);
		m_textEditor->textCursor().insertText(dialog.m_title, charFormat);
	}
}

void SummaryWindow::copyFormatting()
{
	m_currentCopiedFormat = m_textEditor->currentCharFormat();
	m_currentCopiedFont = m_textEditor->currentFont();
}

void SummaryWindow::pasteMatchFormat()
{
	m_textEditor->setCurrentCharFormat(m_currentCopiedFormat);
	m_textEditor->setCurrentFont(m_currentCopiedFont);
}

void SummaryWindow::saveSummary()
{
	if (!m_labelTitle->text().isEmpty())
	{
		queries::updateSummary(m_labelTitle->text(), m_textEditor->toHtml());
	}
}

void SummaryWindow::exportSummaryToHtml()
{
	QString filePath = QFileDialog::getSaveFileName(this, tr("Save File"), "/", tr("*.html"));
	std::ofstream htmlFile(filePath.toStdString());
	htmlFile << m_textEditor->toHtml().toStdString();
	htmlFile.close();
	QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
}

void SummaryWindow::setBackColor()
{
	QPoint bottom = m_buttonEditorBackColor->rect().bottomLeft();
	QPoint globalPos = m_buttonEditorBackColor->mapToGlobal(bottom);
	globalPos.setY(globalPos.y() + 2);

	ColorPickerWidget* widget = new ColorPickerWidget(this, Qt::white);
	widget->move(globalPos);
	common::openDialog(widget, ":/styles/colorpickerstyle.qss");

	if (!widget->colorSelected())
	{
		return;
	}

	QString styleSheet = "background-color: " + widget->getCurrentColor().name();

	QTextFrame* currentFrame = m_textEditor->textCursor().currentFrame();
	QTextFrameFormat currentFormat = currentFrame->frameFormat();
	// Apply only to code box in case cursor in code box
	if (currentFormat.border() == 1)
	{
		currentFormat.setBackground(QBrush(widget->getCurrentColor()));
		currentFrame->setFrameFormat(currentFormat);
		return;
	}

	// Apply to editor background
	m_textEditor->setStyleSheet(styleSheet);
	m_buttonEditorBackColor->setStyleSheet(styleSheet);
	if (widget->getCurrentColor() != Qt::white)
	{
		m_buttonEditorBackColor->setIcon(QIcon(":/icons/background_fill_white.png"));
	}
	else
	{
		m_buttonEditorBackColor->setIcon(QIcon(":/icons/background_fill_black.png"));
	}

	m_buttonEditorBackColor->update();
}

void SummaryWindow::sentenceCase()
{
	QTextCursor cursor = m_textEditor->textCursor();
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
	QTextCursor cursor = m_textEditor->textCursor();
	if (cursor.selectedText().isEmpty())
	{
		return;
	}

	QStringList tempList = cursor.selectedText().split(" ");
	QStringList newText;
	for (const QString& word: tempList)
	{
		newText.append(word.toUpper());
	}

	cursor.removeSelectedText();
	cursor.insertText(newText.join(" "));
}

void SummaryWindow::lowerCase()
{
	QTextCursor cursor = m_textEditor->textCursor();
	if (cursor.selectedText().isEmpty())
	{
		return;
	}

	QStringList tempList = cursor.selectedText().split(" ");
	QStringList newText;
	for (const QString& word: tempList)
	{
		newText.append(word.toLower());
	}

	cursor.removeSelectedText();
	cursor.insertText(newText.join(" "));
}

void SummaryWindow::capitalCase()
{
	QTextCursor cursor = m_textEditor->textCursor();
	if (cursor.selectedText().isEmpty())
	{
		return;
	}

	QStringList tempList = cursor.selectedText().split(" ");
	QStringList newText;
	for (const QString& word: tempList)
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
	QTextCursor cursor = m_textEditor->textCursor();
	cursor.movePosition(QTextCursor::StartOfLine);
	cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
	m_textEditor->setTextCursor(cursor);
}

void SummaryWindow::duplicateCurrentLine()
{
	QTextCursor cursor = m_textEditor->textCursor();
	cursor.movePosition(QTextCursor::StartOfLine);
	cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
	QString currentLine = cursor.selectedText();
	cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::MoveAnchor);
	cursor.insertText("\n");
	cursor.insertText(currentLine);
}

void SummaryWindow::deleteCurrentLine()
{
	QTextCursor cursor = m_textEditor->textCursor();
	cursor.movePosition(QTextCursor::StartOfLine);
	cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
	cursor.removeSelectedText();
	cursor.deletePreviousChar();
}

void SummaryWindow::zoomIn()
{
	m_textEditor->zoomIn(5);
}

void SummaryWindow::zoomOut()
{
	m_textEditor->zoomOut(5);
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
	m_textEditor->textCursor().insertText(dateStr);
}

void SummaryWindow::insertShortDate()
{
	QDate currDate = QDate::currentDate();
	QString day = QString::number(currDate.day());
	QString month = QString::number(currDate.month());
	QString year = QString::number(currDate.year());
	QString dateStr = day + "/" + month + "/" + year;
	m_textEditor->textCursor().insertText(dateStr);
}

void SummaryWindow::addTableRow()
{
	QTextCursor cursor = m_textEditor->textCursor();
	QTextTable* currTable = cursor.currentTable();
	if (currTable != nullptr)
	{
		currTable->insertRows(currTable->cellAt(cursor).row(), 1);
	}
}

void SummaryWindow::addTableColumn()
{
	QTextCursor cursor = m_textEditor->textCursor();
	QTextTable* currTable = cursor.currentTable();
	if (currTable != nullptr)
	{
		currTable->insertColumns(currTable->cellAt(cursor).column(), 1);
	}
}

// Find functionality specific
void SummaryWindow::toggleFindWidget(bool visible)
{
	m_frameFind->setVisible(visible);
	highlightText();

	if (visible)
	{
		m_textFind->setFocus();
	}
}

void SummaryWindow::highlightText()
{
	if (m_frameFind->isVisible())
	{
		m_textHighlighter.setWordPattern(m_textFind->text());
	}
	else
	{
		m_textHighlighter.setWordPattern("");
	}

	if (sender() == m_textEditor)
	{
		m_textEditor->blockSignals(true);
		m_textHighlighter.customRehighlight();
		m_textEditor->blockSignals(false);
	}
	else
	{
		m_textHighlighter.customRehighlight();
	}
}

void SummaryWindow::textEditRefreshHighlighter(int cursorIndex)
{
	if (cursorIndex >= 0)
	{
		QTextCursor currentCursor = m_textEditor->textCursor();
		currentCursor.setPosition(cursorIndex);
		m_textEditor->setTextCursor(currentCursor);
		m_textEditor->ensureCursorVisible();
	}
}

void SummaryWindow::findText(const QString& arg1)
{
	highlightText();
	if (arg1.isEmpty())
	{
		m_labelFindMatchNum->setText("");
		return;
	}
	QString labelText = QString("Found %1 m_matches").arg(QString::number(m_textHighlighter.matchNumber()));
	m_labelFindMatchNum->setText(labelText);
	QFont customFont;
	customFont.setItalic(true);
	customFont.setPointSize(12);
	customFont.setFamily("Segoe UI");
	m_labelFindMatchNum->setFont(customFont);
	m_labelFindMatchNum->setStyleSheet("color: grey");
}

void SummaryWindow::findNext()
{
	int cursorIndex = m_textHighlighter.setNextMatchStateActive();
	if (m_textHighlighter.matchIndex() == m_textHighlighter.matchNumber() - 1)
	{
		m_textHighlighter.setMatchIndex(-1);
	}
	textEditRefreshHighlighter(cursorIndex);
}

void SummaryWindow::findPrevious()
{
	int cursorIndex = m_textHighlighter.setPrevMatchStateActive();
	if (m_textHighlighter.matchIndex() == 0)
	{
		m_textHighlighter.setMatchIndex(m_textHighlighter.matchNumber() - 1);
	}
	textEditRefreshHighlighter(cursorIndex);
}

void SummaryWindow::increaseFontSize()
{
	m_spinBoxFontSize->setValue(m_spinBoxFontSize->value() + 1);
}

void SummaryWindow::decreaseFontSize()
{
	m_spinBoxFontSize->setValue(m_spinBoxFontSize->value() - 1);
}

void SummaryWindow::increaseIndent()
{
	m_textEditor->changeListIndentation(1);
}

void SummaryWindow::decreaseIndent()
{
	m_textEditor->changeListIndentation(-1);
}
