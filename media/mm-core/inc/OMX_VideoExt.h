/*
 * Copyright (c) 2010 The Khronos Group Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject
 * to the following conditions:
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

/** OMX_VideoExt.h - OpenMax IL version 1.1.2
 * The OMX_VideoExt header file contains extensions to the
 * definitions used by both the application and the component to
 * access video items.
 */

#ifndef OMX_VideoExt_h
#define OMX_VideoExt_h

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Each OMX header shall include all required header files to allow the
 * header to compile without errors.  The includes below are required
 * for this header file to compile successfully
 */
#include <OMX_Core.h>

/** NALU Formats */
typedef enum OMX_NALUFORMATSTYPE {
    OMX_NaluFormatStartCodes = 1,
    OMX_NaluFormatOneNaluPerBuffer = 2,
    OMX_NaluFormatOneByteInterleaveLength = 4,
    OMX_NaluFormatTwoByteInterleaveLength = 8,
    OMX_NaluFormatFourByteInterleaveLength = 16,
    OMX_NaluFormatCodingMax = 0x7FFFFFFF
} OMX_NALUFORMATSTYPE;

/** NAL Stream Format */
typedef struct OMX_NALSTREAMFORMATTYPE{
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_NALUFORMATSTYPE eNaluFormat;
} OMX_NALSTREAMFORMATTYPE;

/** AVC additional profiles */
typedef enum OMX_VIDEO_AVCPROFILEEXTTYPE {
    OMX_VIDEO_AVCProfileConstrainedBaseline = 0x10000,   /**< Constrained baseline profile */
    OMX_VIDEO_AVCProfileConstrainedHigh     = 0x80000,   /**< Constrained high profile */
} OMX_VIDEO_AVCPROFILEEXTTYPE;

/** VP8 profiles */
typedef enum OMX_VIDEO_VP8PROFILETYPE {
    OMX_VIDEO_VP8ProfileMain = 0x01,
    OMX_VIDEO_VP8ProfileUnknown = 0x6EFFFFFF,
    OMX_VIDEO_VP8ProfileMax = 0x7FFFFFFF
} OMX_VIDEO_VP8PROFILETYPE;

/** VP8 levels */
typedef enum OMX_VIDEO_VP8LEVELTYPE {
    OMX_VIDEO_VP8Level_Version0 = 0x01,
    OMX_VIDEO_VP8Level_Version1 = 0x02,
    OMX_VIDEO_VP8Level_Version2 = 0x04,
    OMX_VIDEO_VP8Level_Version3 = 0x08,
    OMX_VIDEO_VP8LevelUnknown = 0x6EFFFFFF,
    OMX_VIDEO_VP8LevelMax = 0x7FFFFFFF
} OMX_VIDEO_VP8LEVELTYPE;

/** VP8 Param */
typedef struct OMX_VIDEO_PARAM_VP8TYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_VIDEO_VP8PROFILETYPE eProfile;
    OMX_VIDEO_VP8LEVELTYPE eLevel;
    OMX_U32 nDCTPartitions;
    OMX_BOOL bErrorResilientMode;
} OMX_VIDEO_PARAM_VP8TYPE;

/** Structure for configuring VP8 reference frames */
typedef struct OMX_VIDEO_VP8REFERENCEFRAMETYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bPreviousFrameRefresh;
    OMX_BOOL bGoldenFrameRefresh;
    OMX_BOOL bAlternateFrameRefresh;
    OMX_BOOL bUsePreviousFrame;
    OMX_BOOL bUseGoldenFrame;
    OMX_BOOL bUseAlternateFrame;
} OMX_VIDEO_VP8REFERENCEFRAMETYPE;

/** Structure for querying VP8 reference frame type */
typedef struct OMX_VIDEO_VP8REFERENCEFRAMEINFOTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bIsIntraFrame;
    OMX_BOOL bIsGoldenOrAlternateFrame;
} OMX_VIDEO_VP8REFERENCEFRAMEINFOTYPE;

/** HEVC Profiles */
typedef enum OMX_VIDEO_HEVCPROFILETYPE {
    OMX_VIDEO_HEVCProfileMain    = 0x01,
    OMX_VIDEO_HEVCProfileMain10  = 0x02,
    // Main10 profile with HDR SEI support.
    OMX_VIDEO_HEVCProfileMain10HDR10  = 0x1000,
    OMX_VIDEO_HEVCProfileUnknown = 0x6EFFFFFF,
    OMX_VIDEO_HEVCProfileMax      = 0x7FFFFFFF
} OMX_VIDEO_HEVCPROFILETYPE;

