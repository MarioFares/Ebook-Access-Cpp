#ifndef CLEANEBOOKSDIALOG_H
#define CLEANEBOOKSDIALOG_H

#include <QLabel>
#include <QDialog>
#include <QBoxLayout>
#include <QSpacerItem>
#include <QListWidget>

class CleanBooksDialog : public QDialog {
    Q_OBJECT

public:
    explicit CleanBooksDialog(QWidget *parent = nullptr);

private slots:
    void setupInterface();

    void setupConnections();

    void setupTabOrder();

    void beginClean();

private:
    // Widgets
    QLabel *_labelTitle;
    QPushButton *_buttonBegin;
    QListWidget *_listWidget;

    // Layouts
    QHBoxLayout *_horLayTitle;
    QHBoxLayout *_horLayButton;
    QVBoxLayout *_vertLayMain;

    // Spacers
    QSpacerItem *_horSpacerTitleLeft;
    QSpacerItem *_horSpacerTitleRight;
    QSpacerItem *_horSpacerButtonLeft;
    QSpacerItem *_horSpacerButtonRight;
};

#endif // CLEANEBOOKSDIALOG_H
