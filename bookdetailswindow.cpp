#include "bookdetailswindow.h"
#include "ui_bookdetailswindow.h"

bookDetailsWindow::bookDetailsWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::bookDetailsWindow)
{
    ui->setupUi(this);
}

bookDetailsWindow::~bookDetailsWindow()
{
    delete ui;
}
