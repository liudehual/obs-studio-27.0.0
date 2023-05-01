#pragma once

#include <QSlider>
#include <QMouseEvent>
///////////////////////// 滑动条 ///////////////////////////////////////
// 主要用于高级音频属性面板
class BalanceSlider : public QSlider {
	Q_OBJECT

public:
	inline BalanceSlider(QWidget *parent = 0) : QSlider(parent) {}

signals:
	void doubleClicked();

protected:
	void mouseDoubleClickEvent(QMouseEvent *event)
	{
		emit doubleClicked();
		event->accept();
	}
};
