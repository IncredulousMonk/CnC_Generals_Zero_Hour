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

///// FFmpegVideo.cpp ////////////////////////////
// Matthew Gill, April 2025
//////////////////////////////////////////////////

/**
 * @file
 * simple media player based on the FFmpeg libraries
 */

#include "LinuxDevice/FFmpeg/FFmpegVideo.h"

extern "C" {
#include "libavutil/time.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
}
#include <dlfcn.h>

#define MAX_QUEUE_SIZE (15 * 1024 * 1024)
#define MIN_FRAMES 25

/* Minimum SDL audio buffer size, in samples. */
#define SDL_AUDIO_MIN_BUFFER_SIZE 512
/* Calculate actual buffer size keeping in mind not cause too frequent audio callbacks */
#define SDL_AUDIO_MAX_CALLBACKS_PER_SEC 30

/* maximum audio speed change to get correct sync */
#define SAMPLE_CORRECTION_PERCENT_MAX 10

/* we use about AUDIO_DIFF_AVG_NB A-V differences to make the average */
#define AUDIO_DIFF_AVG_NB   20

/* polls for possible required screen refresh at least this often, should be less than 1/fps */
#define REFRESH_RATE 0.01

#define USE_ONEPASS_SUBTITLE_RENDER 1

static const int sws_flags = SWS_BICUBIC;

enum {
   AV_SYNC_AUDIO_MASTER, /* default choice */
   AV_SYNC_VIDEO_MASTER,
   AV_SYNC_EXTERNAL_CLOCK, /* synchronize to an external clock */
};

static const struct TextureFormatEntry {
   enum AVPixelFormat format;
   SDL_PixelFormat texture_fmt;
} sdl_texture_format_map[] = {
   { AV_PIX_FMT_RGB8,           SDL_PIXELFORMAT_RGB332 },
   { AV_PIX_FMT_RGB444,         SDL_PIXELFORMAT_XRGB4444 },
   { AV_PIX_FMT_RGB555,         SDL_PIXELFORMAT_XRGB1555 },
   { AV_PIX_FMT_BGR555,         SDL_PIXELFORMAT_XBGR1555 },
   { AV_PIX_FMT_RGB565,         SDL_PIXELFORMAT_RGB565 },
   { AV_PIX_FMT_BGR565,         SDL_PIXELFORMAT_BGR565 },
   { AV_PIX_FMT_RGB24,          SDL_PIXELFORMAT_RGB24 },
   { AV_PIX_FMT_BGR24,          SDL_PIXELFORMAT_BGR24 },
   { AV_PIX_FMT_0RGB32,         SDL_PIXELFORMAT_XRGB8888 },
   { AV_PIX_FMT_0BGR32,         SDL_PIXELFORMAT_XBGR8888 },
   { AV_PIX_FMT_NE(RGB0, 0BGR), SDL_PIXELFORMAT_RGBX8888 },
   { AV_PIX_FMT_NE(BGR0, 0RGB), SDL_PIXELFORMAT_BGRX8888 },
   { AV_PIX_FMT_RGB32,          SDL_PIXELFORMAT_ARGB8888 },
   { AV_PIX_FMT_RGB32_1,        SDL_PIXELFORMAT_RGBA8888 },
   { AV_PIX_FMT_BGR32,          SDL_PIXELFORMAT_ABGR8888 },
   { AV_PIX_FMT_BGR32_1,        SDL_PIXELFORMAT_BGRA8888 },
   { AV_PIX_FMT_YUV420P,        SDL_PIXELFORMAT_IYUV },
   { AV_PIX_FMT_YUYV422,        SDL_PIXELFORMAT_YUY2 },
   { AV_PIX_FMT_UYVY422,        SDL_PIXELFORMAT_UYVY },
   { AV_PIX_FMT_NONE,           SDL_PIXELFORMAT_UNKNOWN },
};

FFmpegVideo::FFmpegVideo() {
}

FFmpegVideo::~FFmpegVideo() {
}

bool FFmpegVideo::open(const char* filename, SDL_Renderer* renderer, int left, int top, int width, int height) {
   // Yuck!  libavformat is bringing in something that calls operator "new" during library load.
   // This causes problems with the DynamicMemoryAllocator, so I'm forced to late load the library.
   m_avformat_lib = open_library("libavformat.so");
   library_function_address(m_avformat_lib, "avformat_open_input", reinterpret_cast<void **>(&avformat_open_input));
   library_function_address(m_avformat_lib, "avformat_close_input", reinterpret_cast<void **>(&avformat_close_input));
   library_function_address(m_avformat_lib, "av_find_best_stream", reinterpret_cast<void **>(&av_find_best_stream));
   library_function_address(m_avformat_lib, "av_guess_frame_rate", reinterpret_cast<void **>(&av_guess_frame_rate));
   library_function_address(m_avformat_lib, "av_guess_sample_aspect_ratio", reinterpret_cast<void **>(&av_guess_sample_aspect_ratio));
   library_function_address(m_avformat_lib, "avformat_alloc_context", reinterpret_cast<void **>(&avformat_alloc_context));
   library_function_address(m_avformat_lib, "av_read_frame", reinterpret_cast<void **>(&av_read_frame));
   library_function_address(m_avformat_lib, "av_format_inject_global_side_data", reinterpret_cast<void **>(&av_format_inject_global_side_data));
   library_function_address(m_avformat_lib, "avio_feof", reinterpret_cast<void **>(&avio_feof));

   m_last_video_stream = m_video_stream = -1;
   m_last_audio_stream = m_audio_stream = -1;
   m_last_subtitle_stream = m_subtitle_stream = -1;
   m_filename = av_strdup(filename);
   m_renderer = renderer;
   m_xleft = left;
   m_ytop = top;
   m_width = width;
   m_height = height;
   if (!m_filename) {
      goto fail;
   }

   /* start video display */
   if (m_pictq.init(&m_videoq, VIDEO_PICTURE_QUEUE_SIZE, true) < 0) {
      goto fail;
   }
   if (m_subpq.init(&m_subtitleq, SUBPICTURE_QUEUE_SIZE, false) < 0) {
      goto fail;
   }
   if (m_sampq.init(&m_audioq, SAMPLE_QUEUE_SIZE, true) < 0) {
      goto fail;
   }

   if (m_videoq.init() < 0 || m_audioq.init() < 0 || m_subtitleq.init() < 0) {
      goto fail;
   }

   if (!(m_continue_read_thread = SDL_CreateCondition())) {
      SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "SDL_CreateCondition(): %s", SDL_GetError());
      goto fail;
   }

   m_vidclk.init(m_videoq.getSerial());
   m_audclk.init(m_audioq.getSerial());
   m_extclk.init(m_extclk.getSerial());
   m_audio_clock_serial = -1;
   m_av_sync_type = AV_SYNC_AUDIO_MASTER;
   m_read_tid = SDL_CreateThread(read_thread_wrapper, "read_thread", this);
   if (!m_read_tid) {
      SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "SDL_CreateThread(): %s", SDL_GetError());
fail:
      close();
      return false;
   }
   return true;
}

