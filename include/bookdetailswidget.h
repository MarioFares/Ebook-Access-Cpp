#ifndef BOOKDETAILSWIDGET_H
#define BOOKDETAILSWIDGET_H

#include <QWidget>

namespace Ui {
class BookDetailsWidget;
}

class BookDetailsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BookDetailsWidget(QWidget *parent = nullptr);
    ~BookDetailsWidget();

private:
    Ui::BookDetailsWidget *ui;
};

#endif // BOOKDETAILSWIDGET_H
