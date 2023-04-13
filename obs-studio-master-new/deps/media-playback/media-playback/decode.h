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

#ifdef __cplusplus
extern "C" {
#endif

#include <util/circlebuf.h>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4244)
#pragma warning(disable : 4204)
#endif

#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <util/threading.h>

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#if LIBAVCODEC_VERSION_MAJOR >= 58
#define CODEC_CAP_TRUNC AV_CODEC_CAP_TRUNCATED
#define CODEC_FLAG_TRUNC AV_CODEC_FLAG_TRUNCATED
#else
#define CODEC_CAP_TRUNC CODEC_CAP_TRUNCATED
#define CODEC_FLAG_TRUNC CODEC_FLAG_TRUNCATED
#endif

struct mp_media;

struct mp_decode {
	struct mp_media *m;
	AVStream *stream;// ffmpeg 中的视频流/音频流
	bool audio;// true 音频流 false 非视频流(一般指视频流)

	AVCodecContext *decoder;// 解码器上下文
	AVBufferRef *hw_ctx;// 硬件解码上下文
	AVCodec *codec;// 解码器

	int64_t last_duration;// 最新一帧持续时间
	int64_t frame_pts;// 0 开始，纳秒，视频播放时长,倍速播放会被修正
	int64_t next_pts;// 下一次刷新时间
	AVFrame *in_frame;// 解码器解码成功后数据会放入该帧中,该帧在软件解码时会指向sw_frame,在硬件解码时指向hw_frame
	AVFrame *sw_frame;// 软解码
	AVFrame *hw_frame;// 硬件解码
	AVFrame *frame;// 实际通过回调传出该libobs 核心的指针，默认指向sw_frame
	enum AVPixelFormat hw_format;// 硬解码格式
	bool got_first_keyframe;// 第一个关键帧s
	bool frame_ready;// 帧数据已经准备好
	bool eof;// 文件结束
	bool hw;// 硬解码

	AVPacket orig_pkt;// 从队列中获取一包数据，该包数据在解码是会被传递给pkt字段用于解码
	AVPacket pkt;// 该帧数据被放入解码中解码
	bool packet_pending;
	struct circlebuf packets;// 包缓存队列
};

extern bool mp_decode_init(struct mp_media *media, enum AVMediaType type,bool hw);
extern void mp_decode_free(struct mp_decode *decode);
extern void mp_decode_clear_packets(struct mp_decode *decode);
extern void mp_decode_push_packet(struct mp_decode *decode, AVPacket *pkt);
extern bool mp_decode_next(struct mp_decode *decode);
extern void mp_decode_flush(struct mp_decode *decode);

#ifdef __cplusplus
}
#endif
