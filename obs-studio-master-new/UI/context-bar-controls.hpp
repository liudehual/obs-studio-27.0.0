#pragma once

#include <memory>
#include <obs.hpp>
#include <QWidget>

class Ui_BrowserSourceToolbar;
class Ui_DeviceSelectToolbar;
class Ui_GameCaptureToolbar;
class Ui_ImageSourceToolbar;
class Ui_ColorSourceToolbar;
class Ui_TextSourceToolbar;
/////////////////////////////////////////////////////////// 部分源控制条 /////////////////////////////
class SourceToolbar : public QWidget {
	Q_OBJECT

	OBSWeakSource weakSource;

protected:
	using properties_delete_t = decltype(&obs_properties_destroy);
	using properties_t =
		std::unique_ptr<obs_properties_t, properties_delete_t>;

	properties_t props;

public:
	SourceToolbar(QWidget *parent, OBSSource source);

	OBSSource GetSource() { return OBSGetStrongRef(weakSource); }

public slots:
	virtual void Update() {}
};
/// <summary>
/// //////////////////// 浏览器源控制条 ///////////////////////////
/// </summary>
class BrowserToolbar : public SourceToolbar {
	Q_OBJECT

	Ui_BrowserSourceToolbar *ui;

public:
	BrowserToolbar(QWidget *parent, OBSSource source);
	~BrowserToolbar();

public slots:
	void on_refresh_clicked();
};
// 组合选择工具栏
class ComboSelectToolbar : public SourceToolbar {
	Q_OBJECT

protected:
	Ui_DeviceSelectToolbar *ui;
	const char *prop_name;
	bool is_int = false;

public:
	ComboSelectToolbar(QWidget *parent, OBSSource source);
	~ComboSelectToolbar();
	virtual void Init();

public slots:
	void on_device_currentIndexChanged(int idx);
};
// 音频采集工具条
class AudioCaptureToolbar : public ComboSelectToolbar {
	Q_OBJECT

public:
	AudioCaptureToolbar(QWidget *parent, OBSSource source);
	void Init() override;
};
// 窗口采集工具条
class WindowCaptureToolbar : public ComboSelectToolbar {
	Q_OBJECT

public:
	WindowCaptureToolbar(QWidget *parent, OBSSource source);
	void Init() override;
};
// 显示器采集工具条
class DisplayCaptureToolbar : public ComboSelectToolbar {
	Q_OBJECT

public:
	DisplayCaptureToolbar(QWidget *parent, OBSSource source);
	void Init() override;
};
// 设备采集工具条
class DeviceCaptureToolbar : public QWidget {
	Q_OBJECT

	OBSWeakSource weakSource;

	Ui_DeviceSelectToolbar *ui;
	const char *activateText;
	const char *deactivateText;
	bool active;

public:
	DeviceCaptureToolbar(QWidget *parent, OBSSource source);
	~DeviceCaptureToolbar();

public slots:
	void on_activateButton_clicked();
};
// 游戏窗口源工具条
class GameCaptureToolbar : public SourceToolbar {
	Q_OBJECT

	Ui_GameCaptureToolbar *ui;

	void UpdateWindowVisibility();

public:
	GameCaptureToolbar(QWidget *parent, OBSSource source);
	~GameCaptureToolbar();

public slots:
	void on_mode_currentIndexChanged(int idx);
	void on_window_currentIndexChanged(int idx);
};
// 图片源工具条
class ImageSourceToolbar : public SourceToolbar {
	Q_OBJECT

	Ui_ImageSourceToolbar *ui;

public:
	ImageSourceToolbar(QWidget *parent, OBSSource source);
	~ImageSourceToolbar();

public slots:
	void on_browse_clicked();
};
// 颜色源工具条
class ColorSourceToolbar : public SourceToolbar {
	Q_OBJECT

	Ui_ColorSourceToolbar *ui;
	QColor color;

	void UpdateColor();

public:
	ColorSourceToolbar(QWidget *parent, OBSSource source);
	~ColorSourceToolbar();

public slots:
	void on_choose_clicked();
};
// 文本源工具条
class TextSourceToolbar : public SourceToolbar {
	Q_OBJECT

	Ui_TextSourceToolbar *ui;
	QFont font;
	QColor color;

public:
	TextSourceToolbar(QWidget *parent, OBSSource source);
	~TextSourceToolbar();

public slots:
	void on_selectFont_clicked();
	void on_selectColor_clicked();
	void on_text_textChanged();
};
