#include "include/bookdetailswindow.h"
#include "ui_bookdetailswindow.h"

BookDetailsWindow::BookDetailsWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BookDetailsWindow)
{
    ui->setupUi(this);
    ui->statusbar->setHidden(true);
}

BookDetailsWindow::~BookDetailsWindow()
{
    delete ui;
}