void FFmpegVideo::close() {
   m_abort_request = 1;
   SDL_WaitThread(m_read_tid, NULL);

   /* close each stream */
   if (m_audio_stream >= 0) {
      stream_component_close(m_audio_stream);
   }
   if (m_video_stream >= 0) {
      stream_component_close(m_video_stream);
   }
   if (m_subtitle_stream >= 0) {
      stream_component_close(m_subtitle_stream);
   }

   avformat_close_input(&m_ic);

   m_videoq.destroy();
   m_audioq.destroy();
   m_subtitleq.destroy();

   /* free all pictures */
   m_pictq.destroy();
   m_sampq.destroy();
   m_subpq.destroy();
   SDL_DestroyCondition(m_continue_read_thread);
   sws_freeContext(m_img_convert_ctx);
   sws_freeContext(m_sub_convert_ctx);
   av_free(m_filename);
   if (m_vid_texture) {
      SDL_DestroyTexture(m_vid_texture);
   }
   if (m_sub_texture) {
      SDL_DestroyTexture(m_sub_texture);
   }
   close_library(m_avformat_lib);
}

/* Called to display each frame.  Returns false if video has finished. */
bool FFmpegVideo::update(double *remaining_time) {
   double time;

   Frame *sp, *sp2;

   if (m_video_st) {
retry:
      if (m_pictq.nb_remaining() == 0) {
         // nothing to do, no picture to display in the queue
      } else {
         double last_duration, duration, delay;
         Frame *vp, *lastvp;

         /* dequeue the picture */
         lastvp = m_pictq.peek_last();
         vp = m_pictq.peek();

         if (vp->serial != m_videoq.getSerial()) {
            m_pictq.next();
            goto retry;
         }

         if (lastvp->serial != vp->serial) {
            m_frame_timer = av_gettime_relative() / 1000000.0;
         }

         /* compute nominal last_duration */
         last_duration = vp_duration(lastvp, vp);
         delay = compute_target_delay(last_duration);

         time = av_gettime_relative()/1000000.0;
         if (time < m_frame_timer + delay) {
            *remaining_time = FFMIN(m_frame_timer + delay - time, *remaining_time);
            goto display;
         }

         m_frame_timer += delay;
         if (delay > 0 && time - m_frame_timer > AV_SYNC_THRESHOLD_MAX) {
            m_frame_timer = time;
         }

         m_pictq.lock();
         if (!isnan(vp->pts)) {
            update_video_pts(vp->pts, vp->serial);
         }
         m_pictq.unlock();

         if (m_pictq.nb_remaining() > 1) {
            Frame *nextvp = m_pictq.peek_next();
            duration = vp_duration(vp, nextvp);
            if (time > m_frame_timer + duration){
               m_frame_drops_late++;
               m_pictq.next();
               goto retry;
            }
         }

         if (m_subtitle_st) {
            while (m_subpq.nb_remaining() > 0) {
               sp = m_subpq.peek();

               if (m_subpq.nb_remaining() > 1) {
                  sp2 = m_subpq.peek_next();
               } else {
                  sp2 = NULL;
               }

               if (sp->serial != m_subtitleq.getSerial()
                  || (m_vidclk.getPts() > (sp->pts + ((float) sp->sub.end_display_time / 1000)))
                  || (sp2 && m_vidclk.getPts() > (sp2->pts + ((float) sp2->sub.start_display_time / 1000))))
               {
                  if (sp->uploaded) {
                     for (unsigned i {0}; i < sp->sub.num_rects; ++i) {
                        AVSubtitleRect *sub_rect = sp->sub.rects[i];
                        uint8_t *pixels;
                        int pitch, j;

                        if (!SDL_LockTexture(m_sub_texture, (SDL_Rect *)sub_rect, (void **)&pixels, &pitch)) {
                           for (j = 0; j < sub_rect->h; j++, pixels += pitch) {
                              memset(pixels, 0, static_cast<size_t>(sub_rect->w) << 2);
                           }
                           SDL_UnlockTexture(m_sub_texture);
                        }
                     }
                  }
                  m_subpq.next();
               } else {
                  break;
               }
            }
         }

         m_pictq.next();
         m_force_refresh = true;
      }
display:
      /* display picture */
      if (m_force_refresh && m_pictq.getRindexShown()) {
         video_display();
      }
   }
   m_force_refresh = false;
   return !m_video_finished;
}

/* display the current picture, if any */
void FFmpegVideo::video_display() {
   SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
   SDL_RenderClear(m_renderer);
   if (m_video_st) {
      video_image_display();
   }
   SDL_RenderPresent(m_renderer);
}

void FFmpegVideo::video_image_display() {
   Frame *vp;
   Frame *sp = NULL;
   SDL_Rect rect;

   vp = m_pictq.peek_last();
   if (m_subtitle_st) {
      if (m_subpq.nb_remaining() > 0) {
         sp = m_subpq.peek();

         if (vp->pts >= sp->pts + ((float) sp->sub.start_display_time / 1000)) {
               if (!sp->uploaded) {
                  uint8_t* pixels[4];
                  int pitch[4];
                  if (!sp->width || !sp->height) {
                     sp->width = vp->width;
                     sp->height = vp->height;
                  }
                  if (realloc_texture(&m_sub_texture, SDL_PIXELFORMAT_ARGB8888, sp->width, sp->height, SDL_BLENDMODE_BLEND, 1) < 0)
                     return;

                  for (unsigned i {0}; i < sp->sub.num_rects; ++i) {
                     AVSubtitleRect *sub_rect = sp->sub.rects[i];

                     sub_rect->x = av_clip(sub_rect->x, 0, sp->width );
                     sub_rect->y = av_clip(sub_rect->y, 0, sp->height);
                     sub_rect->w = av_clip(sub_rect->w, 0, sp->width  - sub_rect->x);
                     sub_rect->h = av_clip(sub_rect->h, 0, sp->height - sub_rect->y);

                     m_sub_convert_ctx = sws_getCachedContext(m_sub_convert_ctx,
                        sub_rect->w, sub_rect->h, AV_PIX_FMT_PAL8,
                        sub_rect->w, sub_rect->h, AV_PIX_FMT_BGRA,
                        0, NULL, NULL, NULL);
                     if (!m_sub_convert_ctx) {
                        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Cannot initialize the conversion context");
                        return;
                     }
                     if (!SDL_LockTexture(m_sub_texture, (SDL_Rect *)sub_rect, (void **)pixels, pitch)) {
                        sws_scale(m_sub_convert_ctx, (const uint8_t * const *)sub_rect->data, sub_rect->linesize,
                           0, sub_rect->h, pixels, pitch);
                        SDL_UnlockTexture(m_sub_texture);
                     }
                  }
                  sp->uploaded = true;
               }
         } else {
            sp = NULL;
         }
      }
   }

   calculate_display_rect(&rect, m_xleft, m_ytop, m_width, m_height, vp->width, vp->height, vp->sar);

   if (!vp->uploaded) {
      if (upload_texture(&m_vid_texture, vp->frame, &m_img_convert_ctx) < 0) {
         return;
      }
      vp->uploaded = true;
      vp->flip_v = vp->frame->linesize[0] < 0;
   }

   set_sdl_yuv_conversion_mode(vp->frame);
   SDL_FRect dstrect;
   dstrect.x = rect.x;
   dstrect.y = rect.y;
   dstrect.w = rect.w;
   dstrect.h = rect.h;
   SDL_RenderTextureRotated(m_renderer, m_vid_texture, NULL, &dstrect, 0, NULL, vp->flip_v ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE);
   set_sdl_yuv_conversion_mode(NULL);
   if (sp) {
#if USE_ONEPASS_SUBTITLE_RENDER
      SDL_RenderTexture(m_renderer, m_sub_texture, NULL, &dstrect);
#else
      int i;
      double xratio = (double)rect.w / (double)sp->width;
      double yratio = (double)rect.h / (double)sp->height;
      for (i = 0; i < sp->sub.num_rects; i++) {
         SDL_Rect *sub_rect = (SDL_Rect*)sp->sub.rects[i];
         SDL_Rect target = {.x = rect.x + sub_rect->x * xratio,
                              .y = rect.y + sub_rect->y * yratio,
                              .w = sub_rect->w * xratio,
                              .h = sub_rect->h * yratio};
         SDL_RenderCopy(m_renderer, m_sub_texture, sub_rect, &target);
      }
#endif
   }
}

