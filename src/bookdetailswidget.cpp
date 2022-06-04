#include "bookdetailswidget.h"
#include "ui_bookdetailswidget.h"

BookDetailsWidget::BookDetailsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BookDetailsWidget)
{
    ui->setupUi(this);
}

BookDetailsWidget::~BookDetailsWidget()
{
    delete ui;
}
