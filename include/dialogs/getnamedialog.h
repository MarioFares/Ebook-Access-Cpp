#ifndef GETNAMEDIALOG_H
#define GETNAMEDIALOG_H

#include <QLabel>
#include <QDialog>
#include <QLineEdit>
#include <QBoxLayout>

class GetNameDialog : public QDialog {
    Q_OBJECT

public:
    explicit GetNameDialog(QWidget *parent = nullptr,
                           QString title = "Input Name",
                           QString prompt = "Please input a _name");

    QString _name;

    void setDataOnOpen();

private slots:
    void setupInterface();

    void setupConnections();

    void setupTabOrder();

    void setName();

private:
    QString _title;
    QString _prompt;

    // Widgets
    QLabel *_labelTitle;
    QLabel *_labelPrompt;
    QLineEdit *_textName;
    QPushButton *_buttonOk;
    QPushButton *_buttonCancel;

    // Layouts
    QHBoxLayout *_horLayTitle;
    QHBoxLayout *_horLayButtons;
    QVBoxLayout *_vertLayMain;

    // Spacers
    QSpacerItem *_horSpacerTitleLeft;
    QSpacerItem *_horSpacerTitleRight;
    QSpacerItem *_horSpacerButtonsLeft;
    QSpacerItem *_horSpacerButtonsRight;
};

#endif // GETNAMEDIALOG_H
