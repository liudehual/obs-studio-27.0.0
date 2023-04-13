/*
 * Copyright (c) 2017 Hugh Bailey <obs.jim@gmail.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#pragma once

#include <obs.h>
#include "decode.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4244)
#pragma warning(disable : 4204)
#endif

#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <util/threading.h>

#ifdef _MSC_VER
#pragma warning(pop)
#endif

typedef void (*mp_video_cb)(void *opaque, struct obs_source_frame *frame);
typedef void (*mp_audio_cb)(void *opaque, struct obs_source_audio *audio);
typedef void (*mp_stop_cb)(void *opaque);

struct mp_media {
	AVFormatContext *fmt;// ffmpeg 格式上下文

	mp_video_cb v_preload_cb;// 暂停回调
	mp_video_cb v_seek_cb;// 视频播放重定位
	mp_stop_cb stop_cb;// 停止播放回调
	mp_video_cb v_cb;// 视频回调
	mp_audio_cb a_cb;// 音频回调
	void *opaque;// 回调自定义参数

	char *path;// 文件路径
	char *format_name;// 格式名
	int buffering;// 缓存
	int speed;// 超速/低速播放

	enum AVPixelFormat scale_format;// 转换格式
	struct SwsContext *swscale;// 图像转换，缩放/格式转换等
	int scale_linesizes[4];// Strid
	uint8_t *scale_pic[4];// 数据域

	struct mp_decode v;// 视频解码器上下文结构
	struct mp_decode a;// 音频解码器上下文结构
	bool is_local_file;// 本地文件
	bool reconnecting;// 重连
	bool has_video;// 存在视频
	bool has_audio;// 存在音频
	bool is_file;// 本地文件
	bool eof;//文件结束
	bool hw;// 启用硬件解码

	struct obs_source_frame obsframe;
	enum video_colorspace cur_space;
	enum video_range_type cur_range;
	enum video_range_type force_range;

	int64_t play_sys_ts;// 开始播放时间戳
	int64_t next_pts_ns;// 下一帧pts时间戳
	uint64_t next_ns;// 下一帧播放的时间戳(系统滴答时间,非标准时间)
	int64_t start_ts;// 开始时间
	int64_t base_ts;// 基本时间戳

	uint64_t interrupt_poll_ts;

	pthread_mutex_t mutex;
	os_sem_t *sem;
	bool stopping;// 停止播放
	bool looping;// 循环播放
	bool active;// 激活播放
	bool reset;// 重置播放
	bool kill;// 退出解码线程

	bool thread_valid;//线程可用
	pthread_t thread;// 解码线程

	bool pause;// 暂停播放
	bool reset_ts;// 充值时间戳
	bool seek;// 重定位
	bool seek_next_ts;// 重定位时间戳
	int64_t seek_pos;// 重定位位置
};

typedef struct mp_media mp_media_t;

struct mp_media_info {
	void *opaque;// 回调自定义参数

	mp_video_cb v_cb;// 视频回调
	mp_video_cb v_preload_cb;// 重加载回调
	mp_video_cb v_seek_cb;// 视频重定位回调
	mp_audio_cb a_cb;// 音频回调
	mp_stop_cb stop_cb;// 停止播放回调

	const char *path;// 文件路径
	const char *format;// 格式
	int buffering;// 缓存
	int speed;// 超速/低速播放
	enum video_range_type force_range;//强制颜色空间
	bool hardware_decoding;// 硬件解码
	bool is_local_file;// 本地文件
	bool reconnecting;// 重连，指网络
};

extern bool mp_media_init(mp_media_t *media, const struct mp_media_info *info);
extern void mp_media_free(mp_media_t *media);
extern void mp_media_play(mp_media_t *media, bool loop, bool reconnecting);
extern void mp_media_stop(mp_media_t *media);
extern void mp_media_play_pause(mp_media_t *media, bool pause);
extern int64_t mp_get_current_time(mp_media_t *m);
extern void mp_media_seek_to(mp_media_t *m, int64_t pos);

/* #define DETAILED_DEBUG_INFO */

#if LIBAVCODEC_VERSION_INT >= AV_VERSION_INT(57, 48, 101)
#define USE_NEW_FFMPEG_DECODE_API
#endif

#ifdef __cplusplus
}
#endif
