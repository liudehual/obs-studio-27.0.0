#pragma once

#include <QThread>
#include <vector>
#include <string>
////////////////////////////////// 关于面板 获取开发人员列表 ///////////////////////////////////
class RemoteTextThread : public QThread {
	Q_OBJECT

	std::string url;
	std::string contentType;
	std::string postData;

	std::vector<std::string> extraHeaders;

	int timeoutSec = 0;

	void run() override;

signals:
	void Result(const QString &text, const QString &error);

public:
	inline RemoteTextThread(std::string url_,
				std::string contentType_ = std::string(),
				std::string postData_ = std::string(),
				int timeoutSec_ = 0)
		: url(url_),
		  contentType(contentType_),
		  postData(postData_),
		  timeoutSec(timeoutSec_)
	{
	}

	inline RemoteTextThread(std::string url_,
				std::vector<std::string> &&extraHeaders_,
				std::string contentType_ = std::string(),
				std::string postData_ = std::string(),
				int timeoutSec_ = 0)
		: url(url_),
		  contentType(contentType_),
		  postData(postData_),
		  extraHeaders(std::move(extraHeaders_)),
		  timeoutSec(timeoutSec_)
	{
	}
};

bool GetRemoteFile(
	const char *url, std::string &str, std::string &error,
	long *responseCode = nullptr, const char *contentType = nullptr,
	const char *postData = nullptr,
	std::vector<std::string> extraHeaders = std::vector<std::string>(),
	std::string *signature = nullptr, int timeoutSec = 0);
