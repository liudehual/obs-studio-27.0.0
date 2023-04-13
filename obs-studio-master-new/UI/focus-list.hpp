#pragma once

#include <QListWidget>
// 滤镜界面列表
class FocusList : public QListWidget {
	Q_OBJECT

public:
	FocusList(QWidget *parent);

protected:
	void focusInEvent(QFocusEvent *event) override;

signals:
	void GotFocus();
};
