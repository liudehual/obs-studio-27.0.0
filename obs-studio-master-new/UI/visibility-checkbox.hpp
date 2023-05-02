#pragma once

#include <QCheckBox>

//////////////////////// 可见性复选框,源列表/滤镜列表,源列表中条目中眼睛图标 ////////////////////////////////
class VisibilityCheckBox : public QCheckBox {
	Q_OBJECT

public:
	VisibilityCheckBox();
	explicit VisibilityCheckBox(QWidget *parent);
};
