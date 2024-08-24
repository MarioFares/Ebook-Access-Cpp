#ifndef YESNODIALOG_H
#define YESNODIALOG_H

#include <QLabel>
#include <QDialog>
#include <QBoxLayout>
#include <QPushButton>

class YesNoDialog : public QDialog {
    Q_OBJECT

public:
    explicit YesNoDialog(QWidget *parent = nullptr, QString windowTitle = "Select",
                         QString title = "Select", QString prompt = "Select");

    bool getResult() const;

private slots:
    void setupInterface();

    void setupConnections();

    void setupTabOrder();

    void setResult();

private:
    QString _windowTitle;
    QString _title;
    QString _prompt;
    bool _result;

    // Widgets
    QLabel *_labelTitle;
    QLabel *_labelPrompt;
    QPushButton *_buttonYes;
    QPushButton *_buttonNo;

    // Layouts
    QHBoxLayout *_horLayTitle;
    QHBoxLayout *_horLayButtons;
    QVBoxLayout *_vertLayMain;

    // Spacers
    QSpacerItem *_horSpacerTitleLeft;
    QSpacerItem *_horSpacerTitleRight;
    QSpacerItem *_horSpacerButtonsLeft;
};

#endif // YESNODIALOG_H
