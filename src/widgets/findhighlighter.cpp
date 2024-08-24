#include "include/widgets/findhighlighter.h"

#include <QTextDocument>

FindHighlighter::FindHighlighter(QTextDocument *parent) : QSyntaxHighlighter(parent) {
    m_defaultHighlightingRule.format.setBackground(Qt::yellow);
    m_activeHighlightingRule.format.setBackground(QColor(255, 165, 0));
}

void FindHighlighter::setWordPattern(const QString &pattern) {
    if (pattern != m_findString) {
        m_findString = pattern;
        m_defaultHighlightingRule.pattern = QRegularExpression(pattern,
                                                               QRegularExpression::PatternOption::CaseInsensitiveOption);
        m_activeMatchIndex = 0;
    }
}

void FindHighlighter::highlightBlock(const QString &text) {
    if (m_findString.isEmpty()) {
        setFormat(0, document()->toPlainText().length(), QTextCharFormat());
        m_activeMatchIndex = 0;
    } else {
        m_matches = m_defaultHighlightingRule.pattern.globalMatch(text);
        while (m_matches.hasNext()) {
            QRegularExpressionMatch match = m_matches.next();
            if (match.hasPartialMatch() || match.hasMatch()) {
                int startOffset = match.capturedStart();
                int endOffset = match.capturedEnd();
                QTextCharFormat format = m_defaultHighlightingRule.format;
                if (m_activeMatchIndex == m_currentMatchIndex) {
                    format = m_activeHighlightingRule.format;
                    m_activeBlock = currentBlock();
                }
                setFormat(startOffset, endOffset - startOffset, format);
                m_matchList.push_back(match);
                ++m_currentMatchIndex;
            }
        }
    }
}

void FindHighlighter::customRehighlight() {
    m_matchList.clear();
    m_currentMatchIndex = 0;
    rehighlight();
}

int FindHighlighter::setNextMatchStateActive() {
    int startIndex = -1;
    if (m_activeMatchIndex + 1 < m_matchList.size()) {
        ++m_activeMatchIndex;
        customRehighlight();
        startIndex = m_matchList[m_activeMatchIndex].capturedEnd() + m_activeBlock.position();
    }
    return startIndex;
}

int FindHighlighter::setPrevMatchStateActive() {
    int startIndex = -1;
    if (m_activeMatchIndex - 1 >= 0) {
        --m_activeMatchIndex;
        customRehighlight();
        startIndex = m_matchList[m_activeMatchIndex].capturedEnd() + m_activeBlock.position();
    }
    return startIndex;
}

qint32 FindHighlighter::matchNumber() {
    return m_matchList.size();
}

qint32 FindHighlighter::matchIndex() {
    return m_activeMatchIndex;
}

void FindHighlighter::setMatchIndex(qint32 matchIndex) {
    m_activeMatchIndex = matchIndex;
}
