#ifndef FINDHIGHLIGHTER_H
#define FINDHIGHLIGHTER_H

#include <QMap>
#include <QTextCharFormat>
#include <QSyntaxHighlighter>
#include <QRegularExpression>

class QTextDocument;

class FindHighlighter : public QSyntaxHighlighter {
    Q_OBJECT

public:
    explicit FindHighlighter(QTextDocument *parent = nullptr);

    void setWordPattern(const QString &word);

    void highlightBlock(const QString &text) override;

    qint32 setNextMatchStateActive();

    qint32 setPrevMatchStateActive();

    void customRehighlight();

    qint32 matchNumber();

    qint32 matchIndex();

    void setMatchIndex(qint32 matchIndex = 0);

private:
    struct HighlightingRule {
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    QString _findString;
    HighlightingRule _defaultHighlightingRule;
    HighlightingRule _activeHighlightingRule;
    QRegularExpressionMatchIterator _matches;
    QList<QRegularExpressionMatch> _matchList;
    qint32 _currentMatchIndex;
    qint32 _activeMatchIndex;
    QTextBlock _activeBlock;
};

#endif // FINDHIGHLIGHTER_H
