/*--------------------------------------------------------------------------
Copyright (c) 2014-2017, The Linux Foundation. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
--------------------------------------------------------------------------*/
#ifndef __OMX_VENC__H
#define __OMX_VENC__H

#include <unistd.h>
#include "omx_video_base.h"
#ifdef _MSM8974_
#include "video_encoder_device_v4l2.h"
#else
#include "video_encoder_device.h"
#endif

#include "swvenc_api.h"
#include "swvenc_types.h"

extern "C" {
    OMX_API void * get_omx_component_factory_fn(void);
}

struct swvenc_video_capability {
    unsigned int min_width;
    unsigned int max_width;
    unsigned int min_height;
    unsigned int max_height;
};


class omx_venc: public omx_video
{
    public:
        omx_venc();
        ~omx_venc();
        OMX_ERRORTYPE component_init(OMX_STRING role);
        OMX_ERRORTYPE set_parameter(OMX_HANDLETYPE hComp,
                OMX_INDEXTYPE  paramIndex,
                OMX_PTR        paramData);
        OMX_ERRORTYPE set_config(OMX_HANDLETYPE hComp,
                OMX_INDEXTYPE  configIndex,
                OMX_PTR        configData);
        OMX_ERRORTYPE component_deinit(OMX_HANDLETYPE hComp);
        bool is_secure_session();
        //OMX strucutres
        OMX_U32 m_nVenc_format;

        SWVENC_HANDLE m_hSwVenc;
        SWVENC_CODEC  m_codec;
        swvenc_video_capability m_capability;
        bool m_max_allowed_bitrate_check;
        bool m_stopped;
        bool set_format;

        int dev_handle_output_extradata(void *, int);
        int dev_handle_input_extradata(void *, int, int);
        bool dev_buffer_ready_to_queue(OMX_BUFFERHEADERTYPE *buffer);
        bool dev_get_dimensions(OMX_U32 ,OMX_U32 *,OMX_U32 *);
        void dev_set_extradata_cookie(void *);
        int dev_set_format(int);

        static SWVENC_STATUS swvenc_empty_buffer_done_cb
        (
          SWVENC_HANDLE    swvenc,
          SWVENC_IPBUFFER *p_ipbuffer,
          void            *p_client
        );
        SWVENC_STATUS swvenc_empty_buffer_done
        (
          SWVENC_IPBUFFER *p_ipbuffer
        );
        static SWVENC_STATUS swvenc_fill_buffer_done_cb
        (
            SWVENC_HANDLE    swvenc,
            SWVENC_OPBUFFER *p_opbuffer,
            void            *p_client
        );
        static SWVENC_STATUS swvenc_handle_event_cb
        (
            SWVENC_HANDLE swvenc,
            SWVENC_EVENT  event,
            void         *p_client
        );

    private:
        venc_debug_cap m_debug;
        bool m_bSeqHdrRequested;

        OMX_U32 dev_stop(void);
        OMX_U32 dev_pause(void);
        OMX_U32 dev_start(void);
        OMX_U32 dev_flush(unsigned);
        OMX_U32 dev_resume(void);
        OMX_U32 dev_start_done(void);
        OMX_U32 dev_set_message_thread_id(pthread_t);
        bool dev_handle_empty_eos_buffer(void);
        bool dev_use_buf( void *,unsigned,unsigned);
        bool dev_free_buf( void *,unsigned);
        bool dev_empty_buf(void *, void *,unsigned,unsigned);
        bool dev_fill_buf(void *, void *,unsigned,unsigned);
        bool dev_get_buf_req(OMX_U32 *,OMX_U32 *,OMX_U32 *,OMX_U32);
        bool dev_set_buf_req(OMX_U32 const *,OMX_U32 const *,OMX_U32 const *,OMX_U32);
        bool dev_get_seq_hdr(void *, unsigned, unsigned *);
        bool dev_loaded_start(void);
        bool dev_loaded_stop(void);
        bool dev_loaded_start_done(void);
        bool dev_loaded_stop_done(void);
        bool dev_get_capability_ltrcount(OMX_U32 *, OMX_U32 *, OMX_U32 *);
        bool dev_get_performance_level(OMX_U32 *);
        bool dev_get_vui_timing_info(OMX_U32 *);
        bool dev_get_vqzip_sei_info(OMX_U32 *);
        bool dev_get_peak_bitrate(OMX_U32 *);
        bool dev_get_batch_size(OMX_U32 *);
        bool dev_get_temporal_layer_caps(OMX_U32 * /*nMaxLayers*/,
                    OMX_U32 * /*nMaxBLayers*/) {
            return false;
        }
        bool dev_get_pq_status(OMX_BOOL *) {
            return false;
        }
        bool dev_is_video_session_supported(OMX_U32 width, OMX_U32 height);
        bool dev_color_align(OMX_BUFFERHEADERTYPE *buffer, OMX_U32 width,
                        OMX_U32 height);
        bool dev_get_output_log_flag();
        int dev_output_log_buffers(const char *buffer_addr, int buffer_len, uint64_t timestamp);
        int dev_extradata_log_buffers(char *buffer);
        bool swvenc_color_align(OMX_BUFFERHEADERTYPE *buffer, OMX_U32 width,
                                OMX_U32 height);

        SWVENC_STATUS swvenc_set_rc_mode(OMX_VIDEO_CONTROLRATETYPE eControlRate);
        SWVENC_STATUS swvenc_set_profile_level(OMX_U32 eProfile,OMX_U32 eLevel);
        SWVENC_STATUS swvenc_set_intra_refresh(OMX_VIDEO_PARAM_INTRAREFRESHTYPE *IntraRefresh);
        SWVENC_STATUS swvenc_set_frame_rate(OMX_U32 nFrameRate);
        SWVENC_STATUS swvenc_set_bit_rate(OMX_U32 nTargetBitrate);
        SWVENC_STATUS swvenc_set_intra_period(OMX_U32 nPFrame,OMX_U32 nBFrame);
        SWVENC_STATUS swvenc_set_color_format(OMX_COLOR_FORMATTYPE);
        SWVENC_STATUS swvenc_get_buffer_req
        (
           OMX_U32 *min_buff_count,
           OMX_U32 *actual_buff_count,
           OMX_U32 *buff_size,
           OMX_U32 *buff_alignment,
           OMX_U32 port
        );
        int swvenc_input_log_buffers(const char *buffer, int bufferlen);

};

#endif //__OMX_VENC__H
