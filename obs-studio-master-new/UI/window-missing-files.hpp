
#pragma once

#include <QPointer>
#include <QStyledItemDelegate>
#include "obs-app.hpp"
#include "ui_OBSMissingFiles.h"

class MissingFilesModel;

enum MissingFilesState { Missing, Found, Replaced, Cleared };
Q_DECLARE_METATYPE(MissingFilesState);
/// <summary>
/// ///////////////////////////////////////////////////////////// 文件丢失 ///////////////////////////////////////////
/// </summary>
class OBSMissingFiles : public QDialog {
	Q_OBJECT
	Q_PROPERTY(QIcon warningIcon READ GetWarningIcon WRITE SetWarningIcon
			   DESIGNABLE true)

	QPointer<MissingFilesModel> filesModel;
	std::unique_ptr<Ui::OBSMissingFiles> ui;

public:
	explicit OBSMissingFiles(obs_missing_files_t *files,
				 QWidget *parent = nullptr);
	virtual ~OBSMissingFiles() override;

	void addMissingFile(const char *originalPath, const char *sourceName);

	QIcon GetWarningIcon();
	void SetWarningIcon(const QIcon &icon);

private:
	void saveFiles();
	void browseFolders();

	obs_missing_files_t *fileStore;

public slots:
	void dataChanged();
};

class MissingFilesModel : public QAbstractTableModel {
	Q_OBJECT

	friend class OBSMissingFiles;

public:
	explicit MissingFilesModel(QObject *parent = 0);

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	int found() const;
	QVariant data(const QModelIndex &index, int role) const;
	QVariant headerData(int section, Qt::Orientation orientation,
			    int role = Qt::DisplayRole) const;
	Qt::ItemFlags flags(const QModelIndex &index) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role);

	bool loop = true;

	QIcon warningIcon;

private:
	struct MissingFileEntry {
		MissingFilesState state = MissingFilesState::Missing;

		QString source;

		QString originalPath;
		QString newPath;
	};

	QList<MissingFileEntry> files;

	void fileCheckLoop(QList<MissingFileEntry> files, QString path,
			   bool skipPrompt);
};

class MissingFilesPathItemDelegate : public QStyledItemDelegate {
	Q_OBJECT

public:
	MissingFilesPathItemDelegate(bool isOutput, const QString &defaultPath);

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
	bool isOutput;
	QString defaultPath;
	const char *PATH_LIST_PROP = "pathList";

	void handleBrowse(QWidget *container);
	void handleClear(QWidget *container);
};
