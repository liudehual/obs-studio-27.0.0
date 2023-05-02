
#pragma once

#include <QDialog>
#include <QDialogButtonBox>
#include <QPointer>
#include <QSplitter>
#include "qt-display.hpp"
#include <obs.hpp>

class OBSPropertiesView;
class OBSBasic;

///////////////////////////////////////////// 属性面板 ////////////////////////////
class OBSBasicProperties : public QDialog {
	Q_OBJECT

private:
	QPointer<OBSQTDisplay> preview;

	OBSBasic *main;
	bool acceptClicked;

	OBSSource source;
	OBSSignal removedSignal;
	OBSSignal renamedSignal;
	OBSSignal updatePropertiesSignal;
	OBSData oldSettings;
	OBSPropertiesView *view;
	QDialogButtonBox *buttonBox;
	QSplitter *windowSplitter;

	OBSSource sourceA;
	OBSSource sourceB;
	OBSSource sourceClone;
	bool direction = true;

	static void SourceRemoved(void *data, calldata_t *params);
	static void SourceRenamed(void *data, calldata_t *params);
	static void UpdateProperties(void *data, calldata_t *params);
	static void DrawPreview(void *data, uint32_t cx, uint32_t cy);
	static void DrawTransitionPreview(void *data, uint32_t cx, uint32_t cy);
	void UpdateCallback(void *obj, obs_data_t *settings);
	bool ConfirmQuit();
	int CheckSettings();
	void Cleanup();

private slots:
	void on_buttonBox_clicked(QAbstractButton *button);
	void AddPreviewButton();

public:
	OBSBasicProperties(QWidget *parent, OBSSource source_);
	~OBSBasicProperties();

	void Init();

protected:
	virtual void closeEvent(QCloseEvent *event) override;
	virtual void reject() override;
};
