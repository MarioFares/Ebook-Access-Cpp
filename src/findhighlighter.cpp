#include "include/findhighlighter.h"
#include <QTextDocument>

FindHighlighter::FindHighlighter(QTextDocument *parent) : QSyntaxHighlighter(parent)
{
    defaultHighlightingRule.format.setBackground(Qt::yellow);
    activeHighlightingRule.format.setBackground(QColor(255, 165, 0));
}

void FindHighlighter::setWordPattern(const QString &pattern)
{
    if (pattern != findString)
    {
        findString = pattern;
        defaultHighlightingRule.pattern = QRegularExpression(pattern, QRegularExpression::PatternOption::CaseInsensitiveOption);
        activeMatchIndex = 0;
    }
}

void FindHighlighter::highlightBlock(const QString &text)
{
    if (findString.isEmpty())
    {
        setFormat(0, document()->toPlainText().length(), QTextCharFormat());
        activeMatchIndex = 0;
    }
    else
    {
        matches = defaultHighlightingRule.pattern.globalMatch(text);
        while (matches.hasNext())
        {
            QRegularExpressionMatch match = matches.next();
            if (match.hasPartialMatch() || match.hasMatch())
            {
                int startOffset = match.capturedStart();
                int endOffset   = match.capturedEnd();
                QTextCharFormat format = defaultHighlightingRule.format;
                if (activeMatchIndex == currentMatchIndex)
                {
                    format = activeHighlightingRule.format;
                    activeBlock = currentBlock();
                }
                setFormat(startOffset, endOffset - startOffset, format);
                matchList.push_back(match);
                ++currentMatchIndex;
            }
        }
    }
}

void FindHighlighter::customRehighlight()
{
    matchList.clear();
    currentMatchIndex = 0;
    rehighlight();
}

int FindHighlighter::setNextMatchStateActive()
{
    int startIndex = -1;
    if (activeMatchIndex + 1 < matchList.size())
    {
        ++activeMatchIndex;
        customRehighlight();
        startIndex = matchList[activeMatchIndex].capturedEnd() + activeBlock.position();
    }
    return startIndex;
}

int FindHighlighter::setPrevMatchStateActive()
{
    int startIndex = -1;
    if (activeMatchIndex - 1 >= 0)
    {
        --activeMatchIndex;
        customRehighlight();
        startIndex = matchList[activeMatchIndex].capturedEnd()  + activeBlock.position();
    }
    return startIndex;
}

qint32 FindHighlighter::matchNumber()
{
    return matchList.size();
}

qint32 FindHighlighter::matchIndex()
{
    return activeMatchIndex;
}

void FindHighlighter::setMatchIndex(qint32 matchIndex)
{
    activeMatchIndex = matchIndex;
}