int FFmpegVideo::realloc_texture(SDL_Texture **texture, SDL_PixelFormat new_format, int new_width, int new_height, SDL_BlendMode blendmode, int init_texture) {
   Uint32 format;
   int w, h;
   SDL_PropertiesID props = 0;
   if (*texture) {
      props = SDL_GetTextureProperties(*texture);
      if (props) {
         format = SDL_GetNumberProperty(props, SDL_PROP_TEXTURE_FORMAT_NUMBER, -1);
         w = SDL_GetNumberProperty(props, SDL_PROP_TEXTURE_WIDTH_NUMBER, -1);
         h = SDL_GetNumberProperty(props, SDL_PROP_TEXTURE_HEIGHT_NUMBER, -1);
      }
   }
   if (!*texture || !props || new_width != w || new_height != h || new_format != format) {
      void *pixels;
      int pitch;
      if (*texture)
         SDL_DestroyTexture(*texture);
      if (!(*texture = SDL_CreateTexture(m_renderer, new_format, SDL_TEXTUREACCESS_STREAMING, new_width, new_height))) {
         return -1;
      }
      if (!SDL_SetTextureBlendMode(*texture, blendmode)) {
         return -1;
      }
      if (init_texture) {
         if (!SDL_LockTexture(*texture, NULL, &pixels, &pitch)) {
            return -1;
         }
         memset(pixels, 0, static_cast<size_t>(pitch * new_height));
         SDL_UnlockTexture(*texture);
      }
      SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Created %dx%d texture with %s.", new_width, new_height, SDL_GetPixelFormatName(new_format));
   }
   return 0;
}

int FFmpegVideo::upload_texture(SDL_Texture **tex, AVFrame *frame, struct SwsContext **img_convert_ctx) {
   int ret = 0;
   SDL_PixelFormat sdl_pix_fmt;
   SDL_BlendMode sdl_blendmode;
   get_sdl_pix_fmt_and_blendmode(frame->format, &sdl_pix_fmt, &sdl_blendmode);
   if (realloc_texture(tex, sdl_pix_fmt == SDL_PIXELFORMAT_UNKNOWN ? SDL_PIXELFORMAT_ARGB8888 : sdl_pix_fmt, frame->width, frame->height, sdl_blendmode, 0) < 0) {
      return -1;
   }
   switch (sdl_pix_fmt) {
      case SDL_PIXELFORMAT_UNKNOWN:
         /* This should only happen if we are not using avfilter... */
         *img_convert_ctx = sws_getCachedContext(*img_convert_ctx,
            frame->width, frame->height, static_cast<AVPixelFormat>(frame->format), frame->width, frame->height,
            AV_PIX_FMT_BGRA, sws_flags, NULL, NULL, NULL);
         if (*img_convert_ctx != NULL) {
            uint8_t *pixels[4];
            int pitch[4];
            if (!SDL_LockTexture(*tex, NULL, (void **)pixels, pitch)) {
               sws_scale(*img_convert_ctx, (const uint8_t * const *)frame->data, frame->linesize,
                        0, frame->height, pixels, pitch);
               SDL_UnlockTexture(*tex);
            }
         } else {
            SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Cannot initialize the conversion context");
            ret = -1;
         }
         break;
      case SDL_PIXELFORMAT_IYUV:
         if (frame->linesize[0] > 0 && frame->linesize[1] > 0 && frame->linesize[2] > 0) {
            ret = SDL_UpdateYUVTexture(*tex, NULL, frame->data[0], frame->linesize[0],
                                                   frame->data[1], frame->linesize[1],
                                                   frame->data[2], frame->linesize[2]);
         } else if (frame->linesize[0] < 0 && frame->linesize[1] < 0 && frame->linesize[2] < 0) {
            ret = SDL_UpdateYUVTexture(*tex, NULL, frame->data[0] + frame->linesize[0] * (frame->height                    - 1), -frame->linesize[0],
                                                   frame->data[1] + frame->linesize[1] * (AV_CEIL_RSHIFT(frame->height, 1) - 1), -frame->linesize[1],
                                                   frame->data[2] + frame->linesize[2] * (AV_CEIL_RSHIFT(frame->height, 1) - 1), -frame->linesize[2]);
         } else {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Mixed negative and positive linesizes are not supported.");
            return -1;
         }
         break;
      default:
         if (frame->linesize[0] < 0) {
            ret = SDL_UpdateTexture(*tex, NULL, frame->data[0] + frame->linesize[0] * (frame->height - 1), -frame->linesize[0]);
         } else {
            ret = SDL_UpdateTexture(*tex, NULL, frame->data[0], frame->linesize[0]);
         }
         break;
   }
   return ret;
}

void FFmpegVideo::get_sdl_pix_fmt_and_blendmode(int format, SDL_PixelFormat *sdl_pix_fmt, SDL_BlendMode *sdl_blendmode) {
   *sdl_blendmode = SDL_BLENDMODE_NONE;
   *sdl_pix_fmt = SDL_PIXELFORMAT_UNKNOWN;
   if (format == AV_PIX_FMT_RGB32 || format == AV_PIX_FMT_RGB32_1 || format == AV_PIX_FMT_BGR32 || format == AV_PIX_FMT_BGR32_1) {
      *sdl_blendmode = SDL_BLENDMODE_BLEND;
   }
   for (size_t i {0}; i < FF_ARRAY_ELEMS(sdl_texture_format_map) - 1; ++i) {
      if (format == sdl_texture_format_map[i].format) {
         *sdl_pix_fmt = sdl_texture_format_map[i].texture_fmt;
         return;
      }
   }
}

void FFmpegVideo::set_sdl_yuv_conversion_mode(AVFrame *frame) {
#if SDL_VERSION_ATLEAST(2,0,8)
   // SDL_YUV_CONVERSION_MODE mode = SDL_YUV_CONVERSION_AUTOMATIC;
   if (frame && (frame->format == AV_PIX_FMT_YUV420P || frame->format == AV_PIX_FMT_YUYV422 || frame->format == AV_PIX_FMT_UYVY422)) {
      if (frame->color_range == AVCOL_RANGE_JPEG)
         // mode = SDL_COLORSPACE_JPEG;
         SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Yay! YUVConversionMode is SDL_COLORSPACE_JPEG");
      else if (frame->colorspace == AVCOL_SPC_BT709)
         // mode = SDL_YUV_CONVERSION_BT709;
         SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "YUVConversionMode should be SDL_YUV_CONVERSION_BT709");
      else if (frame->colorspace == AVCOL_SPC_BT470BG || frame->colorspace == AVCOL_SPC_SMPTE170M || frame->colorspace == AVCOL_SPC_SMPTE240M)
         // mode = SDL_YUV_CONVERSION_BT601;
         SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "YUVConversionMode should be SDL_YUV_CONVERSION_BT601");
   }
   // SDL_SetYUVConversionMode(mode);
#endif
}

int FFmpegVideo::read_thread_wrapper(void *arg) {
   FFmpegVideo* self = static_cast<FFmpegVideo*>(arg);
   return self->read_thread();
}

