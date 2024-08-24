#ifndef QUOTEDIALOG_H
#define QUOTEDIALOG_H

#include <QLabel>
#include <QDialog>
#include <QBoxLayout>
#include <QPushButton>

class QuoteDialog : public QDialog {
    Q_OBJECT

public:
    explicit QuoteDialog(QWidget *parent = nullptr);

private slots:
    void setupInterface();

    void setupConnections();

    void setupTabOrder();

    void nextQuote();

    void prevQuote();

private:
    QStringList _quotesList;
    quint32 _currIndex;

    // Widgets
    QLabel *_labelQuote;
    QPushButton *_buttonPrevious;
    QPushButton *_buttonNext;

    // Layouts
    QHBoxLayout *_horLayQuote;
    QHBoxLayout *_horLayButtons;

    // Spacers
    QSpacerItem *_horSpacerQuoteLeft;
    QSpacerItem *_horSpacerQuoteRight;
    QSpacerItem *_horSpacerButtonsLeft;
    QSpacerItem *_horSpacerButtonsRight;
    QVBoxLayout *_vertLayMain;
};

#endif // QUOTEDIALOG_H
