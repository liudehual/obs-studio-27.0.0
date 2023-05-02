#pragma once

#include <QSlider>
#include "slider-ignorewheel.hpp"
////////////////////////////////////// 属性界面，添加float 型属性值滑块 /////////////////////////////////////
class DoubleSlider : public SliderIgnoreScroll {
	Q_OBJECT

	double minVal, maxVal, minStep;

public:
	DoubleSlider(QWidget *parent = nullptr);

	void setDoubleConstraints(double newMin, double newMax, double newStep,
				  double val);

signals:
	void doubleValChanged(double val);

public slots:
	void intValChanged(int val);
	void setDoubleVal(double val);
};
