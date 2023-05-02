#pragma once

#include <obs.hpp>
#include <memory>

#include "ui_OBSBasicSourceSelect.h"

class OBSBasic;
///////////////////////////// 添加源时弹出窗口 ///////////////////////////////////////
// 小窗口 新建或添加现有
class OBSBasicSourceSelect : public QDialog {
	Q_OBJECT

private:
	std::unique_ptr<Ui::OBSBasicSourceSelect> ui;
	const char *id;

	static bool EnumSources(void *data, obs_source_t *source);
	static bool EnumGroups(void *data, obs_source_t *source);

	static void OBSSourceRemoved(void *data, calldata_t *calldata);
	static void OBSSourceAdded(void *data, calldata_t *calldata);

private slots:
	void on_buttonBox_accepted();
	void on_buttonBox_rejected();

	void SourceAdded(OBSSource source);
	void SourceRemoved(OBSSource source);

public:
	OBSBasicSourceSelect(OBSBasic *parent, const char *id);

	OBSSource newSource;

	static void SourcePaste(const char *name, bool visible, bool duplicate);
};
