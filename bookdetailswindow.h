#ifndef BOOKDETAILSWINDOW_H
#define BOOKDETAILSWINDOW_H

#include <QMainWindow>

namespace Ui {
class bookDetailsWindow;
}

class bookDetailsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit bookDetailsWindow(QWidget *parent = nullptr);
    ~bookDetailsWindow();

private:
    Ui::bookDetailsWindow *ui;
};

#endif // BOOKDETAILSWINDOW_H