/* this thread gets the stream from the disk */
int FFmpegVideo::read_thread() {
   AVFormatContext *ic = NULL;
   int err, ret;
   int st_index[AVMEDIA_TYPE_NB];
   AVPacket *pkt = NULL;
   SDL_Mutex *wait_mutex = SDL_CreateMutex();

   if (!wait_mutex) {
      SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "SDL_CreateMutex(): %s", SDL_GetError());
      ret = AVERROR(ENOMEM);
      goto fail;
   }

   memset(st_index, -1, sizeof(st_index));
   m_eof = false;

   pkt = av_packet_alloc();
   if (!pkt) {
      SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Could not allocate packet.");
      ret = AVERROR(ENOMEM);
      goto fail;
   }
   ic = avformat_alloc_context();
   if (!ic) {
      SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Could not allocate context.");
      ret = AVERROR(ENOMEM);
      goto fail;
   }
   ic->interrupt_callback.callback = decode_interrupt_cb_wrapper;
   ic->interrupt_callback.opaque = this;
   err = avformat_open_input(&ic, m_filename, NULL, NULL);
   if (err < 0) {
      print_error(m_filename, err);
      ret = -1;
      goto fail;
   }
   m_ic = ic;

   av_format_inject_global_side_data(ic);

   if (ic->pb)
      ic->pb->eof_reached = 0; // FIXME hack, ffplay maybe should not use avio_feof() to test for the end

   m_max_frame_duration = (ic->iformat->flags & AVFMT_TS_DISCONT) ? 10.0 : 3600.0;

   // av_dump_format(ic, 0, m_vs->filename, 0);

   for (unsigned i {0}; i < ic->nb_streams; ++i) {
      AVStream *st = ic->streams[i];
      st->discard = AVDISCARD_ALL;
   }

   st_index[AVMEDIA_TYPE_VIDEO] =
      av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO,
                           st_index[AVMEDIA_TYPE_VIDEO], -1, NULL, 0);
   st_index[AVMEDIA_TYPE_AUDIO] =
      av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO,
                           st_index[AVMEDIA_TYPE_AUDIO],
                           st_index[AVMEDIA_TYPE_VIDEO],
                           NULL, 0);
   st_index[AVMEDIA_TYPE_SUBTITLE] =
      av_find_best_stream(ic, AVMEDIA_TYPE_SUBTITLE,
                           st_index[AVMEDIA_TYPE_SUBTITLE],
                           (st_index[AVMEDIA_TYPE_AUDIO] >= 0 ?
                              st_index[AVMEDIA_TYPE_AUDIO] :
                              st_index[AVMEDIA_TYPE_VIDEO]),
                           NULL, 0);

   /* open the streams */
   if (st_index[AVMEDIA_TYPE_AUDIO] >= 0) {
      stream_component_open(st_index[AVMEDIA_TYPE_AUDIO]);
   }

   ret = -1;
   if (st_index[AVMEDIA_TYPE_VIDEO] >= 0) {
      ret = stream_component_open(st_index[AVMEDIA_TYPE_VIDEO]);
   }

   if (st_index[AVMEDIA_TYPE_SUBTITLE] >= 0) {
      stream_component_open(st_index[AVMEDIA_TYPE_SUBTITLE]);
   }

   if (m_video_stream < 0 && m_audio_stream < 0) {
      SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Failed to open file '%s'", m_filename);
      ret = -1;
      goto fail;
   }

   for (;;) {
      if (m_abort_request)
         break;
      if (m_queue_attachments_req) {
         if (m_video_st && m_video_st->disposition & AV_DISPOSITION_ATTACHED_PIC) {
               if ((ret = av_packet_ref(pkt, &m_video_st->attached_pic)) < 0) {
                  goto fail;
               }
               m_videoq.put(pkt);
               m_videoq.put_nullpacket(pkt, m_video_stream);
         }
         m_queue_attachments_req = 0;
      }

      /* if the queue are full, no need to read more */
      if ((m_audioq.getSize() + m_videoq.getSize() + m_subtitleq.getSize() > MAX_QUEUE_SIZE
         || (stream_has_enough_packets(m_audio_st, m_audio_stream, &m_audioq) &&
               stream_has_enough_packets(m_video_st, m_video_stream, &m_videoq) &&
               stream_has_enough_packets(m_subtitle_st, m_subtitle_stream, &m_subtitleq)))) {
         /* wait 10 ms */
         SDL_LockMutex(wait_mutex);
         SDL_WaitConditionTimeout(m_continue_read_thread, wait_mutex, 10);
         SDL_UnlockMutex(wait_mutex);
         continue;
      }
      if ((!m_audio_st || (m_auddec.getFinished() == m_audioq.getSerial() && m_sampq.nb_remaining() == 0)) &&
         (!m_video_st || (m_viddec.getFinished() == m_videoq.getSerial() && m_pictq.nb_remaining() == 0))) {
         ret = AVERROR_EOF;
         goto fail;
      }
      ret = av_read_frame(ic, pkt);
      if (ret < 0) {
         if ((ret == AVERROR_EOF || avio_feof(ic->pb)) && !m_eof) {
               if (m_video_stream >= 0) {
                  m_videoq.put_nullpacket(pkt, m_video_stream);
               }
               if (m_audio_stream >= 0) {
                  m_audioq.put_nullpacket(pkt, m_audio_stream);
               }
               if (m_subtitle_stream >= 0) {
                  m_subtitleq.put_nullpacket(pkt, m_subtitle_stream);
               }
               m_eof = true;
         }
         if (ic->pb && ic->pb->error) {
               goto fail;
         }
         SDL_LockMutex(wait_mutex);
         SDL_WaitConditionTimeout(m_continue_read_thread, wait_mutex, 10);
         SDL_UnlockMutex(wait_mutex);
         continue;
      } else {
         m_eof = false;
      }

      if (pkt->stream_index == m_audio_stream) {
         m_audioq.put(pkt);
      } else if (pkt->stream_index == m_video_stream && !(m_video_st->disposition & AV_DISPOSITION_ATTACHED_PIC)) {
         m_videoq.put(pkt);
      } else if (pkt->stream_index == m_subtitle_stream) {
         m_subtitleq.put(pkt);
      } else {
         av_packet_unref(pkt);
      }
   }

   ret = 0;
fail:
   if (ic && !m_ic)
      avformat_close_input(&ic);

   av_packet_free(&pkt);
   m_video_finished = true;
   SDL_DestroyMutex(wait_mutex);
   return 0;
}

int FFmpegVideo::decode_interrupt_cb_wrapper(void *ctx) {
   FFmpegVideo* self = static_cast<FFmpegVideo*>(ctx);
   return self->decode_interrupt_cb();
}

int FFmpegVideo::decode_interrupt_cb() {
   return m_abort_request;
}

void FFmpegVideo::calculate_display_rect(SDL_Rect *rect,
      int scr_xleft, int scr_ytop, int scr_width, int scr_height,
      int pic_width, int pic_height, AVRational pic_sar) {
   AVRational aspect_ratio = pic_sar;
   int64_t width, height, x, y;

   if (av_cmp_q(aspect_ratio, av_make_q(0, 1)) <= 0) {
      aspect_ratio = av_make_q(1, 1);
   }

   aspect_ratio = av_mul_q(aspect_ratio, av_make_q(pic_width, pic_height));

   /* XXX: we suppose the screen has a 1.0 pixel ratio */
   height = scr_height;
   width = av_rescale(height, aspect_ratio.num, aspect_ratio.den) & ~1;
   if (width > scr_width) {
      width = scr_width;
      height = av_rescale(width, aspect_ratio.den, aspect_ratio.num) & ~1;
   }
   x = (scr_width - width) / 2;
   y = (scr_height - height) / 2;
   rect->x = scr_xleft + x;
   rect->y = scr_ytop  + y;
   rect->w = FFMAX((int)width,  1);
   rect->h = FFMAX((int)height, 1);
}

