/******************************************************************************
    Copyright (C) 2013-2014 by Hugh Bailey <obs.jim@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#pragma once

#include <QBuffer>
#include <QAction>
#include <QWidgetAction>
#include <QSystemTrayIcon>
#include <QStyledItemDelegate>
#include <obs.hpp>
#include <vector>
#include <memory>
#include "window-main.hpp"
#include "window-basic-interaction.hpp"
#include "window-basic-properties.hpp"
#include "window-basic-transform.hpp"
#include "window-basic-adv-audio.hpp"
#include "window-basic-filters.hpp"
#include "window-projector.hpp"
#include "window-basic-about.hpp"
#include "auth-base.hpp"
#include "log-viewer.hpp"

#include <obs-frontend-internal.hpp>

#include <util/platform.h>
#include <util/threading.h>
#include <util/util.hpp>

#include <QPointer>

class QMessageBox;
class QListWidgetItem;
class VolControl;
class OBSBasicStats;

#include "ui_OBSBasic.h"
#include "ui_ColorSelect.h"

#define DESKTOP_AUDIO_1 Str("DesktopAudioDevice1")
#define DESKTOP_AUDIO_2 Str("DesktopAudioDevice2")
#define AUX_AUDIO_1 Str("AuxAudioDevice1")
#define AUX_AUDIO_2 Str("AuxAudioDevice2")
#define AUX_AUDIO_3 Str("AuxAudioDevice3")
#define AUX_AUDIO_4 Str("AuxAudioDevice4")

#define SIMPLE_ENCODER_X264 "x264"
#define SIMPLE_ENCODER_X264_LOWCPU "x264_lowcpu"
#define SIMPLE_ENCODER_QSV "qsv"
#define SIMPLE_ENCODER_NVENC "nvenc"
#define SIMPLE_ENCODER_AMD "amd"

#define PREVIEW_EDGE_SIZE 10

struct BasicOutputHandler;

enum class QtDataRole {
	OBSRef = Qt::UserRole,
	OBSSignals,
};

struct SavedProjectorInfo {
	ProjectorType type;
	int monitor;
	std::string geometry;
	std::string name;
	bool alwaysOnTop;
	bool alwaysOnTopOverridden;
};

struct QuickTransition {
	QPushButton *button = nullptr;
	OBSSource source;
	obs_hotkey_id hotkey = OBS_INVALID_HOTKEY_ID;
	int duration = 0;
	int id = 0;
	bool fadeToBlack = false;

	inline QuickTransition() {}
	inline QuickTransition(OBSSource source_, int duration_, int id_,
			       bool fadeToBlack_ = false)
		: source(source_),
		  duration(duration_),
		  id(id_),
		  fadeToBlack(fadeToBlack_),
		  renamedSignal(std::make_shared<OBSSignal>(
			  obs_source_get_signal_handler(source), "rename",
			  SourceRenamed, this))
	{
	}

private:
	static void SourceRenamed(void *param, calldata_t *data);
	std::shared_ptr<OBSSignal> renamedSignal;
};

class ColorSelect : public QWidget {

public:
	explicit ColorSelect(QWidget *parent = 0);

private:
	std::unique_ptr<Ui::ColorSelect> ui;
};

class OBSBasic : public OBSMainWindow {
	Q_OBJECT
	Q_PROPERTY(QIcon imageIcon READ GetImageIcon WRITE SetImageIcon DESIGNABLE true)
	Q_PROPERTY(QIcon colorIcon READ GetColorIcon WRITE SetColorIcon DESIGNABLE true)
	Q_PROPERTY(QIcon slideshowIcon READ GetSlideshowIcon WRITE SetSlideshowIcon DESIGNABLE true)
	Q_PROPERTY(QIcon audioInputIcon READ GetAudioInputIcon WRITE SetAudioInputIcon DESIGNABLE true)
	Q_PROPERTY(QIcon audioOutputIcon READ GetAudioOutputIcon WRITE SetAudioOutputIcon DESIGNABLE true)
	Q_PROPERTY(QIcon desktopCapIcon READ GetDesktopCapIcon WRITE SetDesktopCapIcon DESIGNABLE true)
	Q_PROPERTY(QIcon windowCapIcon READ GetWindowCapIcon WRITE SetWindowCapIcon DESIGNABLE true)
	Q_PROPERTY(QIcon gameCapIcon READ GetGameCapIcon WRITE SetGameCapIcon DESIGNABLE true)
	Q_PROPERTY(QIcon cameraIcon READ GetCameraIcon WRITE SetCameraIcon DESIGNABLE true)
	Q_PROPERTY(QIcon textIcon READ GetTextIcon WRITE SetTextIcon DESIGNABLE true)
	Q_PROPERTY(QIcon mediaIcon READ GetMediaIcon WRITE SetMediaIcon DESIGNABLE true)
	Q_PROPERTY(QIcon browserIcon READ GetBrowserIcon WRITE SetBrowserIcon DESIGNABLE true)
	Q_PROPERTY(QIcon groupIcon READ GetGroupIcon WRITE SetGroupIcon DESIGNABLE true)
	Q_PROPERTY(QIcon sceneIcon READ GetSceneIcon WRITE SetSceneIcon DESIGNABLE true)
	Q_PROPERTY(QIcon defaultIcon READ GetDefaultIcon WRITE SetDefaultIcon DESIGNABLE true)

	friend class OBSAbout;
	friend class OBSBasicPreview;
	friend class OBSBasicStatusBar;
	friend class OBSBasicSourceSelect;
	friend class OBSBasicSettings;
	friend class Auth;
	friend class AutoConfig;
	friend class AutoConfigStreamPage;
	friend class RecordButton;
	friend class ReplayBufferButton;
	friend class ExtraBrowsersModel;
	friend class ExtraBrowsersDelegate;
	friend class DeviceCaptureToolbar;
	friend class DeviceToolbarPropertiesThread;
	friend struct BasicOutputHandler;
	friend struct OBSStudioAPI;

	enum class MoveDir { Up, Down, Left, Right };

	enum DropType {
		DropType_RawText,
		DropType_Text,
		DropType_Image,
		DropType_Media,
		DropType_Html,
		DropType_Url,
	};

private:
	obs_frontend_callbacks *api = nullptr;

	std::shared_ptr<Auth> auth;

	std::vector<VolControl *> volumes;

	std::vector<OBSSignal> signalHandlers;

	QList<QPointer<QDockWidget>> extraDocks;

	bool loaded = false;
	long disableSaving = 1;
	bool projectChanged = false;
	bool previewEnabled = true;

	std::list<const char *> copyStrings;
	const char *copyFiltersString = nullptr;
	bool copyVisible = true;

	bool closing = false;
	QScopedPointer<QThread> devicePropertiesThread;
	QScopedPointer<QThread> whatsNewInitThread;
	QScopedPointer<QThread> updateCheckThread;
	QScopedPointer<QThread> introCheckThread;
	QScopedPointer<QThread> logUploadThread;

	QPointer<OBSBasicInteraction> interaction;
	QPointer<OBSBasicProperties> properties;
	QPointer<OBSBasicTransform> transformWindow;
	QPointer<OBSBasicAdvAudio> advAudioWindow;
	QPointer<OBSBasicFilters> filters;
	QPointer<QDockWidget> statsDock;
	QPointer<OBSAbout> about;

	OBSLogViewer *logView = nullptr;

	QPointer<QTimer> cpuUsageTimer;
	QPointer<QTimer> diskFullTimer;

	os_cpu_usage_info_t *cpuUsageInfo = nullptr;

	OBSService service;
	std::unique_ptr<BasicOutputHandler> outputHandler;
	bool streamingStopping = false;
	bool recordingStopping = false;
	bool replayBufferStopping = false;

	gs_vertbuffer_t *box = nullptr;
	gs_vertbuffer_t *boxLeft = nullptr;
	gs_vertbuffer_t *boxTop = nullptr;
	gs_vertbuffer_t *boxRight = nullptr;
	gs_vertbuffer_t *boxBottom = nullptr;
	gs_vertbuffer_t *circle = nullptr;

	bool sceneChanging = false;

	int previewX = 0, previewY = 0;
	int previewCX = 0, previewCY = 0;
	float previewScale = 0.0f;

	ConfigFile basicConfig;// 配置  C:\Users\Administrator\AppData\Roaming\obs-studio\basic\profiles\未命名\basic.ini 配置文件中的内容

	std::vector<SavedProjectorInfo *> savedProjectorsArray;
	std::vector<OBSProjector *> projectors;

	QPointer<QWidget> stats;
	QPointer<QWidget> remux;
	QPointer<QWidget> extraBrowsers;
	QPointer<QWidget> importer;

	QPointer<QMenu> startStreamMenu;

	QPointer<QPushButton> transitionButton;
	QPointer<QPushButton> replayBufferButton;
	QPointer<QHBoxLayout> replayLayout;
	QScopedPointer<QPushButton> pause;
	QScopedPointer<QPushButton> replay;

	QPointer<QPushButton> vcamButton;
	bool vcamEnabled = false;

	QScopedPointer<QSystemTrayIcon> trayIcon;
	QPointer<QAction> sysTrayStream;
	QPointer<QAction> sysTrayRecord;
	QPointer<QAction> sysTrayReplayBuffer;
	QPointer<QAction> sysTrayVirtualCam;
	QPointer<QAction> showHide;
	QPointer<QAction> exit;
	QPointer<QMenu> trayMenu;
	QPointer<QMenu> previewProjector;
	QPointer<QMenu> studioProgramProjector;
	QPointer<QMenu> multiviewProjectorMenu;
	QPointer<QMenu> previewProjectorSource;
	QPointer<QMenu> previewProjectorMain;
	QPointer<QMenu> sceneProjectorMenu;
	QPointer<QMenu> sourceProjector;
	QPointer<QMenu> scaleFilteringMenu;
	QPointer<QMenu> colorMenu;
	QPointer<QWidgetAction> colorWidgetAction;
	QPointer<ColorSelect> colorSelect;
	QPointer<QMenu> deinterlaceMenu;
	QPointer<QMenu> perSceneTransitionMenu;
	QPointer<QObject> shortcutFilter;
	QPointer<QAction> renameScene;
	QPointer<QAction> renameSource;

	QPointer<QWidget> programWidget;
	QPointer<QVBoxLayout> programLayout;
	QPointer<QLabel> programLabel;

	QScopedPointer<QThread> patronJsonThread;
	std::string patronJson;

	void UpdateMultiviewProjectorMenu();

	void DrawBackdrop(float cx, float cy);

	void SetupEncoders();

	void CreateFirstRunSources();
	void CreateDefaultScene(bool firstStart);

	void UpdateVolumeControlsDecayRate();
	void UpdateVolumeControlsPeakMeterType();
	void ClearVolumeControls();

	void UploadLog(const char *subdir, const char *file, const bool crash);

	void Save(const char *file);
	void Load(const char *file);

	void InitHotkeys();
	void CreateHotkeys();
	void ClearHotkeys();

	bool InitService();

	bool InitBasicConfigDefaults();
	void InitBasicConfigDefaults2();
	bool InitBasicConfig();

	void InitOBSCallbacks();

	void InitPrimitives();

	void OnFirstLoad();

	OBSSceneItem GetSceneItem(QListWidgetItem *item);
	OBSSceneItem GetCurrentSceneItem();

	bool QueryRemoveSource(obs_source_t *source);

	void TimedCheckForUpdates();
	void CheckForUpdates(bool manualUpdate);

	void GetFPSCommon(uint32_t &num, uint32_t &den) const;
	void GetFPSInteger(uint32_t &num, uint32_t &den) const;
	void GetFPSFraction(uint32_t &num, uint32_t &den) const;
	void GetFPSNanoseconds(uint32_t &num, uint32_t &den) const;
	void GetConfigFPS(uint32_t &num, uint32_t &den) const;

	void UpdatePreviewScalingMenu();

	void LoadSceneListOrder(obs_data_array_t *array);
	obs_data_array_t *SaveSceneListOrder();
	void ChangeSceneIndex(bool relative, int idx, int invalidIdx);

	void TempFileOutput(const char *path, int vBitrate, int aBitrate);
	void TempStreamOutput(const char *url, const char *key, int vBitrate,int aBitrate);

	void CloseDialogs();
	void ClearSceneData();
	void ClearProjectors();

	void Nudge(int dist, MoveDir dir);

	OBSProjector *OpenProjector(obs_source_t *source, int monitor,ProjectorType type);

	void GetAudioSourceFilters();
	void GetAudioSourceProperties();
	void VolControlContextMenu();
	void ToggleVolControlLayout();
	void ToggleMixerLayout(bool vertical);

	void RefreshSceneCollections();
	void ChangeSceneCollection();
	void LogScenes();

	void LoadProfile();
	void ResetProfileData();
	bool AddProfile(bool create_new, const char *title, const char *text,const char *init_text = nullptr, bool rename = false);
	void DeleteProfile(const char *profile_name, const char *profile_dir);
	void RefreshProfiles();
	void ChangeProfile();
	void CheckForSimpleModeX264Fallback();

	void SaveProjectNow();

	int GetTopSelectedSourceItem();

	QModelIndexList GetAllSelectedSourceItems();

	obs_hotkey_pair_id streamingHotkeys, recordingHotkeys, pauseHotkeys,replayBufHotkeys, vcamHotkeys, togglePreviewHotkeys,contextBarHotkeys;
	obs_hotkey_id forceStreamingStopHotkey;

	void InitDefaultTransitions();
	void InitTransition(obs_source_t *transition);
	obs_source_t *FindTransition(const char *name);
	OBSSource GetCurrentTransition();
	obs_data_array_t *SaveTransitions();
	void LoadTransitions(obs_data_array_t *transitions);

	obs_source_t *fadeTransition;
	obs_source_t *cutTransition;

	void CreateProgramDisplay();
	void CreateProgramOptions();
	int TransitionCount();
	int AddTransitionBeforeSeparator(const QString &name,
					 obs_source_t *source);
	void AddQuickTransitionId(int id);
	void AddQuickTransition();
	void AddQuickTransitionHotkey(QuickTransition *qt);
	void RemoveQuickTransitionHotkey(QuickTransition *qt);
	void LoadQuickTransitions(obs_data_array_t *array);
	obs_data_array_t *SaveQuickTransitions();
	void ClearQuickTransitionWidgets();
	void RefreshQuickTransitions();
	void DisableQuickTransitionWidgets();
	void EnableTransitionWidgets(bool enable);
	void CreateDefaultQuickTransitions();

	QMenu *CreatePerSceneTransitionMenu();

	QuickTransition *GetQuickTransition(int id);
	int GetQuickTransitionIdx(int id);
	QMenu *CreateTransitionMenu(QWidget *parent, QuickTransition *qt);
	void ClearQuickTransitions();
	void QuickTransitionClicked();
	void QuickTransitionChange();
	void QuickTransitionChangeDuration(int value);
	void QuickTransitionRemoveClicked();

	void SetPreviewProgramMode(bool enabled);
	void ResizeProgram(uint32_t cx, uint32_t cy);
	void SetCurrentScene(obs_scene_t *scene, bool force = false);
	static void RenderProgram(void *data, uint32_t cx, uint32_t cy);

	std::vector<QuickTransition> quickTransitions;
	QPointer<QWidget> programOptions;
	QPointer<OBSQTDisplay> program;
	OBSWeakSource lastScene;
	OBSWeakSource swapScene;
	OBSWeakSource programScene;
	bool editPropertiesMode = false;
	bool sceneDuplicationMode = true;
	bool swapScenesMode = true;
	volatile bool previewProgramMode = false;
	obs_hotkey_id togglePreviewProgramHotkey = 0;
	obs_hotkey_id transitionHotkey = 0;
	obs_hotkey_id statsHotkey = 0;
	obs_hotkey_id screenshotHotkey = 0;
	obs_hotkey_id sourceScreenshotHotkey = 0;
	int quickTransitionIdCounter = 1;
	bool overridingTransition = false;

	int programX = 0, programY = 0;
	int programCX = 0, programCY = 0;
	float programScale = 0.0f;

	int disableOutputsRef = 0;

	inline void OnActivate();
	inline void OnDeactivate();

	void AddDropSource(const char *file, DropType image);
	void AddDropURL(const char *url, QString &name, obs_data_t *settings,const obs_video_info &ovi);
	void ConfirmDropUrl(const QString &url);
	void dragEnterEvent(QDragEnterEvent *event) override;
	void dragLeaveEvent(QDragLeaveEvent *event) override;
	void dragMoveEvent(QDragMoveEvent *event) override;
	void dropEvent(QDropEvent *event) override;

	void ReplayBufferClicked();

	bool sysTrayMinimizeToTray();

	void EnumDialogs();

	QList<QDialog *> visDialogs;
	QList<QDialog *> modalDialogs;
	QList<QMessageBox *> visMsgBoxes;

	QList<QPoint> visDlgPositions;

	QByteArray startingDockLayout;

	obs_data_array_t *SaveProjectors();
	void LoadSavedProjectors(obs_data_array_t *savedProjectors);

	void ReceivedIntroJson(const QString &text);
	void ShowWhatsNew(const QString &url);

#ifdef BROWSER_AVAILABLE
	QList<QSharedPointer<QDockWidget>> extraBrowserDocks;
	QList<QSharedPointer<QAction>> extraBrowserDockActions;
	QStringList extraBrowserDockTargets;

	void ClearExtraBrowserDocks();
	void LoadExtraBrowserDocks();
	void SaveExtraBrowserDocks();
	void ManageExtraBrowserDocks();
	void AddExtraBrowserDock(const QString &title, const QString &url,bool firstCreate);
#endif

	QIcon imageIcon;
	QIcon colorIcon;
	QIcon slideshowIcon;
	QIcon audioInputIcon;
	QIcon audioOutputIcon;
	QIcon desktopCapIcon;
	QIcon windowCapIcon;
	QIcon gameCapIcon;
	QIcon cameraIcon;
	QIcon textIcon;
	QIcon mediaIcon;
	QIcon browserIcon;
	QIcon groupIcon;
	QIcon sceneIcon;
	QIcon defaultIcon;

	QIcon GetImageIcon() const;
	QIcon GetColorIcon() const;
	QIcon GetSlideshowIcon() const;
	QIcon GetAudioInputIcon() const;
	QIcon GetAudioOutputIcon() const;
	QIcon GetDesktopCapIcon() const;
	QIcon GetWindowCapIcon() const;
	QIcon GetGameCapIcon() const;
	QIcon GetCameraIcon() const;
	QIcon GetTextIcon() const;
	QIcon GetMediaIcon() const;
	QIcon GetBrowserIcon() const;
	QIcon GetDefaultIcon() const;

	QSlider *tBar;
	bool tBarActive = false;

	OBSSource GetOverrideTransition(OBSSource source);
	int GetOverrideTransitionDuration(OBSSource source);

	void UpdateProjectorHideCursor();
	void UpdateProjectorAlwaysOnTop(bool top);
	void ResetProjectors();

	QPointer<QObject> screenshotData;

public slots:
	// 未使用
	void DeferSaveBegin();
	// 未使用
	void DeferSaveEnd();

	// 显示流出错
	void DisplayStreamStartError();

	// 启动推流
	void StartStreaming();
	// 停止推流
	void StopStreaming();
	// 强制停止推流
	void ForceStopStreaming();

	// 未使用
	void StreamDelayStarting(int sec);
	// 未使用
	void StreamDelayStopping(int sec);
	// 未使用
	void StreamingStart();
	// 未使用
	void StreamStopping();
	// 未使用
	void StreamingStop(int errorcode, QString last_error);
	// 启动录像
	void StartRecording();
	// 停止录像
	void StopRecording();
	// 在其它槽函数中调用
	void RecordingStart();
	// 在其它槽函数调用
	void RecordStopping();
	// 未使用
	void RecordingStop(int code, QString last_error);
	//
	void ShowReplayBufferPauseWarning();
	//
	void StartReplayBuffer();
	//
	void StopReplayBuffer();
	//
	void ReplayBufferStart();
	//
	void ReplayBufferSave();
	//
	void ReplayBufferSaved();
	//
	void ReplayBufferStopping();
	//
	void ReplayBufferStop(int code);

	// 启动/关闭虚拟摄像头
	void StartVirtualCam();
	void StopVirtualCam();
	//
	void OnVirtualCamStart();
	//
	void OnVirtualCamStop(int code);
	//
	void SaveProjectDeferred();
	//
	void SaveProject();
	//设置转场
	void SetTransition(OBSSource transition);
	//覆盖转场
	void OverrideTransition(OBSSource transition);
	//
	void TransitionToScene(OBSScene scene, bool force = false);
	//
	void TransitionToScene(OBSSource scene, bool force = false,bool quickTransition = false,int quickDuration = 0, bool black = false,bool manual = false);

	// 切换场景
	void SetCurrentScene(OBSSource scene, bool force = false);
	//
	bool AddSceneCollection(bool create_new,const QString &name = QString());
	//
	void UpdatePatronJson(const QString &text, const QString &error);
	// 显示上文小部件
	void ShowContextBar();
	// 隐藏上文小部件
	void HideContextBar();
	//暂停录像
	void PauseRecording();
	//取消暂停录像
	void UnpauseRecording();

private slots:
	// 添加源(视频，图片等)
	// 该槽函数直接调用或invokeMethod调用
	void AddSceneItem(OBSSceneItem item);
	// 将视频添加到场景列表中
	void AddScene(OBSSource source);
	// 移除场景
	void RemoveScene(OBSSource source);
	// 重命名源
	void RenameSources(OBSSource source, QString newName, QString prevName);
	// 激活音频源
	void ActivateAudioSource(OBSSource source);
	// 反激活音频源
	void DeactivateAudioSource(OBSSource source);
	// 复制选定的场景
	void DuplicateSelectedScene();
	// 移除选定的场景
	void RemoveSelectedScene();
	// 移除选定场景项
	void RemoveSelectedSceneItem();
	// 保持窗口置顶
	void ToggleAlwaysOnTop();
	// 源排序
	void ReorderSources(OBSScene scene);
	// 刷新源
	void RefreshSources(OBSScene scene);
	// 处理热键
	void ProcessHotkey(obs_hotkey_id id, bool pressed);
	// 添加转场
	void AddTransition(QString id);
	// 重命名转场
	void RenameTransition();
	// 点击转场
	void TransitionClicked();
	// 停止转场
	void TransitionStopped();
	// 完全停止转场
	void TransitionFullyStopped();
	// 触发快速转场
	void TriggerQuickTransition(int id);
	//设置去隔行模式
	void SetDeinterlacingMode();
	//设置逐行扫描顺序
	void SetDeinterlacingOrder();
	// 设置缩放过滤器
	void SetScaleFilter();
	//图标激活
	void IconActivated(QSystemTrayIcon::ActivationReason reason);
	// 设置是否显示
	void SetShowing(bool showing);
	//切换显示隐藏
	void ToggleShowHide();
	// 隐藏音频控制
	void HideAudioControl();
	// 不隐藏音频控制
	void UnhideAllAudioControls();
	//切换隐藏混合器
	void ToggleHideMixer();
	// 混音器重名源
	void MixerRenameSource();

	void on_vMixerScrollArea_customContextMenuRequested();
	void on_hMixerScrollArea_customContextMenuRequested();

	void on_actionCopySource_triggered();
	void on_actionPasteRef_triggered();
	void on_actionPasteDup_triggered();

	void on_actionCopyFilters_triggered();
	void on_actionPasteFilters_triggered();
	// 颜色改变
	void ColorChange();

	SourceTreeItem *GetItemWidgetFromSceneItem(obs_sceneitem_t *sceneItem);

	// 显示关于界面
	void on_actionShowAbout_triggered();

	void AudioMixerCopyFilters();
	void AudioMixerPasteFilters();
	// 启动预览
	void EnablePreview();
	// 关闭预览
	void DisablePreview();

	// 复制场景滤镜
	void SceneCopyFilters();
	// 拷贝场景滤镜
	void ScenePasteFilters();

	void CheckDiskSpaceRemaining();
	void OpenSavedProjector(SavedProjectorInfo *info);

	void ScenesReordered();

	void ResetStatsHotkey();

	void SetImageIcon(const QIcon &icon);
	void SetColorIcon(const QIcon &icon);
	void SetSlideshowIcon(const QIcon &icon);
	void SetAudioInputIcon(const QIcon &icon);
	void SetAudioOutputIcon(const QIcon &icon);
	void SetDesktopCapIcon(const QIcon &icon);
	void SetWindowCapIcon(const QIcon &icon);
	void SetGameCapIcon(const QIcon &icon);
	void SetCameraIcon(const QIcon &icon);
	void SetTextIcon(const QIcon &icon);
	void SetMediaIcon(const QIcon &icon);
	void SetBrowserIcon(const QIcon &icon);
	void SetGroupIcon(const QIcon &icon);
	void SetSceneIcon(const QIcon &icon);
	void SetDefaultIcon(const QIcon &icon);

	void TBarChanged(int value);
	void TBarReleased();

	void LockVolumeControl(bool lock);

private:
	/* OBS Callbacks */
	static void SceneReordered(void *data, calldata_t *params);
	static void SceneRefreshed(void *data, calldata_t *params);
	static void SceneItemAdded(void *data, calldata_t *params);
	static void SourceCreated(void *data, calldata_t *params);
	static void SourceRemoved(void *data, calldata_t *params);
	static void SourceActivated(void *data, calldata_t *params);
	static void SourceDeactivated(void *data, calldata_t *params);
	static void SourceAudioActivated(void *data, calldata_t *params);
	static void SourceAudioDeactivated(void *data, calldata_t *params);
	static void SourceRenamed(void *data, calldata_t *params);
	static void RenderMain(void *data, uint32_t cx, uint32_t cy);

	void ResizePreview(uint32_t cx, uint32_t cy);

	void AddSource(const char *id);
	QMenu *CreateAddSourcePopupMenu();
	void AddSourcePopupMenu(const QPoint &pos);
	void copyActionsDynamicProperties();

	static void HotkeyTriggered(void *data, obs_hotkey_id id, bool pressed);

	void AutoRemux();

	void UpdatePause(bool activate = true);
	void UpdateReplayBuffer(bool activate = true);

	bool OutputPathValid();
	void OutputPathInvalidMessage();

	bool LowDiskSpace();
	void DiskSpaceMessage();

	OBSSource prevFTBSource = nullptr;

