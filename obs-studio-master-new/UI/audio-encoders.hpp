#pragma once

#include <obs.hpp>

#include <map>
/// <summary>
/// ////////////////////// AAC 编码器 相关 ////////////////////////////////
/// </summary>
/// <returns></returns>
const std::map<int, const char *> &GetAACEncoderBitrateMap();
const char *GetAACEncoderForBitrate(int bitrate);
int FindClosestAvailableAACBitrate(int bitrate);
