#pragma once

#include <QScrollArea>

class QResizeEvent;
// 混音器界面
class HScrollArea : public QScrollArea {
	Q_OBJECT

public:
	inline HScrollArea(QWidget *parent = nullptr) : QScrollArea(parent)
	{
		setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	}

protected:
	virtual void resizeEvent(QResizeEvent *event) override;
};