/** HEVC levels */
typedef enum OMX_VIDEO_HEVCLEVELTYPE {
    OMX_VIDEO_HEVCLevel_Version0  = 0x0,
    OMX_VIDEO_HEVCMainTierLevel1  = 0x1,
    OMX_VIDEO_HEVCHighTierLevel1  = 0x2,
    OMX_VIDEO_HEVCMainTierLevel2  = 0x4,
    OMX_VIDEO_HEVCHighTierLevel2  = 0x8,
    OMX_VIDEO_HEVCMainTierLevel21 = 0x10,
    OMX_VIDEO_HEVCHighTierLevel21 = 0x20,
    OMX_VIDEO_HEVCMainTierLevel3  = 0x40,
    OMX_VIDEO_HEVCHighTierLevel3  = 0x80,
    OMX_VIDEO_HEVCMainTierLevel31 = 0x100,
    OMX_VIDEO_HEVCHighTierLevel31 = 0x200,
    OMX_VIDEO_HEVCMainTierLevel4  = 0x400,
    OMX_VIDEO_HEVCHighTierLevel4  = 0x800,
    OMX_VIDEO_HEVCMainTierLevel41 = 0x1000,
    OMX_VIDEO_HEVCHighTierLevel41 = 0x2000,
    OMX_VIDEO_HEVCMainTierLevel5  = 0x4000,
    OMX_VIDEO_HEVCHighTierLevel5  = 0x8000,
    OMX_VIDEO_HEVCMainTierLevel51 = 0x10000,
    OMX_VIDEO_HEVCHighTierLevel51 = 0x20000,
    OMX_VIDEO_HEVCMainTierLevel52 = 0x40000,
    OMX_VIDEO_HEVCHighTierLevel52 = 0x80000,
    OMX_VIDEO_HEVCMainTierLevel6  = 0x100000,
    OMX_VIDEO_HEVCHighTierLevel6  = 0x200000,
    OMX_VIDEO_HEVCMainTierLevel61 = 0x400000,
    OMX_VIDEO_HEVCHighTierLevel61 = 0x800000,
    OMX_VIDEO_HEVCMainTierLevel62 = 0x1000000,
    OMX_VIDEO_HEVCLevelUnknown = 0x6EFFFFFF,
    OMX_VIDEO_HEVCLevelMax = 0x7FFFFFFF
} OMX_VIDEO_HEVCLEVELTYPE;

/** HEVC Param */
typedef struct OMX_VIDEO_PARAM_HEVCTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_VIDEO_HEVCPROFILETYPE eProfile;
    OMX_VIDEO_HEVCLEVELTYPE eLevel;
    OMX_U32 nKeyFrameInterval;
} OMX_VIDEO_PARAM_HEVCTYPE;

/**
 * Structure for configuring video compression intra refresh period
 *
 * STRUCT MEMBERS:
 *  nSize               : Size of the structure in bytes
 *  nVersion            : OMX specification version information
 *  nPortIndex          : Port that this structure applies to
 *  nRefreshPeriod      : Intra refreh period in frames. Value 0 means disable intra refresh
*/
typedef struct OMX_VIDEO_CONFIG_ANDROID_INTRAREFRESHTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_U32 nRefreshPeriod;
} OMX_VIDEO_CONFIG_ANDROID_INTRAREFRESHTYPE;

/** Maximum number of temporal layers supported by AVC/HEVC */
#define OMX_VIDEO_ANDROID_MAXTEMPORALLAYERS 8

/** temporal layer patterns */
typedef enum OMX_VIDEO_ANDROID_TEMPORALLAYERINGPATTERNTYPE {
    OMX_VIDEO_AndroidTemporalLayeringPatternNone = 0,
    // pattern as defined by WebRTC
    OMX_VIDEO_AndroidTemporalLayeringPatternWebRTC = 1 << 0,
    // pattern where frames in any layer other than the base layer only depend on at most the very
    // last frame from each preceding layer (other than the base layer.)
    OMX_VIDEO_AndroidTemporalLayeringPatternAndroid = 1 << 1,
} OMX_VIDEO_ANDROID_TEMPORALLAYERINGPATTERNTYPE;