/* open a given stream. Return 0 if OK */
int FFmpegVideo::stream_component_open(int stream_index) {
   AVFormatContext *ic = m_ic;
   AVCodecContext *avctx;
   const AVCodec *codec;
   AVDictionary *opts = NULL;
   AVDictionaryEntry *t = NULL;
   int sample_rate, nb_channels;
   int64_t channel_layout;
   int ret = 0;
   int stream_lowres = 0;

   if (stream_index < 0 || stream_index >= static_cast<int>(ic->nb_streams)) {
      return -1;
   }

   avctx = avcodec_alloc_context3(NULL);
   if (!avctx) {
      return AVERROR(ENOMEM);
   }

   ret = avcodec_parameters_to_context(avctx, ic->streams[stream_index]->codecpar);
   if (ret < 0)
      goto fail;
   avctx->pkt_timebase = ic->streams[stream_index]->time_base;

   codec = avcodec_find_decoder(avctx->codec_id);

   switch(avctx->codec_type){
      case AVMEDIA_TYPE_AUDIO   : m_last_audio_stream    = stream_index; break;
      case AVMEDIA_TYPE_SUBTITLE: m_last_subtitle_stream = stream_index; break;
      case AVMEDIA_TYPE_VIDEO   : m_last_video_stream    = stream_index; break;
      default: break;
   }
   if (!codec) {
      SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "No decoder could be found for codec %s", avcodec_get_name(avctx->codec_id));
      ret = AVERROR(EINVAL);
      goto fail;
   }

   avctx->codec_id = codec->id;
   if (stream_lowres > codec->max_lowres) {
      SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "The maximum value for lowres supported by the decoder is %d",
               codec->max_lowres);
      stream_lowres = codec->max_lowres;
   }
   avctx->lowres = stream_lowres;

   if ((ret = avcodec_open2(avctx, codec, NULL)) < 0) {
      goto fail;
   }
   if ((t = av_dict_get(opts, "", NULL, AV_DICT_IGNORE_SUFFIX))) {
      SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Option %s not found.", t->key);
      ret =  AVERROR_OPTION_NOT_FOUND;
      goto fail;
   }

   m_eof = false;
   ic->streams[stream_index]->discard = AVDISCARD_DEFAULT;
   switch (avctx->codec_type) {
   case AVMEDIA_TYPE_AUDIO:
      sample_rate    = avctx->sample_rate;
      nb_channels    = avctx->channels;
      channel_layout = static_cast<int64_t>(avctx->channel_layout);

      /* prepare audio output */
      if ((ret = audio_open(channel_layout, nb_channels, sample_rate, &m_audio_tgt)) < 0) {
         goto fail;
      }
      m_audio_hw_buf_size = ret;
      m_audio_src = m_audio_tgt;
      m_audio_buf_size  = 0;
      m_audio_buf_index = 0;

      /* init averaging filter */
      m_audio_diff_avg_coef  = exp(log(0.01) / AUDIO_DIFF_AVG_NB);
      m_audio_diff_avg_count = 0;
      /* since we do not have a precise anough audio FIFO fullness,
         we correct audio sync only if larger than this threshold */
      m_audio_diff_threshold = (double)(m_audio_hw_buf_size) / m_audio_tgt.bytes_per_sec;

      m_audio_stream = stream_index;
      m_audio_st = ic->streams[stream_index];

      if ((ret = m_auddec.init(avctx, &m_audioq, m_continue_read_thread)) < 0)
         goto fail;
      if ((m_ic->iformat->flags & (AVFMT_NOBINSEARCH | AVFMT_NOGENSEARCH | AVFMT_NO_BYTE_SEEK)) && !m_ic->iformat->read_seek) {
         m_auddec.setStartPts(m_audio_st->start_time);
         m_auddec.setStartPtsTb(m_audio_st->time_base);
      }
      if ((ret = m_auddec.start(audio_thread_wrapper, "audio_decoder", this)) < 0) {
         goto out;
      }
      SDL_ResumeAudioDevice(SDL_GetAudioStreamDevice(m_hw_audio_stream));
      break;
   case AVMEDIA_TYPE_VIDEO:
      m_video_stream = stream_index;
      m_video_st = ic->streams[stream_index];

      if ((ret = m_viddec.init(avctx, &m_videoq, m_continue_read_thread)) < 0) {
         goto fail;
      }
      if ((ret = m_viddec.start(video_thread_wrapper, "video_decoder", this)) < 0) {
         goto out;
      }
      m_queue_attachments_req = 1;
      break;
   case AVMEDIA_TYPE_SUBTITLE:
      m_subtitle_stream = stream_index;
      m_subtitle_st = ic->streams[stream_index];

      if ((ret = m_subdec.init(avctx, &m_subtitleq, m_continue_read_thread)) < 0) {
         goto fail;
      }
      if ((ret = m_subdec.start(subtitle_thread_wrapper, "subtitle_decoder", this)) < 0) {
         goto out;
      }
      break;
   default:
      break;
   }
   goto out;

fail:
   avcodec_free_context(&avctx);
out:
   av_dict_free(&opts);

   return ret;
}

int FFmpegVideo::audio_open(int64_t wanted_channel_layout, int wanted_nb_channels, int wanted_sample_rate, struct AudioParams *audio_hw_params) {
   SDL_AudioSpec wanted_spec, spec;
   const char *env;
   static const int next_nb_channels[] = {0, 0, 1, 6, 2, 6, 4, 6};
   static const int next_sample_rates[] = {0, 44100, 48000, 96000, 192000};
   int next_sample_rate_idx = FF_ARRAY_ELEMS(next_sample_rates) - 1;

   env = SDL_getenv("SDL_AUDIO_CHANNELS");
   if (env) {
      wanted_nb_channels = atoi(env);
      wanted_channel_layout = av_get_default_channel_layout(wanted_nb_channels);
   }
   if (!wanted_channel_layout || wanted_nb_channels != av_get_channel_layout_nb_channels(static_cast<uint64_t>(wanted_channel_layout))) {
      wanted_channel_layout = av_get_default_channel_layout(wanted_nb_channels);
      wanted_channel_layout &= ~AV_CH_LAYOUT_STEREO_DOWNMIX;
   }
   wanted_nb_channels = av_get_channel_layout_nb_channels(static_cast<uint64_t>(wanted_channel_layout));
   wanted_spec.channels = wanted_nb_channels;
   wanted_spec.freq = wanted_sample_rate;
   if (wanted_spec.freq <= 0 || wanted_spec.channels <= 0) {
      SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Invalid sample rate or channel count!");
      return -1;
   }
   while (next_sample_rate_idx && next_sample_rates[next_sample_rate_idx] >= wanted_spec.freq) {
      next_sample_rate_idx--;
   }
   wanted_spec.format = SDL_AUDIO_S16;
   while (!(m_hw_audio_stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &wanted_spec, sdl_new_audio_callback, this))) {
      SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "SDL_OpenAudio (%d channels, %d Hz): %s",
            wanted_spec.channels, wanted_spec.freq, SDL_GetError());
      wanted_spec.channels = next_nb_channels[FFMIN(7, wanted_spec.channels)];
      if (!wanted_spec.channels) {
         wanted_spec.freq = next_sample_rates[next_sample_rate_idx--];
         wanted_spec.channels = wanted_nb_channels;
         if (!wanted_spec.freq) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "No more combinations to try, audio open failed");
            return -1;
         }
      }
      wanted_channel_layout = av_get_default_channel_layout(wanted_spec.channels);
   }
   SDL_GetAudioStreamFormat(m_hw_audio_stream, NULL, &spec);
   if (spec.format != SDL_AUDIO_S16) {
      SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL advised audio format %d is not supported!", spec.format);
      return -1;
   }
   if (spec.channels != wanted_spec.channels) {
      wanted_channel_layout = av_get_default_channel_layout(spec.channels);
      if (!wanted_channel_layout) {
         SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL advised channel count %d is not supported!", spec.channels);
         return -1;
      }
   }

   audio_hw_params->fmt = AV_SAMPLE_FMT_S16;
   audio_hw_params->freq = spec.freq;
   audio_hw_params->channel_layout = wanted_channel_layout;
   audio_hw_params->channels =  spec.channels;
   audio_hw_params->frame_size = av_samples_get_buffer_size(NULL, audio_hw_params->channels, 1, audio_hw_params->fmt, 1);
   audio_hw_params->bytes_per_sec = av_samples_get_buffer_size(NULL, audio_hw_params->channels, audio_hw_params->freq, audio_hw_params->fmt, 1);
   if (audio_hw_params->bytes_per_sec <= 0 || audio_hw_params->frame_size <= 0) {
      SDL_LogError(SDL_LOG_CATEGORY_ERROR, "av_samples_get_buffer_size failed");
      return -1;
   }
   return 8192; // Buffer size hard coded in SDL source.
   // Samples * channels * 2 bytes per sample.
   // return FFMAX(SDL_AUDIO_MIN_BUFFER_SIZE, 2 << av_log2(wanted_spec.freq / SDL_AUDIO_MAX_CALLBACKS_PER_SEC)) * spec.channels * 2;
}

