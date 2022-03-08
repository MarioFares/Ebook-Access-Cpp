#include "include/colorpickerwidget.h"
#include "include/common.h"

#include <QPair>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QApplication>
#include <QColorDialog>

colorPickerWidget::colorPickerWidget(QWidget*parent, QColor defaultColor)
    : QDialog(parent)
{
    this->defaultColor = defaultColor;
    this->currentColor = defaultColor;
    this->isColorSelected = false;

    setupUi();
    setupConnections();
}

void colorPickerWidget::setupUi()
{
    this->rows = 5;
    this->columns = 10;
    this->colorGridLayout = new QGridLayout();

    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::Popup);
    this->setFixedSize(224,187);

    this->defaultColorButton = new QPushButton("Default Color", this);
    this->moreColorsButton = new QPushButton("More Colors...", this);
    QVBoxLayout *vBoxLayout = new QVBoxLayout(this);

    QHBoxLayout *defaultButtonLayout = new QHBoxLayout();
    QSpacerItem *horizontalSpacer1 = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
    defaultButtonLayout->addWidget(defaultColorButton);
    defaultButtonLayout->addSpacerItem(horizontalSpacer1);

    QHBoxLayout *moreColorsLayout = new QHBoxLayout();
    QSpacerItem *horizontalSpacer2 = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
    moreColorsLayout->addWidget(moreColorsButton);
    moreColorsLayout->addSpacerItem(horizontalSpacer2);

    vBoxLayout->addLayout(defaultButtonLayout);
    vBoxLayout->addLayout(this->colorGridLayout);
    vBoxLayout->addLayout(moreColorsLayout);

    QFrame *mainFrame = new QFrame(this);
    mainFrame->setLayout(vBoxLayout);

    GradientList colorGradients = initialialzeColorGradients();
    this->columns = colorGradients.size();
    createColorsButtons();
    setButtonColors(colorGradients);

    moreColorsButton->setCursor(Qt::PointingHandCursor);
    moreColorsButton->setObjectName("moreColorsButton");
    moreColorsButton->setFlat(true);
    moreColorsButton->setIcon(QIcon(":/icons/more_colors_icon.png"));

    defaultColorButton->setCursor(Qt::PointingHandCursor);
    defaultColorButton->setObjectName("defaultColorButton");
    defaultColorButton->setFlat(true);
    QPixmap pixmap(13, 13);
    pixmap.fill(this->defaultColor);
    defaultColorButton->setIcon(QIcon(pixmap));
    defaultColorButton->setIconSize(QSize(16, 16));
}

void colorPickerWidget::setupConnections()
{
    connect(defaultColorButton, &QPushButton::clicked, [this]
    {
       setCurrentColor(this->defaultColor);
    });
    connect(moreColorsButton, SIGNAL(clicked()), this, SLOT(openColorDialog()));
}

QColor colorPickerWidget::interpolateColor(QColor colorStart, QColor colorEnd, float percent)
{
    double resultRedF = colorStart.redF()   + percent * (colorEnd.redF()   - colorStart.redF());
    double resultGreenF = colorStart.greenF() + percent * (colorEnd.greenF() - colorStart.greenF());
    double resultBlueF = colorStart.blueF()  + percent * (colorEnd.blueF()  - colorStart.blueF());
    QColor interpolatedColor;
    interpolatedColor.setRedF(resultRedF);
    interpolatedColor.setGreenF(resultGreenF);
    interpolatedColor.setBlueF(resultBlueF);
    return interpolatedColor;
}

GradientList colorPickerWidget::initialialzeColorGradients()
{
    GradientList gradientList;
    gradientList.push_back(qMakePair(Qt::white            ,             Qt::black)); // white         -> black
    gradientList.push_back(qMakePair(QColor( 93, 218, 255), QColor( 20, 113, 145))); // cyan          -> blue green
    gradientList.push_back(qMakePair(QColor(129, 182, 255), QColor( 28,  51, 135))); // light blue    -> dark blue
    gradientList.push_back(qMakePair(QColor(186,   0, 255), QColor( 65,   0, 125))); // magenta       -> violet
    gradientList.push_back(qMakePair(QColor(229,   0, 255), QColor(106,   0, 129))); // pink          -> wine
    gradientList.push_back(qMakePair(QColor(241,  79, 154), QColor(123,   0,  61))); // bright orange -> brown
    gradientList.push_back(qMakePair(QColor(255, 122, 116), QColor(173,   0,   0))); // orange        -> dark red
    gradientList.push_back(qMakePair(QColor(251, 205,  95), QColor(167,  69,   0))); // light orange  -> dark orange
    gradientList.push_back(qMakePair(QColor(245, 255, 125), QColor(147, 150,   0))); // light yellow  -> gray
    gradientList.push_back(qMakePair(QColor(150, 238, 125), QColor( 50, 135,  18))); // lime green    -> dark green
    return gradientList;
}

void colorPickerWidget::createColorsButtons()
{
    for (int row = 0; row < this->rows; ++row)
    {
        for (int column = 0; column < this->columns; ++column)
        {
            QPushButton* colorButton = new QPushButton("");
            colorButton->setCursor(Qt::PointingHandCursor);
            this->colorGridLayout->addWidget(colorButton, row, column);
            connect(colorButton, &QPushButton::clicked, this,
            [this, colorButton]()
            {
               setCurrentColor(colorButton->palette().button().color());
            }, Qt::DirectConnection);
        }
    }
}

void colorPickerWidget::setButtonColors(GradientList gradientList)
{
    QColor colorStart;
    QColor colorEnd;
    for (int column = 0; column < gradientList.size(); ++column)
    {
        float percent = 0.0F;
        colorStart = gradientList.at(column).first;
        colorEnd = gradientList.at(column).second;
        for (int row = 0; row < this->rows; ++row)
        {
            QPushButton* colorButton = qobject_cast<QPushButton*>(this->colorGridLayout->itemAtPosition(row, column)->widget());
            colorButton->setFixedSize(QSize(15, 15));
            colorButton->setCursor(Qt::PointingHandCursor);
            QColor interpolatedColor = interpolateColor(colorStart, colorEnd, percent);
            colorButton->setStyleSheet(QString("QPushButton{"
                                       "background-color: rgb(%0,%1,%2);"
                                       "border : none;"
                                       "}").arg(interpolatedColor.red()).arg(interpolatedColor.green()).arg(interpolatedColor.blue()));
            percent += 1.0F / (this->rows - 1);
        }
    }
}

void colorPickerWidget::setCurrentColor(QColor color)
{
    this->close();
    this->currentColor = color;
    this->isColorSelected = true;
}

QColor colorPickerWidget::getCurrentColor()
{
    return this->currentColor;
}

void colorPickerWidget::openColorDialog()
{
    this->close();
    QColorDialog dialog(this);
    common::openDialog(&dialog, ":/styles/summarystyle.qss");
    setCurrentColor(dialog.selectedColor());
}

bool colorPickerWidget::colorSelected()
{
    return this->isColorSelected;
}
