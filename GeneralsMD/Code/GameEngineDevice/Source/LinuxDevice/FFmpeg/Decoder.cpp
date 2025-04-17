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

///// Decoder.cpp ////////////////////////////////
// Matthew Gill, April 2025
//////////////////////////////////////////////////

#include "LinuxDevice/FFmpeg/Decoder.h"
#include "LinuxDevice/FFmpeg/PacketQueue.h"
#include "LinuxDevice/FFmpeg/FrameQueue.h"

int Decoder::init(AVCodecContext *avctx, PacketQueue *queue, SDL_Condition *empty_queue_cond) {
   m_pkt = av_packet_alloc();
   if (!m_pkt) {
      return AVERROR(ENOMEM);
   }
   m_avctx = avctx;
   m_queue = queue;
   m_empty_queue_cond = empty_queue_cond;
   m_start_pts = AV_NOPTS_VALUE;
   m_pkt_serial = -1;
   return 0;
}

int Decoder::start(int (*fn)(void *), const char *thread_name, void* arg) {
   m_queue->start();
   m_decoder_tid = SDL_CreateThread(fn, thread_name, arg);
   if (!m_decoder_tid) {
      SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_CreateThread: %s", SDL_GetError());
      return AVERROR(ENOMEM);
   }
   return 0;
}

void Decoder::abort(FrameQueue *fq) {
   m_queue->abort();
   fq->signal();
   SDL_WaitThread(m_decoder_tid, NULL);
   m_decoder_tid = NULL;
   m_queue->flush();
}

int Decoder::decode_frame(AVFrame *frame, AVSubtitle *sub) {
   int ret = AVERROR(EAGAIN);

   for (;;) {
      if (m_queue->getSerial() == m_pkt_serial) {
         do {
               if (m_queue->isAbortRequested())
                  return -1;

               switch (m_avctx->codec_type) {
                  case AVMEDIA_TYPE_VIDEO:
                     ret = avcodec_receive_frame(m_avctx, frame);
                     if (ret >= 0) {
                        frame->pts = frame->best_effort_timestamp;
                     }
                     break;
                  case AVMEDIA_TYPE_AUDIO:
                     ret = avcodec_receive_frame(m_avctx, frame);
                     if (ret >= 0) {
                        AVRational tb = {1, frame->sample_rate};
                        if (frame->pts != AV_NOPTS_VALUE) {
                           frame->pts = av_rescale_q(frame->pts, m_avctx->pkt_timebase, tb);
                        } else if (m_next_pts != AV_NOPTS_VALUE) {
                           frame->pts = av_rescale_q(m_next_pts, m_next_pts_tb, tb);
                        }
                        if (frame->pts != AV_NOPTS_VALUE) {
                           m_next_pts = frame->pts + frame->nb_samples;
                           m_next_pts_tb = tb;
                        }
                     }
                     break;
                  default:
                     break;
               }
               if (ret == AVERROR_EOF) {
                  m_finished = m_pkt_serial;
                  avcodec_flush_buffers(m_avctx);
                  return 0;
               }
               if (ret >= 0) {
                  return 1;
               }
         } while (ret != AVERROR(EAGAIN));
      }

      do {
         if (m_queue->getPacketCount() == 0) {
            SDL_SignalCondition(m_empty_queue_cond);
         }
         if (m_packet_pending) {
            m_packet_pending = false;
         } else {
            int old_serial = m_pkt_serial;
            if (m_queue->get(m_pkt, 1, &m_pkt_serial) < 0) {
               return -1;
            }
            if (old_serial != m_pkt_serial) {
               avcodec_flush_buffers(m_avctx);
               m_finished = 0;
               m_next_pts = m_start_pts;
               m_next_pts_tb = m_start_pts_tb;
            }
         }
         if (m_queue->getSerial() == m_pkt_serial) {
            break;
         }
         av_packet_unref(m_pkt);
      } while (1);

      if (m_avctx->codec_type == AVMEDIA_TYPE_SUBTITLE) {
         int got_frame = 0;
         ret = avcodec_decode_subtitle2(m_avctx, sub, &got_frame, m_pkt);
         if (ret < 0) {
            ret = AVERROR(EAGAIN);
         } else {
            if (got_frame && !m_pkt->data) {
               m_packet_pending = true;
            }
            ret = got_frame ? 0 : (m_pkt->data ? AVERROR(EAGAIN) : AVERROR_EOF);
         }
         av_packet_unref(m_pkt);
      } else {
         if (avcodec_send_packet(m_avctx, m_pkt) == AVERROR(EAGAIN)) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Receive_frame and send_packet both returned EAGAIN, which is an API violation.");
            m_packet_pending = true;
         } else {
            av_packet_unref(m_pkt);
         }
      }
   }
}

void Decoder::destroy() {
   av_packet_free(&m_pkt);
   avcodec_free_context(&m_avctx);
}