void FFmpegVideo::sdl_new_audio_callback(void *userdata, SDL_AudioStream *stream, int additional_amount, int)
{
   /* Calculate a little more audio here, maybe using `userdata`, write it to `stream`
   *
   * If you want to use the original callback, you could do something like this:
   */
   if (additional_amount > 0) {
      Uint8 *data = SDL_stack_alloc(Uint8, static_cast<size_t>(additional_amount));
      if (data) {
         FFmpegVideo* self = static_cast<FFmpegVideo*>(userdata);
         self->sdl_audio_callback(data, additional_amount);
         SDL_PutAudioStreamData(stream, data, additional_amount);
         SDL_stack_free(data);
      }
   }
}

/* prepare a new audio buffer */
void FFmpegVideo::sdl_audio_callback(Uint8* stream, int len)
{
   int audio_size, len1;

   m_audio_callback_time = av_gettime_relative();

   while (len > 0) {
      if (m_audio_buf_index >= m_audio_buf_size) {
         audio_size = audio_decode_frame();
         if (audio_size < 0) {
               /* if error, just output silence */
            m_audio_buf = NULL;
            m_audio_buf_size = static_cast<unsigned>(len);
         } else {
            m_audio_buf_size = static_cast<unsigned>(audio_size);
         }
         m_audio_buf_index = 0;
      }
      len1 = static_cast<int>(m_audio_buf_size - m_audio_buf_index);
      if (len1 > len)
         len1 = len;
      if (m_audio_buf)
         memcpy(stream, (uint8_t *)m_audio_buf + m_audio_buf_index, static_cast<size_t>(len1));
      else {
         memset(stream, 0, static_cast<size_t>(len1));
      }
      len -= len1;
      stream += len1;
      m_audio_buf_index += static_cast<unsigned>(len1);
   }
   m_audio_write_buf_size = static_cast<int>(m_audio_buf_size) - static_cast<int>(m_audio_buf_index);
   /* Let's assume the audio driver that is used by SDL has two periods. */
   if (!isnan(m_audio_clock)) {
      m_audclk.set_at(m_audio_clock - (double)(2 * m_audio_hw_buf_size + m_audio_write_buf_size) / m_audio_tgt.bytes_per_sec, m_audio_clock_serial, m_audio_callback_time / 1000000.0);
      m_extclk.sync_to_slave(m_audclk);
   }
}

/**
 * Decode one audio frame and return its uncompressed size.
 *
 * The processed audio frame is decoded, converted if required, and
 * stored in is->audio_buf, with size in bytes given by the return
 * value.
 */
int FFmpegVideo::audio_decode_frame() {
   int data_size, resampled_data_size;
   int64_t dec_channel_layout;
   av_unused double audio_clock0;
   int wanted_nb_samples;
   Frame *af;

   do {
#if defined(_WIN32)
      while (frame_queue_nb_remaining(&m_sampq) == 0) {
         if ((av_gettime_relative() - m_audio_callback_time) > 1000000LL * m_audio_hw_buf_size / m_audio_tgt.bytes_per_sec / 2) {
            return -1;
         }
         av_usleep(1000);
      }
#endif
      if (!(af = m_sampq.peek_readable())) {
         return -1;
      }
      m_sampq.next();
   } while (af->serial != m_audioq.getSerial());

   data_size = av_samples_get_buffer_size(NULL, af->frame->channels,
      af->frame->nb_samples, static_cast<AVSampleFormat>(af->frame->format), 1);

   dec_channel_layout =
      (af->frame->channel_layout && af->frame->channels == av_get_channel_layout_nb_channels(af->frame->channel_layout)) ?
      static_cast<int64_t>(af->frame->channel_layout) : av_get_default_channel_layout(af->frame->channels);
   wanted_nb_samples = synchronize_audio(af->frame->nb_samples);

   if (af->frame->format       != m_audio_src.fmt            ||
      dec_channel_layout       != m_audio_src.channel_layout ||
      af->frame->sample_rate   != m_audio_src.freq           ||
      (wanted_nb_samples       != af->frame->nb_samples && !m_swr_ctx)) {
      swr_free(&m_swr_ctx);
      m_swr_ctx = swr_alloc_set_opts(NULL,
         m_audio_tgt.channel_layout, m_audio_tgt.fmt, m_audio_tgt.freq,
         dec_channel_layout, static_cast<AVSampleFormat>(af->frame->format), af->frame->sample_rate,
         0, NULL);
      if (!m_swr_ctx || swr_init(m_swr_ctx) < 0) {
         SDL_LogError(SDL_LOG_CATEGORY_ERROR,
            "Cannot create sample rate converter for conversion of %d Hz %s %d channels to %d Hz %s %d channels!",
            af->frame->sample_rate, av_get_sample_fmt_name(static_cast<AVSampleFormat>(af->frame->format)), af->frame->channels,
            m_audio_tgt.freq, av_get_sample_fmt_name(m_audio_tgt.fmt), m_audio_tgt.channels);
         swr_free(&m_swr_ctx);
         return -1;
      }
      m_audio_src.channel_layout = dec_channel_layout;
      m_audio_src.channels       = af->frame->channels;
      m_audio_src.freq = af->frame->sample_rate;
      m_audio_src.fmt = static_cast<AVSampleFormat>(af->frame->format);
   }

   if (m_swr_ctx) {
      const uint8_t **in = (const uint8_t **)af->frame->extended_data;
      uint8_t **out = &m_audio_buf1;
      int out_count = (int64_t)wanted_nb_samples * m_audio_tgt.freq / af->frame->sample_rate + 256;
      int out_size  = av_samples_get_buffer_size(NULL, m_audio_tgt.channels, out_count, m_audio_tgt.fmt, 0);
      int len2;
      if (out_size < 0) {
         SDL_LogError(SDL_LOG_CATEGORY_ERROR, "av_samples_get_buffer_size() failed");
         return -1;
      }
      if (wanted_nb_samples != af->frame->nb_samples) {
         if (swr_set_compensation(m_swr_ctx, (wanted_nb_samples - af->frame->nb_samples) * m_audio_tgt.freq / af->frame->sample_rate,
                                       wanted_nb_samples * m_audio_tgt.freq / af->frame->sample_rate) < 0) {
               SDL_LogError(SDL_LOG_CATEGORY_ERROR, "swr_set_compensation() failed");
               return -1;
         }
      }
      av_fast_malloc(&m_audio_buf1, &m_audio_buf1_size, static_cast<size_t>(out_size));
      if (!m_audio_buf1)
         return AVERROR(ENOMEM);
      len2 = swr_convert(m_swr_ctx, out, out_count, in, af->frame->nb_samples);
      if (len2 < 0) {
         SDL_LogError(SDL_LOG_CATEGORY_ERROR, "swr_convert() failed");
         return -1;
      }
      if (len2 == out_count) {
         SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "audio buffer is probably too small");
         if (swr_init(m_swr_ctx) < 0) {
            swr_free(&m_swr_ctx);
         }
      }
      m_audio_buf = m_audio_buf1;
      resampled_data_size = len2 * m_audio_tgt.channels * av_get_bytes_per_sample(m_audio_tgt.fmt);
   } else {
      m_audio_buf = af->frame->data[0];
      resampled_data_size = data_size;
   }

   audio_clock0 = m_audio_clock;
   /* update the audio clock with the pts */
   if (!isnan(af->pts)) {
      m_audio_clock = af->pts + (double) af->frame->nb_samples / af->frame->sample_rate;
   } else {
      m_audio_clock = NAN;
   }
   m_audio_clock_serial = af->serial;
