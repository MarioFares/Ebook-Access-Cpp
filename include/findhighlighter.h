#ifndef FINDHIGHLIGHTER_H
#define FINDHIGHLIGHTER_H

#include <QMap>
#include <QTextCharFormat>
#include <QSyntaxHighlighter>
#include <QRegularExpression>

class QTextDocument;

class FindHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    FindHighlighter(QTextDocument *parent = nullptr);

    void setWordPattern(const QString &word);

    void highlightBlock(const QString &text);

    qint32 setNextMatchStateActive();

    qint32 setPrevMatchStateActive();

    void customRehighlight();

    qint32 matchNumber();

    qint32 matchIndex();

    void setMatchIndex(qint32 matchIndex = 0);

private:
struct HighlightingRule
{
    QRegularExpression pattern;
    QTextCharFormat format;
};

    QString findString;

    HighlightingRule defaultHighlightingRule;

    HighlightingRule activeHighlightingRule;

    QRegularExpressionMatchIterator matches;

    QList<QRegularExpressionMatch> matchList;

    qint32 currentMatchIndex;

    qint32 activeMatchIndex;

    QTextBlock activeBlock;
};

#endif // FINDHIGHLIGHTER_H
