#ifndef ADDBOOKSDIALOG_H
#define ADDBOOKSDIALOG_H

#include <QDialog>
#include <QFileInfo>


namespace Ui {
class addBooksDialog;
}

class AddBooksDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddBooksDialog(QWidget *parent = nullptr);
    ~AddBooksDialog();

private slots:
    void setupConnections();

    void insertBook(const QFileInfo &entry, const QString &tags, const QString &genre, const QString &author);

    void setupEntries(const QString &dir, bool recursive);

    void iterateInsertEntries(const QVector<QFileInfo> &entries, const QVector<QString> &exts,
                              const QString &tags, const QString &genres, const QString &authors);

    void browseDirs();

    void addDir();

private:
    Ui::addBooksDialog *ui;

};

#endif // ADDBOOKSDIALOG_H
