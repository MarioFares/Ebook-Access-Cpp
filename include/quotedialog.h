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
    void on_buttonNext_clicked();

    void on_buttonPrevious_clicked();

private:
    Ui::quoteDialog *ui;
    QStringList quotesList;
    quint32 currIndex;
};

#endif // QUOTEDIALOG_H
