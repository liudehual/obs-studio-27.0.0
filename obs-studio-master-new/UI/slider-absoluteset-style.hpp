#pragma once

#include <QProxyStyle>
///////////////////////////////滑块风格设置 //////////////////////////////////////////
// 视频源 进度条滑块风格
// 混音器界面 音频滑块风格
class SliderAbsoluteSetStyle : public QProxyStyle {
public:
	SliderAbsoluteSetStyle(const QString &baseStyle);
	SliderAbsoluteSetStyle(QStyle *baseStyle = Q_NULLPTR);
	int styleHint(QStyle::StyleHint hint, const QStyleOption *option,
		      const QWidget *widget,
		      QStyleHintReturn *returnData) const;
};
