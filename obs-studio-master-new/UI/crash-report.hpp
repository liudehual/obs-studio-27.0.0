#pragma once

#include <QDialog>

class QPlainTextEdit;

//////////////////////// 程序崩溃面板 //////////////////////////////
class OBSCrashReport : public QDialog {
	Q_OBJECT

	QPlainTextEdit *textBox;

public:
	OBSCrashReport(QWidget *parent, const char *text);

public slots:
	void ExitClicked();
	void CopyClicked();
};
