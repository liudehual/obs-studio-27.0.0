
#pragma once

#include "obs-app.hpp"
#include "window-basic-main.hpp"
#include <QPointer>
#include <QStyledItemDelegate>
#include <QFileInfo>
#include "ui_OBSImporter.h"

class ImporterModel;
/////////////////////////////////////////////// 导入窗口 ////////////////////////////////////////////
// 场景集合-导入
//// 
class OBSImporter : public QDialog {
	Q_OBJECT

	QPointer<ImporterModel> optionsModel;
	std::unique_ptr<Ui::OBSImporter> ui;

public:
	explicit OBSImporter(QWidget *parent = nullptr);

	void addImportOption(QString path, bool automatic);

protected:
	virtual void dropEvent(QDropEvent *ev) override;
	virtual void dragEnterEvent(QDragEnterEvent *ev) override;

public slots:
	void browseImport();
	void importCollections();
	void dataChanged();
};

class ImporterModel : public QAbstractTableModel {
	Q_OBJECT

	friend class OBSImporter;

public:
	ImporterModel(QObject *parent = 0) : QAbstractTableModel(parent) {}

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role) const;
	QVariant headerData(int section, Qt::Orientation orientation,
			    int role = Qt::DisplayRole) const;
	Qt::ItemFlags flags(const QModelIndex &index) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role);

private:
	struct ImporterEntry {
		QString path;
		QString program;
		QString name;

		bool selected;
		bool empty;
	};

	QList<ImporterEntry> options;

	void checkInputPath(int row);
};

class ImporterEntryPathItemDelegate : public QStyledItemDelegate {
	Q_OBJECT

public:
	ImporterEntryPathItemDelegate();

	virtual QWidget *createEditor(QWidget *parent,
				      const QStyleOptionViewItem & /* option */,
				      const QModelIndex &index) const override;

	virtual void setEditorData(QWidget *editor,
				   const QModelIndex &index) const override;
	virtual void setModelData(QWidget *editor, QAbstractItemModel *model,
				  const QModelIndex &index) const override;
	virtual void paint(QPainter *painter,
			   const QStyleOptionViewItem &option,
			   const QModelIndex &index) const override;

private:
	const char *PATH_LIST_PROP = "pathList";

	void handleBrowse(QWidget *container);
	void handleClear(QWidget *container);

private slots:
	void updateText();
};
