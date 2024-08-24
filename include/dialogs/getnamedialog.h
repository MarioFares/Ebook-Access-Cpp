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
                           QString prompt = "Please input a m_name");

    QString m_name;

    void setDataOnOpen();

private slots:
    void setupInterface();

    void setupConnections();

    void setupTabOrder();

    void setName();

private:
    QString m_title;
    QString m_prompt;

    // Widgets
    QLabel *m_labelTitle;
    QLabel *m_labelPrompt;
    QLineEdit *m_textName;
    QPushButton *m_buttonOk;
    QPushButton *m_buttonCancel;

    // Layouts
    QHBoxLayout *m_horLayTitle;
    QHBoxLayout *m_horLayButtons;
    QVBoxLayout *m_vertLayMain;

    // Spacers
    QSpacerItem *m_horSpacerTitleLeft;
    QSpacerItem *m_horSpacerTitleRight;
    QSpacerItem *m_horSpacerButtonsLeft;
    QSpacerItem *m_horSpacerButtonsRight;
};

#endif // GETNAMEDIALOG_H
