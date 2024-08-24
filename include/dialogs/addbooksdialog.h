#ifndef ADDBOOKSDIALOG_H
#define ADDBOOKSDIALOG_H

#include <QLabel>
#include <QLayout>
#include <QDialog>
#include <QFileInfo>
#include <QCheckBox>
#include <QLineEdit>
#include <QProgressBar>

class AddBooksDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddBooksDialog(QWidget *parent = nullptr);

private slots:
    void setupInterface();

    void setupConnections();

    void setupTabOrder();

    static void insertBook(const QFileInfo &entry, const QString &tags, const QString &genre, const QString &author,
                           const QString &publisher, const QString &datePublished, const QString &series,
                           quint32 rating, quint32 status);

    void setupEntries(const QString &dir, bool recursive);

    void iterateInsertEntries(const QVector<QFileInfo> &entries, const QVector<QString> &exts,
                              const QString &tags, const QString &genres, const QString &authors,
                              const QString &publisher, const QString &datePublished, const QString &series,
                              quint32 rating, quint32 status);

    void browseDirs();

    void addDir();

private:
    // Widgets
    QPushButton *_buttonAdd;
    QCheckBox *_checkBoxRecursive;
    QLineEdit *_textFolderPath;
    QLabel *_labelAddBooks;
    QPushButton *_buttonClose;
    QPushButton *_buttonBrowseFolders;
    QProgressBar *_progressBar;
    QLabel *_labelFolder;

    // Layouts
    QHBoxLayout *_horLayTitle;
    QHBoxLayout *_horLayFolder;
    QHBoxLayout *_horLayCheckBox;
    QHBoxLayout *_horLayProgressBar;
    QHBoxLayout *_horLayButtons;
    QVBoxLayout *_vertLayMain;

    // Spacers
    QSpacerItem *_horSpacerTitleLeft;
    QSpacerItem *_horSpacerTitleRight;
    QSpacerItem *_horSpacerCheckBoxRight;
    QSpacerItem *_horSpacerProgressBarLeft;
    QSpacerItem *_horSpacerProgressBarRight;
    QSpacerItem *_horSpacerButtonsLeft;
    QSpacerItem *_horSpacerButtonsRight;
};

#endif // ADDBOOKSDIALOG_H
