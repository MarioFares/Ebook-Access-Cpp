#include "include/texteditor.h"

#include <QEvent>
#include <QKeyEvent>
#include <QTextList>
#include <QTextBlock>
#include <QTextBrowser>

TextEditor::TextEditor(QWidget* parent) : QTextBrowser(parent)
{
	m_charPairs['('] = ')';
	m_charPairs['"'] = '"';
	m_charPairs['['] = ']';
	m_charPairs['{'] = '}';
}

void TextEditor::keyPressEvent(QKeyEvent* ev)
{
	switch (ev->key())
	{
	case Qt::Key_Tab:
		changeListIndentation(1);
		break;
	case Qt::Key_Backtab:
		changeListIndentation(-1);
		break;
	case Qt::Key_ParenLeft:
		insertClosingChar('(', textCursor().selectedText());
		break;
	case Qt::Key_QuoteDbl:
		insertClosingChar('"', textCursor().selectedText());
		break;
	case Qt::Key_BracketLeft:
		insertClosingChar('[', textCursor().selectedText());
		break;
	case Qt::Key_BraceLeft:
		insertClosingChar('{', textCursor().selectedText());
		break;
	case Qt::Key_Backspace:
		handleBackspace();
		break;
	case Qt::Key_Return:
		handleReturn();
		break;
	default:
		QTextBrowser::keyPressEvent(ev);
		break;
	}
}

void TextEditor::insertClosingChar(const char& openChar, const QString& selectedText)
{
	QTextCursor cursor = textCursor();
	QString newText = openChar + selectedText + m_charPairs[openChar];
	cursor.removeSelectedText();
	cursor.insertText(newText);
	if (selectedText == "")
	{
		cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 1);
		setTextCursor(cursor);
	}
}

void TextEditor::handleBackspace()
{
	QTextCursor cursor = textCursor();
	QTextBlock currentBlock = cursor.block();
	if (!cursor.selectedText().isEmpty())
	{
		cursor.clearSelection();
	}

	if (cursor.currentList() != nullptr && currentBlock.text().isEmpty())
	{
		if (changeListIndentation(-1) == 0)
		{
			int index = textCursor().currentList()->itemNumber(currentBlock);
			cursor.currentList()->removeItem(index);
		}
	}
	else
	{
		textCursor().deletePreviousChar();
	}
}

void TextEditor::handleReturn()
{
	QTextCursor cursor = textCursor();
	QTextBlock currentBlock = cursor.block();
	if (!cursor.selectedText().isEmpty())
	{
		cursor.clearSelection();
	}

	if (cursor.currentList() != nullptr && currentBlock.text().isEmpty())
	{
		if (changeListIndentation(-1) == 0)
		{
			int index = textCursor().currentList()->itemNumber(currentBlock);
			cursor.currentList()->removeItem(index);
		}
	}
	else
	{
		insertPlainText("\n");
	}
}

void TextEditor::createList(QTextListFormat::Style listStyle)
{
	QTextCursor cursor = textCursor();
	cursor.setKeepPositionOnInsert(true);
	QTextList* currentList = cursor.currentList();
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

int TextEditor::changeListIndentation(const int& increment)
{
	QTextCursor cursor = textCursor();
	QTextList* currentList = cursor.currentList();

	if (currentList != nullptr)
	{
		QTextListFormat listFormat;
		QTextListFormat::Style currentStyle = currentList->format().style();
		listFormat.setIndent(cursor.currentList()->format().indent() + increment);
		if (currentStyle == QTextListFormat::ListDisc || currentStyle == QTextListFormat::ListCircle ||
				currentStyle == QTextListFormat::ListSquare)
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

		if (currentStyle == QTextListFormat::ListDecimal || currentStyle == QTextListFormat::ListLowerAlpha ||
				currentStyle == QTextListFormat::ListLowerRoman)
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
		blockFormat.setIndent(
				increment == 1 ? blockFormat.indent() + 1 : (blockFormat.indent() == 0 ? 0 : blockFormat.indent() - 1));
		cursor.setBlockFormat(blockFormat);
		return blockFormat.indent();
	}
}

