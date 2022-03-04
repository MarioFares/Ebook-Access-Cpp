#ifndef BOOKDETAILSWINDOW_H
#define BOOKDETAILSWINDOW_H

#include <QMainWindow>

namespace Ui {
class BookDetailsWindow;
}

class BookDetailsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BookDetailsWindow(QWidget *parent = nullptr);
    ~BookDetailsWindow();

private:
    Ui::BookDetailsWindow *ui;
};

#endif // BOOKDETAILSWINDOW_H
