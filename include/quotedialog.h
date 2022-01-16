#ifndef QUOTEDIALOG_H
#define QUOTEDIALOG_H

#include <QDialog>

namespace Ui {
class quoteDialog;
}

class quoteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit quoteDialog(QWidget *parent = nullptr);
    ~quoteDialog();

private slots:
    void setupConnections();

    void nextQuote();

    void prevQuote();

private:
    Ui::quoteDialog *ui;
    QStringList quotesList;
    quint32 currIndex;
};

#endif // QUOTEDIALOG_H