/**
 * Android specific param for configuration of temporal layering.
 * Android only supports temporal layering where successive layers each double the
 * previous layer's framerate.
 * NOTE: Reading this parameter at run-time SHALL return actual run-time values.
 *
 *  nSize                      : Size of the structure in bytes
 *  nVersion                   : OMX specification version information
 *  nPortIndex                 : Port that this structure applies to (output port for encoders)
 *  eSupportedPatterns         : A bitmask of supported layering patterns
 *  nLayerCountMax             : Max number of temporal coding layers supported
 *                               by the encoder (must be at least 1, 1 meaning temporal layering
 *                               is NOT supported)
 *  nBLayerCountMax            : Max number of layers that can contain B frames
 *                               (0) to (nLayerCountMax - 1)
 *  ePattern                   : Layering pattern.
 *  nPLayerCountActual         : Number of temporal layers to be coded with non-B frames,
 *                               starting from and including the base-layer.
 *                               (1 to nLayerCountMax - nBLayerCountActual)
 *                               If nPLayerCountActual is 1 and nBLayerCountActual is 0, temporal
 *                               layering is disabled. Otherwise, it is enabled.
 *  nBLayerCountActual         : Number of temporal layers to be coded with B frames,
 *                               starting after non-B layers.
 *                               (0 to nBLayerCountMax)
 *  bBitrateRatiosSpecified    : Flag to indicate if layer-wise bitrate
 *                               distribution is specified.
 *  nBitrateRatios             : Bitrate ratio (100 based) per layer (index 0 is base layer).
 *                               Honored if bBitrateRatiosSpecified is set.
 *                               i.e for 4 layers with desired distribution (25% 25% 25% 25%),
 *                               nBitrateRatio = {25, 50, 75, 100, ... }
 *                               Values in indices not less than 'the actual number of layers
 *                               minus 1' MAY be ignored and assumed to be 100.
 */
typedef struct OMX_VIDEO_PARAM_ANDROID_TEMPORALLAYERINGTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_VIDEO_ANDROID_TEMPORALLAYERINGPATTERNTYPE eSupportedPatterns;
    OMX_U32 nLayerCountMax;
    OMX_U32 nBLayerCountMax;
    OMX_VIDEO_ANDROID_TEMPORALLAYERINGPATTERNTYPE ePattern;
    OMX_U32 nPLayerCountActual;
    OMX_U32 nBLayerCountActual;
    OMX_BOOL bBitrateRatiosSpecified;
    OMX_U32 nBitrateRatios[OMX_VIDEO_ANDROID_MAXTEMPORALLAYERS];
} OMX_VIDEO_PARAM_ANDROID_TEMPORALLAYERINGTYPE;

/**
 * Android specific config for changing the temporal-layer count or
 * bitrate-distribution at run-time.
 *
 *  nSize                      : Size of the structure in bytes
 *  nVersion                   : OMX specification version information
 *  nPortIndex                 : Port that this structure applies to (output port for encoders)
 *  ePattern                   : Layering pattern.
 *  nPLayerCountActual         : Number of temporal layers to be coded with non-B frames.
 *                               (same OMX_VIDEO_PARAM_ANDROID_TEMPORALLAYERINGTYPE limits apply.)
 *  nBLayerCountActual         : Number of temporal layers to be coded with B frames.
 *                               (same OMX_VIDEO_PARAM_ANDROID_TEMPORALLAYERINGTYPE limits apply.)
 *  bBitrateRatiosSpecified    : Flag to indicate if layer-wise bitrate
 *                               distribution is specified.
 *  nBitrateRatios             : Bitrate ratio (100 based, Q16 values) per layer (0 is base layer).
 *                               Honored if bBitrateRatiosSpecified is set.
 *                               (same OMX_VIDEO_PARAM_ANDROID_TEMPORALLAYERINGTYPE limits apply.)
 */
typedef struct OMX_VIDEO_CONFIG_ANDROID_TEMPORALLAYERINGTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_VIDEO_ANDROID_TEMPORALLAYERINGPATTERNTYPE ePattern;
    OMX_U32 nPLayerCountActual;
    OMX_U32 nBLayerCountActual;
    OMX_BOOL bBitrateRatiosSpecified;
    OMX_U32 nBitrateRatios[OMX_VIDEO_ANDROID_MAXTEMPORALLAYERS];
} OMX_VIDEO_CONFIG_ANDROID_TEMPORALLAYERINGTYPE;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OMX_VideoExt_h */
/* File EOF */
