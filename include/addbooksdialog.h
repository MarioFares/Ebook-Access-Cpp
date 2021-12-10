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
    void insertBook(const QFileInfo &entry, const QString &tags, const QString &genre, const QString &author);

    void setupEntries(const QString &dir, bool recursive);

    void iterateInsertEntries(const QVector<QFileInfo> &entries, const QVector<QString> &exts,
                              const QString &tags, const QString &genres, const QString &authors);

    QVector<QFileInfo> getEntriesVector(const QString &dir, bool recursive);

    QVector<QString> getExtVector(QVector<QFileInfo> entries);

    void on_buttonClose_clicked();

    void on_buttonBrowseFolders_clicked();

    void on_buttonAdd_clicked();

private:
    Ui::addBooksDialog *ui;

};

#endif // ADDBOOKSDIALOG_H
