#ifndef CLEANEBOOKSDIALOG_H
#define CLEANEBOOKSDIALOG_H

#include <QDialog>

namespace Ui {
class cleanEbooksDialog;
}

class CleanEbooksDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CleanEbooksDialog(QWidget *parent = nullptr);
    ~CleanEbooksDialog();

private slots:
    void setupConnections();

    void beginClean();

private:
    Ui::cleanEbooksDialog *ui;
};

#endif // CLEANEBOOKSDIALOG_H
