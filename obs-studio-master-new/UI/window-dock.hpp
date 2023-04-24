#pragma once

#include <QDockWidget>
/////////////////////////////////// 浮动窗口  //////////////////////////////////////////////////////
///////////////////////////////// 支持浮动的窗口需要作为子部件加入到该部件中或继承该部件////////////////////////
class OBSDock : public QDockWidget {
	Q_OBJECT

public:
	inline OBSDock(QWidget *parent = nullptr) : QDockWidget(parent) {}

	virtual void closeEvent(QCloseEvent *event);
};