public:
	OBSSource GetProgramSource();
	OBSScene GetCurrentScene();

	void SysTrayNotify(const QString &text, QSystemTrayIcon::MessageIcon n);

	inline OBSSource GetCurrentSceneSource()
	{
		OBSScene curScene = GetCurrentScene();
		return OBSSource(obs_scene_get_source(curScene));
	}

	obs_service_t *GetService();
	void SetService(obs_service_t *service);

	int GetTransitionDuration();

	inline bool IsPreviewProgramMode() const
	{
		return os_atomic_load_bool(&previewProgramMode);
	}

	inline bool VCamEnabled() const { return vcamEnabled; }

	bool StreamingActive() const;
	bool Active() const;

	void ResetUI();
	int ResetVideo();
	bool ResetAudio();

	void AddVCamButton();
	void ResetOutputs();

	void ResetAudioDevice(const char *sourceId, const char *deviceId,const char *deviceDesc, int channel);

	void NewProject();
	void LoadProject();

	inline void GetDisplayRect(int &x, int &y, int &cx, int &cy)
	{
		x = previewX;
		y = previewY;
		cx = previewCX;
		cy = previewCY;
	}

	inline bool SavingDisabled() const { return disableSaving; }

	inline double GetCPUUsage() const
	{
		return os_cpu_usage_info_query(cpuUsageInfo);
	}

	void SaveService();
	bool LoadService();

	inline Auth *GetAuth() { return auth.get(); }

	inline void EnableOutputs(bool enable)
	{
		if (enable) {
			if (--disableOutputsRef < 0)
				disableOutputsRef = 0;
		} else {
			disableOutputsRef++;
		}
	}

	QMenu *AddDeinterlacingMenu(QMenu *menu, obs_source_t *source);
	QMenu *AddScaleFilteringMenu(QMenu *menu, obs_sceneitem_t *item);
	QMenu *AddBackgroundColorMenu(QMenu *menu, QWidgetAction *widgetAction,ColorSelect *select,obs_sceneitem_t *item);
	void CreateSourcePopupMenu(int idx, bool preview);

	void UpdateTitleBar();
	void UpdateSceneSelection(OBSSource source);

	void SystemTrayInit();
	void SystemTray(bool firstStarted);

	void OpenSavedProjectors();

	void CreateInteractionWindow(obs_source_t *source);
	void CreatePropertiesWindow(obs_source_t *source);
	void CreateFiltersWindow(obs_source_t *source);

	QAction *AddDockWidget(QDockWidget *dock);

	static OBSBasic *Get();

	const char *GetCurrentOutputPath();

	void DeleteProjector(OBSProjector *projector);
	void AddProjectorMenuMonitors(QMenu *parent, QObject *target,const char *slot);

	QIcon GetSourceIcon(const char *id) const;
	QIcon GetGroupIcon() const;
	QIcon GetSceneIcon() const;

	OBSWeakSource copyFilter = nullptr;