#ifdef DEBUG
   {
      static double last_clock;
      printf("audio: delay=%0.3f clock=%0.3f clock0=%0.3f\n",
            m_audio_clock - last_clock,
            m_audio_clock, audio_clock0);
      last_clock = m_audio_clock;
   }
#endif
   return resampled_data_size;
}

/* return the wanted number of samples to get better sync if sync_type is video
 * or external master clock */
int FFmpegVideo::synchronize_audio(int nb_samples) {
   int wanted_nb_samples = nb_samples;

   /* if not master, then we try to remove or add samples to correct the clock */
   if (get_master_sync_type() != AV_SYNC_AUDIO_MASTER) {
      double diff, avg_diff;
      int min_nb_samples, max_nb_samples;

      diff = m_audclk.get() - get_master_clock();

      if (!isnan(diff) && fabs(diff) < AV_NOSYNC_THRESHOLD) {
         m_audio_diff_cum = diff + m_audio_diff_avg_coef * m_audio_diff_cum;
         if (m_audio_diff_avg_count < AUDIO_DIFF_AVG_NB) {
            /* not enough measures to have a correct estimate */
            m_audio_diff_avg_count++;
         } else {
            /* estimate the A-V difference */
            avg_diff = m_audio_diff_cum * (1.0 - m_audio_diff_avg_coef);

            if (fabs(avg_diff) >= m_audio_diff_threshold) {
               wanted_nb_samples = nb_samples + (int)(diff * m_audio_src.freq);
               min_nb_samples = ((nb_samples * (100 - SAMPLE_CORRECTION_PERCENT_MAX) / 100));
               max_nb_samples = ((nb_samples * (100 + SAMPLE_CORRECTION_PERCENT_MAX) / 100));
               wanted_nb_samples = av_clip(wanted_nb_samples, min_nb_samples, max_nb_samples);
            }
            SDL_LogTrace(SDL_LOG_CATEGORY_APPLICATION, "diff=%f adiff=%f sample_diff=%d apts=%0.3f %f",
               diff, avg_diff, wanted_nb_samples - nb_samples,
               m_audio_clock, m_audio_diff_threshold);
         }
      } else {
         /* too big difference : may be initial PTS errors, so reset A-V filter */
         m_audio_diff_avg_count = 0;
         m_audio_diff_cum       = 0;
      }
   }

   return wanted_nb_samples;
}

int FFmpegVideo::get_master_sync_type() {
   if (m_av_sync_type == AV_SYNC_VIDEO_MASTER) {
      if (m_video_st)
         return AV_SYNC_VIDEO_MASTER;
      else
         return AV_SYNC_AUDIO_MASTER;
   } else if (m_av_sync_type == AV_SYNC_AUDIO_MASTER) {
      if (m_audio_st)
         return AV_SYNC_AUDIO_MASTER;
      else
         return AV_SYNC_EXTERNAL_CLOCK;
   } else {
      return AV_SYNC_EXTERNAL_CLOCK;
   }
}

int FFmpegVideo::audio_thread_wrapper(void *arg) {
   FFmpegVideo* self = static_cast<FFmpegVideo*>(arg);
   return self->audio_thread();
}

int FFmpegVideo::audio_thread() {
   AVFrame *frame {av_frame_alloc()};
   Frame *af;
   int got_frame {0};
   AVRational tb;
   int ret {0};

   if (!frame) {
      return AVERROR(ENOMEM);
   }

   do {
      if ((got_frame = m_auddec.decode_frame(frame, NULL)) < 0) {
         goto the_end;
      }

      if (got_frame) {
         tb = {1, frame->sample_rate};

         if (!(af = m_sampq.peek_writable())) {
            goto the_end;
         }

         af->pts = (frame->pts == AV_NOPTS_VALUE) ? NAN : frame->pts * av_q2d(tb);
         af->pos = frame->pkt_pos;
         af->serial = m_auddec.getPktSerial();
         af->duration = av_q2d({frame->nb_samples, frame->sample_rate});

         av_frame_move_ref(af->frame, frame);
         m_sampq.push();
      }
   } while (ret >= 0 || ret == AVERROR(EAGAIN) || ret == AVERROR_EOF);
the_end:
   av_frame_free(&frame);
   return ret;
}

int FFmpegVideo::video_thread_wrapper(void *arg) {
   FFmpegVideo* self = static_cast<FFmpegVideo*>(arg);
   return self->video_thread();
}

int FFmpegVideo::video_thread() {
   AVFrame *frame {av_frame_alloc()};
   double pts;
   double duration;
   int ret;
   AVRational tb {m_video_st->time_base};
   AVRational frame_rate = av_guess_frame_rate(m_ic, m_video_st, NULL);

   if (!frame) {
      return AVERROR(ENOMEM);
   }

   for (;;) {
      ret = get_video_frame(frame);
      if (ret < 0) {
         goto the_end;
      }
      if (!ret) {
         continue;
      }

      duration = (frame_rate.num && frame_rate.den ? av_q2d({frame_rate.den, frame_rate.num}) : 0);
      pts = (frame->pts == AV_NOPTS_VALUE) ? NAN : frame->pts * av_q2d(tb);
      ret = queue_picture(frame, pts, duration, frame->pkt_pos, m_viddec.getPktSerial());
      av_frame_unref(frame);

      if (ret < 0)
         goto the_end;
   }
the_end:
   av_frame_free(&frame);
   return 0;
}

