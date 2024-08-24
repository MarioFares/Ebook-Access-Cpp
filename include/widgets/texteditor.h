#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QHash>
#include <QWidget>
#include <QTextBrowser>

class TextEditor : public QTextBrowser {
    Q_OBJECT

public:
    explicit TextEditor(QWidget *parent = nullptr);

    void createList(QTextListFormat::Style listStyle);

    int changeListIndentation(const int &increment);

private slots:
    void keyPressEvent(QKeyEvent *ev) override;

    void handleReturn();

    void handleBackspace();

    void insertClosingChar(const char &openChar, const QString &selectedText);

private:
    QHash<char, char> _charPairs;
};

#endif // TEXTEDITOR_H
