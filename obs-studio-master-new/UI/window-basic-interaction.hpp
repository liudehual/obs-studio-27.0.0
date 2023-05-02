
#pragma once

#include <QDialog>
#include <memory>
#include <functional>

#include <obs.hpp>

#include "properties-view.hpp"

class OBSBasic;

#include "ui_OBSBasicInteraction.h"

class OBSEventFilter;

class OBSBasicInteraction : public QDialog {
	Q_OBJECT

private:
	OBSBasic *main;

	std::unique_ptr<Ui::OBSBasicInteraction> ui;
	OBSSource source;
	OBSSignal removedSignal;
	OBSSignal renamedSignal;
	std::unique_ptr<OBSEventFilter> eventFilter;

	static void SourceRemoved(void *data, calldata_t *params);
	static void SourceRenamed(void *data, calldata_t *params);
	static void DrawPreview(void *data, uint32_t cx, uint32_t cy);

	bool GetSourceRelativeXY(int mouseX, int mouseY, int &x, int &y);

	bool HandleMouseClickEvent(QMouseEvent *event);
	bool HandleMouseMoveEvent(QMouseEvent *event);
	bool HandleMouseWheelEvent(QWheelEvent *event);
	bool HandleFocusEvent(QFocusEvent *event);
	bool HandleKeyEvent(QKeyEvent *event);

	OBSEventFilter *BuildEventFilter();

public:
	OBSBasicInteraction(QWidget *parent, OBSSource source_);
	~OBSBasicInteraction();

	void Init();

protected:
	virtual void closeEvent(QCloseEvent *event) override;
};

typedef std::function<bool(QObject *, QEvent *)> EventFilterFunc;

class OBSEventFilter : public QObject {
	Q_OBJECT
public:
	OBSEventFilter(EventFilterFunc filter_) : filter(filter_) {}

protected:
	bool eventFilter(QObject *obj, QEvent *event)
	{
		return filter(obj, event);
	}

public:
	EventFilterFunc filter;
};
