#pragma once

#include <QPushButton>
//////////////////////////////////////// 录像按钮 /////////////////////////////////////////////
class RecordButton : public QPushButton {
	Q_OBJECT

public:
	inline RecordButton(QWidget *parent = nullptr) : QPushButton(parent) {}

	virtual void resizeEvent(QResizeEvent *event) override;
};
//////////////////////////////////////// 回放缓存按钮 /////////////////////////////////////////
class ReplayBufferButton : public QPushButton {
	Q_OBJECT

public:
	inline ReplayBufferButton(const QString &text,
				  QWidget *parent = nullptr)
		: QPushButton(text, parent)
	{
	}

	virtual void resizeEvent(QResizeEvent *event) override;
};
