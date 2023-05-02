#include <QObject>
#include <string>
#include <thread>
#include <obs.hpp>

////////////////////////////////// 截图功能 /////////////////////////////////////////
class ScreenshotObj : public QObject {
	Q_OBJECT

public:
	ScreenshotObj(obs_source_t *source);
	~ScreenshotObj() override;
	void Screenshot();
	void Download();
	void Copy();
	void MuxAndFinish();

	gs_texrender_t *texrender = nullptr;
	gs_stagesurf_t *stagesurf = nullptr;
	OBSWeakSource weakSource;
	std::string path;
	QImage image;
	uint32_t cx;
	uint32_t cy;
	std::thread th;

	int stage = 0;

public slots:
	void Save();
};
