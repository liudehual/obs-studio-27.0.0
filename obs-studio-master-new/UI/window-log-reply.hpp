
#pragma once

#include <memory>
#include "ui_OBSLogReply.h"

//////////////////// 日志上传成功/失败面板 ///////////////////////////
class OBSLogReply : public QDialog {
	Q_OBJECT

private:
	std::unique_ptr<Ui::OBSLogReply> ui;

public:
	OBSLogReply(QWidget *parent, const QString &url, const bool crash);

private slots:
	void on_copyURL_clicked();
	void on_analyzeURL_clicked();
};
