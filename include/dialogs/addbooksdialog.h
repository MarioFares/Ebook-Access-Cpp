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
    QPushButton *m_buttonAdd;
    QCheckBox *m_checkBoxRecursive;
    QLineEdit *m_textFolderPath;
    QLabel *m_labelAddBooks;
    QPushButton *m_buttonClose;
    QPushButton *m_buttonBrowseFolders;
    QProgressBar *m_progressBar;
    QLabel *m_labelFolder;

    // Layouts
    QHBoxLayout *m_horLayTitle;
    QHBoxLayout *m_horLayFolder;
    QHBoxLayout *m_horLayCheckBox;
    QHBoxLayout *m_horLayProgressBar;
    QHBoxLayout *m_horLayButtons;
    QVBoxLayout *m_vertLayMain;

    // Spacers
    QSpacerItem *m_horSpacerTitleLeft;
    QSpacerItem *m_horSpacerTitleRight;
    QSpacerItem *m_horSpacerCheckBoxRight;
    QSpacerItem *m_horSpacerProgressBarLeft;
    QSpacerItem *m_horSpacerProgressBarRight;
    QSpacerItem *m_horSpacerButtonsLeft;
    QSpacerItem *m_horSpacerButtonsRight;
};

#endif // ADDBOOKSDIALOG_H
