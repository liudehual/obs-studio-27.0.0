#pragma once

#include <QProxyStyle>
/////////////////////// 设置程序风格 ///////////////////////
class OBSProxyStyle : public QProxyStyle {
public:
	QPixmap generatedIconPixmap(QIcon::Mode iconMode, const QPixmap &pixmap,
				    const QStyleOption *option) const override;
};
