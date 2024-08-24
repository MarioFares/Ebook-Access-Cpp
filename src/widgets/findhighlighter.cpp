#include "include/widgets/findhighlighter.h"

#include <QTextDocument>

FindHighlighter::FindHighlighter(QTextDocument *parent) : QSyntaxHighlighter(parent) {
    _defaultHighlightingRule.format.setBackground(Qt::yellow);
    _activeHighlightingRule.format.setBackground(QColor(255, 165, 0));
}

void FindHighlighter::setWordPattern(const QString &pattern) {
    if (pattern != _findString) {
        _findString = pattern;
        _defaultHighlightingRule.pattern = QRegularExpression(pattern,
                                                               QRegularExpression::PatternOption::CaseInsensitiveOption);
        _activeMatchIndex = 0;
    }
}

void FindHighlighter::highlightBlock(const QString &text) {
    if (_findString.isEmpty()) {
        setFormat(0, document()->toPlainText().length(), QTextCharFormat());
        _activeMatchIndex = 0;
    } else {
        _matches = _defaultHighlightingRule.pattern.globalMatch(text);
        while (_matches.hasNext()) {
            QRegularExpressionMatch match = _matches.next();
            if (match.hasPartialMatch() || match.hasMatch()) {
                int startOffset = match.capturedStart();
                int endOffset = match.capturedEnd();
                QTextCharFormat format = _defaultHighlightingRule.format;
                if (_activeMatchIndex == _currentMatchIndex) {
                    format = _activeHighlightingRule.format;
                    _activeBlock = currentBlock();
                }
                setFormat(startOffset, endOffset - startOffset, format);
                _matchList.push_back(match);
                ++_currentMatchIndex;
            }
        }
    }
}

void FindHighlighter::customRehighlight() {
    _matchList.clear();
    _currentMatchIndex = 0;
    rehighlight();
}

int FindHighlighter::setNextMatchStateActive() {
    int startIndex = -1;
    if (_activeMatchIndex + 1 < _matchList.size()) {
        ++_activeMatchIndex;
        customRehighlight();
        startIndex = _matchList[_activeMatchIndex].capturedEnd() + _activeBlock.position();
    }
    return startIndex;
}

int FindHighlighter::setPrevMatchStateActive() {
    int startIndex = -1;
    if (_activeMatchIndex - 1 >= 0) {
        --_activeMatchIndex;
        customRehighlight();
        startIndex = _matchList[_activeMatchIndex].capturedEnd() + _activeBlock.position();
    }
    return startIndex;
}

qint32 FindHighlighter::matchNumber() {
    return _matchList.size();
}

qint32 FindHighlighter::matchIndex() {
    return _activeMatchIndex;
}

void FindHighlighter::setMatchIndex(qint32 matchIndex) {
    _activeMatchIndex = matchIndex;
}
