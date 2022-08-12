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
	explicit FindHighlighter(QTextDocument* parent = nullptr);

	void setWordPattern(const QString& word);

	void highlightBlock(const QString& text) override;

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
	QString m_findString;
	HighlightingRule m_defaultHighlightingRule;
	HighlightingRule m_activeHighlightingRule;
	QRegularExpressionMatchIterator m_matches;
	QList<QRegularExpressionMatch> m_matchList;
	qint32 m_currentMatchIndex;
	qint32 m_activeMatchIndex;
	QTextBlock m_activeBlock;
};

#endif // FINDHIGHLIGHTER_H