int FFmpegVideo::get_video_frame(AVFrame *frame) {
   int got_picture;

   if ((got_picture = m_viddec.decode_frame(frame, NULL)) < 0) {
      return -1;
   }

   if (got_picture) {
      double dpts {NAN};

      if (frame->pts != AV_NOPTS_VALUE) {
         dpts = av_q2d(m_video_st->time_base) * frame->pts;
      }

      frame->sample_aspect_ratio = av_guess_sample_aspect_ratio(m_ic, m_video_st, frame);

      if (frame->pts != AV_NOPTS_VALUE) {
         double diff = dpts - get_master_clock();
         if (!isnan(diff) && fabs(diff) < AV_NOSYNC_THRESHOLD &&
               diff - m_frame_last_filter_delay < 0 &&
               m_viddec.getPktSerial() == m_vidclk.getSerial() &&
               m_videoq.getPacketCount()) {
            m_frame_drops_early++;
            av_frame_unref(frame);
            got_picture = 0;
         }
      }
   }

   return got_picture;
}

int FFmpegVideo::queue_picture(AVFrame *src_frame, double pts, double duration, int64_t pos, int serial) {
   Frame *vp;

#if defined(DEBUG_SYNC)
   printf("frame_type=%c pts=%0.3f\n",
         av_get_picture_type_char(src_frame->pict_type), pts);
#endif

   if (!(vp = m_pictq.peek_writable())) {
      return -1;
   }

   vp->sar = src_frame->sample_aspect_ratio;
   vp->uploaded = false;

   vp->width = src_frame->width;
   vp->height = src_frame->height;
   vp->format = src_frame->format;

   vp->pts = pts;
   vp->duration = duration;
   vp->pos = pos;
   vp->serial = serial;

   av_frame_move_ref(vp->frame, src_frame);
   m_pictq.push();
   return 0;
}

int FFmpegVideo::subtitle_thread_wrapper(void *arg) {
   FFmpegVideo* self = static_cast<FFmpegVideo*>(arg);
   return self->subtitle_thread();
}

int FFmpegVideo::subtitle_thread() {
   Frame *sp;
   int got_subtitle;
   double pts;

   for (;;) {
      if (!(sp = m_subpq.peek_writable())) {
         return 0;
      }

      if ((got_subtitle = m_subdec.decode_frame(NULL, &sp->sub)) < 0) {
         break;
      }

      pts = 0;

      if (got_subtitle && sp->sub.format == 0) {
         if (sp->sub.pts != AV_NOPTS_VALUE) {
            pts = sp->sub.pts / (double)AV_TIME_BASE;
         }
         sp->pts = pts;
         sp->serial = m_subdec.getPktSerial();
         sp->width = m_subdec.getCodecContext()->width;
         sp->height = m_subdec.getCodecContext()->height;
         sp->uploaded = false;

         /* now we can update the picture count */
         m_subpq.push();
      } else if (got_subtitle) {
         avsubtitle_free(&sp->sub);
      }
   }
   return 0;
}

int FFmpegVideo::stream_has_enough_packets(AVStream *st, int stream_id, PacketQueue *queue) {
   return stream_id < 0 ||
         queue->isAbortRequested() ||
         (st->disposition & AV_DISPOSITION_ATTACHED_PIC) ||
         (queue->getPacketCount() > MIN_FRAMES && (!queue->getDuration() || av_q2d(st->time_base) * queue->getDuration() > 1.0));
}

void FFmpegVideo::stream_component_close(int stream_index) {
   AVCodecParameters* codecpar;

   if (stream_index < 0 || stream_index >= static_cast<int>(m_ic->nb_streams)) {
      return;
   }
   codecpar = m_ic->streams[stream_index]->codecpar;

   switch (codecpar->codec_type) {
   case AVMEDIA_TYPE_AUDIO:
   m_auddec.abort(&m_sampq);
      SDL_DestroyAudioStream(m_hw_audio_stream);
      m_auddec.destroy();
      swr_free(&m_swr_ctx);
      av_freep(&m_audio_buf1);
      m_audio_buf1_size = 0;
      m_audio_buf = NULL;
      break;
   case AVMEDIA_TYPE_VIDEO:
      m_viddec.abort(&m_pictq);
      m_viddec.destroy();
      break;
   case AVMEDIA_TYPE_SUBTITLE:
      m_subdec.abort(&m_subpq);
      m_subdec.destroy();
      break;
   default:
      break;
   }

   m_ic->streams[stream_index]->discard = AVDISCARD_ALL;
   switch (codecpar->codec_type) {
   case AVMEDIA_TYPE_AUDIO:
      m_audio_st = NULL;
      m_audio_stream = -1;
      break;
   case AVMEDIA_TYPE_VIDEO:
      m_video_st = NULL;
      m_video_stream = -1;
      break;
   case AVMEDIA_TYPE_SUBTITLE:
      m_subtitle_st = NULL;
      m_subtitle_stream = -1;
      break;
   default:
      break;
   }
}

double FFmpegVideo::compute_target_delay(double delay) {
   double sync_threshold, diff = 0;

   /* update delay to follow master synchronisation source */
   if (get_master_sync_type() != AV_SYNC_VIDEO_MASTER) {
      /* if video is slave, we try to correct big delays by
         duplicating or deleting a frame */
      diff = m_vidclk.get() - get_master_clock();

      /* skip or repeat frame. We take into account the
         delay to compute the threshold. I still don't know
         if it is the best guess */
      sync_threshold = FFMAX(AV_SYNC_THRESHOLD_MIN, FFMIN(AV_SYNC_THRESHOLD_MAX, delay));
      if (!isnan(diff) && fabs(diff) < m_max_frame_duration) {
         if (diff <= -sync_threshold)
               delay = FFMAX(0, delay + diff);
         else if (diff >= sync_threshold && delay > AV_SYNC_FRAMEDUP_THRESHOLD)
               delay = delay + diff;
         else if (diff >= sync_threshold)
               delay = 2 * delay;
      }
   }

   SDL_LogTrace(SDL_LOG_CATEGORY_APPLICATION, "video: delay=%0.3f A-V=%f", delay, -diff);

   return delay;
}

/* get the current master clock value */
double FFmpegVideo::get_master_clock() {
   double val;

   switch (get_master_sync_type()) {
      case AV_SYNC_VIDEO_MASTER:
         val = m_vidclk.get();
         break;
      case AV_SYNC_AUDIO_MASTER:
         val = m_audclk.get();
         break;
      default:
         val = m_extclk.get();
         break;
   }
   return val;
}

double FFmpegVideo::vp_duration(Frame *vp, Frame *nextvp) {
   if (vp->serial == nextvp->serial) {
      double duration = nextvp->pts - vp->pts;
      if (isnan(duration) || duration <= 0 || duration > m_max_frame_duration) {
         return vp->duration;
      } else {
         return duration;
      }
   } else {
      return 0.0;
   }
}

void FFmpegVideo::update_video_pts(double pts, int serial) {
   /* update current video pts */
   m_vidclk.set(pts, serial);
   m_extclk.sync_to_slave(m_vidclk);
}

void FFmpegVideo::print_error(const char *filename, int err) {
   char errbuf[128];
   const char *errbuf_ptr = errbuf;

   if (av_strerror(err, errbuf, sizeof(errbuf)) < 0) {
      errbuf_ptr = strerror(AVUNERROR(err));
   }
   SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s: %s", filename, errbuf_ptr);
}

void* FFmpegVideo::open_library(const char* filename) {
   void* handle {dlopen(filename, RTLD_LAZY)};
   if (!handle) {
      fputs(dlerror(), stderr);
      throw "Couldn't open shared library.";
   }
   return handle;
}

void* FFmpegVideo::library_function_address(void* handle, const char* symbol, void** fun) {
   *fun = dlsym(handle, symbol);
   char* error;
   if ((error = dlerror()) != NULL)  {
      fputs(error, stderr);
      throw "Couldn't resolve library symbol.";
   }
   return fun;
}

void FFmpegVideo::close_library(void* handle) {
   if (handle) {
      dlclose(handle);
   }
}