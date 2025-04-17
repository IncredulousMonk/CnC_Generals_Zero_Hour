/*
 * Copyright (c) 2003 Fabrice Bellard
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

///// FFmpegVideo.h //////////////////////////////
// Matthew Gill, April 2025
//////////////////////////////////////////////////

/**
 * @file
 * simple media player based on the FFmpeg libraries
 */

#ifndef FFMPEG_VIDEO_H
#define FFMPEG_VIDEO_H

#include "LinuxDevice/FFmpeg/FrameQueue.h"
#include "LinuxDevice/FFmpeg/PacketQueue.h"
#include "LinuxDevice/FFmpeg/Decoder.h"
#include "LinuxDevice/FFmpeg/Clock.h"

struct AudioParams {
   int freq;
   int channels;
   int64_t channel_layout;
   enum AVSampleFormat fmt;
   int frame_size;
   int bytes_per_sec;
};

class FFmpegVideo {
private:
   SDL_Renderer* m_renderer {};

   SDL_AudioStream* m_hw_audio_stream {};
   int64_t m_audio_callback_time {};
   SDL_Thread* m_read_tid {};
   int m_abort_request {};
   bool m_video_finished {};
   bool m_force_refresh {};
   int m_queue_attachments_req {};
   int m_read_pause_return {};
   AVFormatContext* m_ic {};

   Clock m_audclk {};
   Clock m_vidclk {};
   Clock m_extclk {};

   FrameQueue m_pictq {}; // Video frame queue
   FrameQueue m_subpq {}; // Subtitle frame queue
   FrameQueue m_sampq {}; // Audio frame queue

   Decoder m_auddec {}; // Audio decoder
   Decoder m_viddec {}; // Video decoder
   Decoder m_subdec {}; // Subtitle decoder

   int m_audio_stream {};

   int m_av_sync_type {};

   double m_audio_clock {};
   int m_audio_clock_serial {};
   double m_audio_diff_cum {}; /* used for AV difference average computation */
   double m_audio_diff_avg_coef {};
   double m_audio_diff_threshold {};
   int m_audio_diff_avg_count {};
   AVStream* m_audio_st {}; // Audio stream
   PacketQueue m_audioq {}; // Audio packet queue
   int m_audio_hw_buf_size {};
   uint8_t* m_audio_buf {};
   uint8_t* m_audio_buf1 {};
   unsigned int m_audio_buf_size {}; /* in bytes */
   unsigned int m_audio_buf1_size {};
   unsigned int m_audio_buf_index {}; /* in bytes */
   int m_audio_write_buf_size {};
   struct AudioParams m_audio_src {};
   struct AudioParams m_audio_tgt {};
   struct SwrContext* m_swr_ctx {};
   int m_frame_drops_early {};
   int m_frame_drops_late {};

   SDL_Texture* m_sub_texture {}; // Subtitle texture
   SDL_Texture* m_vid_texture {}; // Video texture

   int m_subtitle_stream {};
   AVStream* m_subtitle_st {}; // Subtitle stream
   PacketQueue m_subtitleq {}; // Subtitle packet queue

   double m_frame_timer {};
   double m_frame_last_returned_time {};
   double m_frame_last_filter_delay {};
   int m_video_stream {};
   AVStream* m_video_st {}; // Video stream
   PacketQueue m_videoq {}; // Video packet queue
   double m_max_frame_duration {}; // maximum duration of a frame - above this, we consider the jump a timestamp discontinuity
   struct SwsContext* m_img_convert_ctx {};
   struct SwsContext* m_sub_convert_ctx {};
   bool m_eof {};

   char* m_filename {};
   int m_xleft {};
   int m_ytop {};
   int m_width {};
   int m_height {};

   int m_last_video_stream {};
   int m_last_audio_stream {};
   int m_last_subtitle_stream {};

   SDL_Condition* m_continue_read_thread {};

   void* m_avformat_lib {};
   int (*avformat_open_input)(AVFormatContext **ps, const char *url, ff_const59 AVInputFormat *fmt, AVDictionary **options) {};
   void (*avformat_close_input)(AVFormatContext **s) {};
   int (*av_find_best_stream)(AVFormatContext *ic, enum AVMediaType type, int wanted_stream_nb, int related_stream, AVCodec **decoder_ret, int flags) {};
   AVRational (*av_guess_frame_rate)(AVFormatContext *ctx, AVStream *stream, AVFrame *frame) {};
   AVRational (*av_guess_sample_aspect_ratio)(AVFormatContext *format, AVStream *stream, AVFrame *frame) {};
   AVFormatContext *(*avformat_alloc_context)(void) {};
   int (*av_read_frame)(AVFormatContext *s, AVPacket *pkt) {};
   void (*av_format_inject_global_side_data)(AVFormatContext *s) {};
   int (*avio_feof)(AVIOContext *s) {};
public:
   FFmpegVideo();
   ~FFmpegVideo();

   // No copies allowed!
   FFmpegVideo(const FFmpegVideo&) = delete;
   FFmpegVideo& operator=(const FFmpegVideo&) = delete;

   bool open(const char* filename, SDL_Renderer* renderer, int left, int top, int width, int height);
   void close();
   bool update(double* remaining_time);

   void forceRefresh() { m_force_refresh = true; }
private:
   void video_display();
   void video_image_display();
   int realloc_texture(SDL_Texture** texture, SDL_PixelFormat new_format, int new_width, int new_height, SDL_BlendMode blendmode, int init_texture);
   int upload_texture(SDL_Texture** tex, AVFrame* frame, struct SwsContext** img_convert_ctx);
   void get_sdl_pix_fmt_and_blendmode(int format, SDL_PixelFormat* sdl_pix_fmt, SDL_BlendMode* sdl_blendmode);
   void set_sdl_yuv_conversion_mode(AVFrame* frame);
   static int read_thread_wrapper(void* arg);
   int read_thread();
   static int decode_interrupt_cb_wrapper(void* ctx);
   int decode_interrupt_cb();
   void calculate_display_rect(SDL_Rect* rect,
      int scr_xleft, int scr_ytop, int scr_width, int scr_height,
      int pic_width, int pic_height, AVRational pic_sar);
   int stream_component_open(int stream_index);
   int audio_open(int64_t wanted_channel_layout, int wanted_nb_channels, int wanted_sample_rate, struct AudioParams* audio_hw_params);
   static void sdl_new_audio_callback(void* userdata, SDL_AudioStream* stream, int additional_amount, int);
   void sdl_audio_callback(Uint8* stream, int len);
   int audio_decode_frame();
   int synchronize_audio(int nb_samples);
   int get_master_sync_type();
   static int audio_thread_wrapper(void* arg);
   int audio_thread();
   static int video_thread_wrapper(void* arg);
   int video_thread();
   int get_video_frame(AVFrame* frame);
   int queue_picture(AVFrame* src_frame, double pts, double duration, int64_t pos, int serial);
   static int subtitle_thread_wrapper(void* arg);
   int subtitle_thread();
   int stream_has_enough_packets(AVStream* st, int stream_id, PacketQueue* queue);
   void stream_component_close(int stream_index);
   double compute_target_delay(double delay);
   double get_master_clock();
   double vp_duration(Frame* vp, Frame* nextvp);
   void update_video_pts(double pts, int serial);
   void print_error(const char* filename, int err);
   void* open_library(const char* filename);
   void* library_function_address(void* handle, const char* symbol, void** fun);
   void close_library(void* handle);
};

#endif
