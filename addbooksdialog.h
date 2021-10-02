#ifndef ADDBOOKSDIALOG_H
#define ADDBOOKSDIALOG_H

#include <QDialog>
#include <QFileInfo>


namespace Ui {
class addBooksDialog;
}

class addBooksDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addBooksDialog(QWidget *parent = nullptr);
    ~addBooksDialog();

private slots:
    void insertBooks(QFileInfo entry, QString tags, QString genre, QString author);

    void setupEntries(QString dir, bool recursive);

    void iterateInsertEntries(QVector<QFileInfo> entries, QVector<QString> exts, QString tags, QString genres, QString authors);

    QVector<QFileInfo> getEntriesVector(QString dir, bool recursive);

    QVector<QString> getExtVector(QVector<QFileInfo> entries);

    void on_buttonClose_clicked();

    void on_buttonBrowseFolders_clicked();

    void on_buttonAdd_clicked();

private:
    Ui::addBooksDialog *ui;

};

#endif // ADDBOOKSDIALOG_H
