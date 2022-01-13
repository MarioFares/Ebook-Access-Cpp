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

private:
    Ui::quoteDialog *ui;
};

#endif // QUOTEDIALOG_H
