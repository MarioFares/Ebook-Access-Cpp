#include "include/widgets/colorpickerwidget.h"
#include "include/common.h"

#include <QPair>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QApplication>
#include <QColorDialog>

ColorPickerWidget::ColorPickerWidget(QWidget* parent, QColor defaultColor) : QDialog(parent)
{
	m_defaultColor = defaultColor;
	m_currentColor = defaultColor;
	m_isColorSelected = false;

	setupInterface();
	setupConnections();
}

void ColorPickerWidget::setupInterface()
{
	// Window
	m_rows = 5;
	m_columns = 10;
	m_colorGridLayout = new QGridLayout();
	setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::Popup);
	setFixedSize(224, 171);

	// Widgets
	m_defaultColorButton = new QPushButton("Default Color", this);
	m_moreColorsButton = new QPushButton("More Colors...", this);
	m_vertLayMain = new QVBoxLayout(this);

	m_horLayDefaultButton = new QHBoxLayout();
	m_horSpacerDefaultButtonRight = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_horLayDefaultButton->addWidget(m_defaultColorButton);
	m_horLayDefaultButton->addSpacerItem(m_horSpacerDefaultButtonRight);

	m_horLayMoreColors = new QHBoxLayout();
	m_horSpacerMoreColorsRight = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_horLayMoreColors->addWidget(m_moreColorsButton);
	m_horLayMoreColors->addSpacerItem(m_horSpacerMoreColorsRight);

	m_vertLayMain->addLayout(m_horLayDefaultButton);
	m_vertLayMain->addLayout(m_colorGridLayout);
	m_vertLayMain->addLayout(m_horLayMoreColors);

	m_mainFrame = new QFrame(this);
	m_mainFrame->setLayout(m_vertLayMain);

	GradientList colorGradients = initialialzeColorGradients();
	m_columns = colorGradients.size();
	createColorsButtons();
	setButtonColors(colorGradients);

	m_moreColorsButton->setCursor(Qt::PointingHandCursor);
	m_moreColorsButton->setObjectName("m_moreColorsButton");
	m_moreColorsButton->setFlat(true);
	m_moreColorsButton->setIcon(QIcon(":/icons/more_colors_icon.png"));

	m_defaultColorButton->setCursor(Qt::PointingHandCursor);
	m_defaultColorButton->setObjectName("m_defaultColorButton");
	m_defaultColorButton->setFlat(true);
	QPixmap pixmap(13, 13);
	pixmap.fill(m_defaultColor);
	m_defaultColorButton->setIcon(QIcon(pixmap));
	m_defaultColorButton->setIconSize(QSize(16, 16));
}

void ColorPickerWidget::setupConnections()
{
	connect(m_defaultColorButton, &QPushButton::clicked, [this]
	{
		setCurrentColor(m_defaultColor);
	});
	connect(m_moreColorsButton, SIGNAL(clicked()), this, SLOT(openColorDialog()));
}

QColor ColorPickerWidget::interpolateColor(QColor colorStart, QColor colorEnd, float percent)
{
	double resultRedF = colorStart.redF() + percent * (colorEnd.redF() - colorStart.redF());
	double resultGreenF = colorStart.greenF() + percent * (colorEnd.greenF() - colorStart.greenF());
	double resultBlueF = colorStart.blueF() + percent * (colorEnd.blueF() - colorStart.blueF());
	QColor interpolatedColor;
	interpolatedColor.setRedF(resultRedF);
	interpolatedColor.setGreenF(resultGreenF);
	interpolatedColor.setBlueF(resultBlueF);
	return interpolatedColor;
}

GradientList ColorPickerWidget::initialialzeColorGradients()
{
	GradientList gradientList;
	gradientList.push_back(qMakePair(Qt::white, Qt::black)); // white         -> black
	gradientList.push_back(qMakePair(QColor(93, 218, 255), QColor(20, 113, 145))); // cyan          -> blue green
	gradientList.push_back(qMakePair(QColor(129, 182, 255), QColor(28, 51, 135))); // light blue    -> dark blue
	gradientList.push_back(qMakePair(QColor(186, 0, 255), QColor(65, 0, 125))); // magenta       -> violet
	gradientList.push_back(qMakePair(QColor(229, 0, 255), QColor(106, 0, 129))); // pink          -> wine
	gradientList.push_back(qMakePair(QColor(241, 79, 154), QColor(123, 0, 61))); // bright orange -> brown
	gradientList.push_back(qMakePair(QColor(255, 122, 116), QColor(173, 0, 0))); // orange        -> dark red
	gradientList.push_back(qMakePair(QColor(251, 205, 95), QColor(167, 69, 0))); // light orange  -> dark orange
	gradientList.push_back(qMakePair(QColor(245, 255, 125), QColor(147, 150, 0))); // light yellow  -> gray
	gradientList.push_back(qMakePair(QColor(150, 238, 125), QColor(50, 135, 18))); // lime green    -> dark green
	return gradientList;
}

void ColorPickerWidget::createColorsButtons()
{
	for (int row = 0; row < m_rows; ++row)
	{
		for (int column = 0; column < m_columns; ++column)
		{
			QPushButton* colorButton = new QPushButton("");
			colorButton->setCursor(Qt::PointingHandCursor);
			m_colorGridLayout->addWidget(colorButton, row, column);
			connect(colorButton, &QPushButton::clicked, this,
					[this, colorButton]()
					{
						setCurrentColor(colorButton->palette().button().color());
					}, Qt::DirectConnection);
		}
	}
}

void ColorPickerWidget::setButtonColors(GradientList gradientList)
{
	QColor colorStart;
	QColor colorEnd;
	for (int column = 0; column < gradientList.size(); ++column)
	{
		float percent = 0.0F;
		colorStart = gradientList.at(column).first;
		colorEnd = gradientList.at(column).second;
		for (int row = 0; row < m_rows; ++row)
		{
			QPushButton* colorButton = qobject_cast<QPushButton*>(
					m_colorGridLayout->itemAtPosition(row, column)->widget());
			colorButton->setFixedSize(QSize(15, 15));
			colorButton->setCursor(Qt::PointingHandCursor);
			QColor interpolatedColor = interpolateColor(colorStart, colorEnd, percent);
			colorButton->setStyleSheet(QString("QPushButton{"
											   "background-color: rgb(%0,%1,%2);"
											   "border : none;"
											   "}").arg(interpolatedColor.red()).arg(interpolatedColor.green()).arg(
					interpolatedColor.blue()));
			percent += 1.0F / (m_rows - 1);
		}
	}
}

void ColorPickerWidget::setCurrentColor(QColor color)
{
	close();
	m_currentColor = color;
	m_isColorSelected = true;
}

QColor ColorPickerWidget::getCurrentColor()
{
	return m_currentColor;
}

void ColorPickerWidget::openColorDialog()
{
	close();
	QColorDialog dialog(this);
	common::openDialog(&dialog, ":/styles/style.qss");
	setCurrentColor(dialog.selectedColor());
}

bool ColorPickerWidget::colorSelected() const
{
	return m_isColorSelected;
}