protected:
	virtual void closeEvent(QCloseEvent *event) override;
	virtual void changeEvent(QEvent *event) override;

private slots:
	//////////////////////////////////////////////////////////////////// 面板菜单事件槽函数 ///////////////////////////////////////////////
	//视图菜单-进入全屏
	void on_actionFullscreenInterface_triggered();
	//文件菜单-显示录像
	void on_actionShow_Recordings_triggered();
	//文件菜单-录像转封装
	void on_actionRemux_triggered();
	//设置
	void on_action_Settings_triggered();
	// 右键菜单 高级音频属性
	void on_actionAdvAudioProperties_triggered();
	//
	void on_advAudioProps_clicked();
	//销毁高级音频属性界面
	void on_advAudioProps_destroyed();
	//显示日志
	void on_actionShowLogs_triggered();
	//上传当前日志文件
	void on_actionUploadCurrentLog_triggered();
	//上传上次日志文件
	void on_actionUploadLastLog_triggered();
	//查看当前日志
	void on_actionViewCurrentLog_triggered();
	//检查升级
	void on_actionCheckForUpdates_triggered();
	//查看错误报告
	void on_actionShowCrashLogs_triggered();
	//上传上一次错误报告
	void on_actionUploadLastCrashLog_triggered();
	//编辑菜单-变换-编辑变换
	void on_actionEditTransform_triggered();
	//编辑菜单-变换-复制变换
	void on_actionCopyTransform_triggered();
	//编辑菜单-变换-粘贴变换
	void on_actionPasteTransform_triggered();
	//编辑菜单-变换-顺时针旋转90度
	void on_actionRotate90CW_triggered();
	//编辑菜单-变换-逆时针旋转90度
	void on_actionRotate90CCW_triggered();
	//编辑菜单-变换-旋转180度
	void on_actionRotate180_triggered();
	//编辑菜单-变换-水平翻转
	void on_actionFlipHorizontal_triggered();
	//编辑菜单-变换-垂直翻转
	void on_actionFlipVertical_triggered();
	//编辑菜单-变换-比例适配屏幕
	void on_actionFitToScreen_triggered();
	//编辑菜单-变换-拉伸到全屏
	void on_actionStretchToScreen_triggered();
	//编辑菜单-变换-屏幕居中
	void on_actionCenterToScreen_triggered();
	//编辑菜单-变换-垂直居中
	void on_actionVerticalCenter_triggered();
	//编辑菜单-变换-水平居中
	void on_actionHorizontalCenter_triggered();
	//源互动事件
	void on_customContextMenuRequested(const QPoint &pos);

	// 场景切换(左键单击)
	void on_scenes_currentItemChanged(QListWidgetItem *current, QListWidgetItem *prev);
	//场景右键菜单(单个场景)
	void on_scenes_customContextMenuRequested(const QPoint &pos);
	//设置 场景列表 网格模式/列表模式
	void on_actionGridMode_triggered();
	/*
		以下事件槽响应主界面 "场景" widget
	*/
	//添加场景
	void on_actionAddScene_triggered();
	//移除场景
	void on_actionRemoveScene_triggered();
	//场景上移
	void on_actionSceneUp_triggered();
	//场景下移
	void on_actionSceneDown_triggered();
	//场景列表 右键菜单
	void on_sources_customContextMenuRequested(const QPoint &pos);
	//左键双击场景
	void on_scenes_itemDoubleClicked(QListWidgetItem *item);
	/*
		以下事件槽响应主界面 "来源" Widget
	*/
	//添加视频源
	void on_actionAddSource_triggered();
	//删除视频源
	void on_actionRemoveSource_triggered();
	//未定义？
	void on_actionInteract_triggered();
	// 设置视频源属性
	void on_actionSourceProperties_triggered();
	// 视频源上移
	void on_actionSourceUp_triggered();
	// 视频源下移
	void on_actionSourceDown_triggered();
	//场景上移
	void on_actionMoveUp_triggered();                
	//场景下移
	void on_actionMoveDown_triggered();
	//移至顶层(视频源列表/场景列表 右键菜单->排序->移至顶层)
	void on_actionMoveToTop_triggered();
	//移至底层(视频源列表/场景列表 右键菜单->排序->移至底层)
	void on_actionMoveToBottom_triggered();
	//锁定预览
	void on_actionLockPreview_triggered();
	//预览缩放
	void on_scalingMenu_aboutToShow();
	//窗口缩放 
	void on_actionScaleWindow_triggered();
	//画布缩放
	void on_actionScaleCanvas_triggered();
	//输出流缩放
	void on_actionScaleOutput_triggered();
	//开始推流
	void on_streamButton_clicked();
	//开始录制
	void on_recordButton_clicked();
	//启动虚拟摄像机
	void VCamButtonClicked();
	//设置
	void on_settingsButton_clicked();
	//截图(场景/视频源)
	void Screenshot(OBSSource source_ = nullptr);
	//截图(视频源)
	void ScreenshotSelectedSource();
	//截图
	void ScreenshotProgram();
	//截图(场景)
	void ScreenshotScene();
	//帮助门户
	void on_actionHelpPortal_triggered();
	//访问OBS主页
	void on_actionWebsite_triggered();
	//加入DisCord服务器
	void on_actionDiscord_triggered();
	// 预览界面 右键菜单
	void on_preview_customContextMenuRequested(const QPoint &pos);
	//工作室模式界面右键菜单
	void on_program_customContextMenuRequested(const QPoint &pos);
	//关闭预览功能
	void PreviewDisabledMenu(const QPoint &pos);
	//场景集合菜单-新建
	void on_actionNewSceneCollection_triggered();
	//场景集合菜单-复制
	void on_actionDupSceneCollection_triggered();
	//场景集合菜单-重命名
	void on_actionRenameSceneCollection_triggered();
	//场景集合菜单-移除
	void on_actionRemoveSceneCollection_triggered();
	//场景集合菜单-导入
	void on_actionImportSceneCollection_triggered();
	//场景集合菜单-导出
	void on_actionExportSceneCollection_triggered();
	//配置文件菜单-新建
	void on_actionNewProfile_triggered();
	//配置文件菜单-复制
	void on_actionDupProfile_triggered();
	//配置文件菜单-重命名
	void on_actionRenameProfile_triggered();
	//配置文件菜单-移除
	void on_actionRemoveProfile_triggered();
	//配置文件菜单-导入
	void on_actionImportProfile_triggered();
	//配置文件菜单-导出
	void on_actionExportProfile_triggered();
	//文件菜单-打开设置文件夹
	void on_actionShowSettingsFolder_triggered();
	//文件菜单-打开配置文件文件夹
	void on_actionShowProfileFolder_triggered();
	//窗口置顶
	void on_actionAlwaysOnTop_triggered();
	//视图菜单-场景/源列表按钮
	void on_toggleListboxToolbars_toggled(bool visible);
	//视图菜单-源工具栏
	void on_toggleContextBar_toggled(bool visible);
	//视图菜单-状态栏
	void on_toggleStatusBar_toggled(bool visible);
	//视图菜单-源图标
	void on_toggleSourceIcons_toggled(bool visible);
	//转场切换
	void on_transitions_currentIndexChanged(int index);
	//转场移除
	void on_transitionRemove_clicked();
	//转场属性
	void on_transitionProps_clicked();
	//转场持续时间改变
	void on_transitionDuration_valueChanged(int value);
	//工作室模式 切换
	void on_modeSwitch_clicked();

	// 视频源属性按钮
	void on_sourcePropertiesButton_clicked();
	// 视频源滤镜按钮
	void on_sourceFiltersButton_clicked();
	//
	void on_sourceInteractButton_clicked();
	//工具菜单-自动配置
	void on_autoConfigure_triggered();
	//视图菜单-统计
	void on_stats_triggered();
	//视图菜单-停靠部件-重置界面
	void on_resetUI_triggered();
	//视图菜单-停靠部件-锁定界面
	void on_lockUI_toggled(bool lock);
	//
	void PauseToggled();
	//上传日志结束(信号由日志线程触发)
	void logUploadFinished(const QString &text, const QString &error);
	//上传崩溃日志结束(信号由日志线程触发)
	void crashUploadFinished(const QString &text, const QString &error);
	//打开日志对话框
	void openLogDialog(const QString &text, const bool crash);
	//检查更新结束
	void updateCheckFinished();

	// 添加视频源菜单项处理函数
	void AddSourceFromAction();
	//移动场景到顶层
	void MoveSceneToTop();
	//移动场景到底层
	void MoveSceneToBottom();
	//编辑场景名
	void EditSceneName();
	//编辑场景条目名
	void EditSceneItemName();
	//场景名编辑
	void SceneNameEdited(QWidget *editor,QAbstractItemDelegate::EndEditHint endHint);
	//打开场景滤镜
	void OpenSceneFilters();
	//打开滤镜
	void OpenFilters();
	//右键菜单-开启预览
	void EnablePreviewDisplay(bool enable);
	//右键菜单-开启预览
	void TogglePreview();
	//上箭头
	void NudgeUp();
	//下箭头
	void NudgeDown();
	//左箭头
	void NudgeLeft();
	//右箭头
	void NudgeRight();
	//工作室模式 全屏投影
	void OpenStudioProgramProjector();
	//开启预览投影(全屏)
	void OpenPreviewProjector();
	//开启源投影(全屏)
	void OpenSourceProjector();
	//开启多画面投影
	void OpenMultiviewProjector();
	//开启场景投影
	void OpenSceneProjector();
	//开启工作室窗口
	void OpenStudioProgramWindow();
	//开启预览投影(窗口)
	void OpenPreviewWindow();
	//开启源投影（窗口）
	void OpenSourceWindow();
	//开启多画面窗口
	void OpenMultiviewWindow();
	//开启场景窗口
	void OpenSceneWindow();
	//延迟系统小图标加载
	void DeferredSysTrayLoad(int requeueCount);
	//
	void StackedMixerAreaContextMenuRequested();
	//右键菜单-调整输出大小(源大小)
	void ResizeOutputSizeOfSource();

public slots:
	void on_actionResetTransform_triggered();

	bool StreamingActive();
	bool RecordingActive();
	bool ReplayBufferActive();

	void ClearContextBar();
	void UpdateContextBar(bool force = false);
	void UpdateContextBarDeferred(bool force = false);

public:
	explicit OBSBasic(QWidget *parent = 0);
	virtual ~OBSBasic();

	virtual void OBSInit() override;

	virtual config_t *Config() const override;

	virtual int GetProfilePath(char *path, size_t size,
				   const char *file) const override;

	static void InitBrowserPanelSafeBlock();

private:
	std::unique_ptr<Ui::OBSBasic> ui;
};

class SceneRenameDelegate : public QStyledItemDelegate {
	Q_OBJECT

public:
	SceneRenameDelegate(QObject *parent);
	virtual void setEditorData(QWidget *editor,
				   const QModelIndex &index) const override;

protected:
	virtual bool eventFilter(QObject *editor, QEvent *event) override;
};
