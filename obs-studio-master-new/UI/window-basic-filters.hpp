#pragma once

#include <QDialog>
#include <QDialogButtonBox>
#include <memory>
#include <obs.hpp>

#include "properties-view.hpp"

class OBSBasic;
class QMenu;

#include "ui_OBSBasicFilters.h"

////////////////////////////// 滤镜界面 ///////////////////////////////////////////
class OBSBasicFilters : public QDialog {
	Q_OBJECT

private:
	OBSBasic *main;

	std::unique_ptr<Ui::OBSBasicFilters> ui;
	OBSSource source;
	OBSPropertiesView *view = nullptr;

	OBSSignal addSignal;
	OBSSignal removeSignal;
	OBSSignal reorderSignal;

	OBSSignal removeSourceSignal;
	OBSSignal renameSourceSignal;
	OBSSignal updatePropertiesSignal;

	inline OBSSource GetFilter(int row, bool async);

	void UpdateFilters();
	void UpdatePropertiesView(int row, bool async);

	static void OBSSourceFilterAdded(void *param, calldata_t *data);
	static void OBSSourceFilterRemoved(void *param, calldata_t *data);
	static void OBSSourceReordered(void *param, calldata_t *data);
	static void SourceRemoved(void *param, calldata_t *data);
	static void SourceRenamed(void *param, calldata_t *data);
	static void UpdateProperties(void *data, calldata_t *params);
	static void DrawPreview(void *data, uint32_t cx, uint32_t cy);

	QMenu *CreateAddFilterPopupMenu(bool async);

	void AddNewFilter(const char *id);
	void ReorderFilter(QListWidget *list, obs_source_t *filter, size_t idx);

	void CustomContextMenu(const QPoint &pos, bool async);
	void EditItem(QListWidgetItem *item, bool async);
	void DuplicateItem(QListWidgetItem *item);

	void FilterNameEdited(QWidget *editor, QListWidget *list);

	bool isAsync;

	int noPreviewMargin;

	bool editActive = false;

private slots:
	void AddFilter(OBSSource filter, bool focus = true);
	void RemoveFilter(OBSSource filter);
	void ReorderFilters();
	void RenameAsyncFilter();
	void RenameEffectFilter();
	void DuplicateAsyncFilter();
	void DuplicateEffectFilter();
	void ResetFilters();

	void AddFilterFromAction();

	void on_addAsyncFilter_clicked();
	void on_removeAsyncFilter_clicked();
	void on_moveAsyncFilterUp_clicked();
	void on_moveAsyncFilterDown_clicked();
	void on_asyncFilters_currentRowChanged(int row);
	void on_asyncFilters_customContextMenuRequested(const QPoint &pos);
	void on_asyncFilters_GotFocus();

	void on_addEffectFilter_clicked();
	void on_removeEffectFilter_clicked();
	void on_moveEffectFilterUp_clicked();
	void on_moveEffectFilterDown_clicked();
	void on_effectFilters_currentRowChanged(int row);
	void on_effectFilters_customContextMenuRequested(const QPoint &pos);
	void on_effectFilters_GotFocus();

	void on_actionRemoveFilter_triggered();
	void on_actionMoveUp_triggered();
	void on_actionMoveDown_triggered();

	void AsyncFilterNameEdited(QWidget *editor,
				   QAbstractItemDelegate::EndEditHint endHint);
	void EffectFilterNameEdited(QWidget *editor,
				    QAbstractItemDelegate::EndEditHint endHint);

	void CopyFilter();
	void PasteFilter();

public:
	OBSBasicFilters(QWidget *parent, OBSSource source_);
	~OBSBasicFilters();

	void Init();

protected:
	virtual void closeEvent(QCloseEvent *event) override;
};
