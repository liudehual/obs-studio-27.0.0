#pragma once

#include <QSpinBox>
#include <QInputEvent>
#include <QtCore/QObject>

/*
	QSpinBox是一个计数器控件，允许用户选择一个整数值，通过单击向上/向下按钮或按键盘上的上/下箭头来增加/减少当前显示的值，当然用户也可以输入值。
*/
///// 属性界面等中使用
class SpinBoxIgnoreScroll : public QSpinBox {
	Q_OBJECT

public:
	SpinBoxIgnoreScroll(QWidget *parent = nullptr);

protected:
	virtual void wheelEvent(QWheelEvent *event) override;
};
