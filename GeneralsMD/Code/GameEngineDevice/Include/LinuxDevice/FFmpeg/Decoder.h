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

///// Decoder.h //////////////////////////////////
// Matthew Gill, April 2025
//////////////////////////////////////////////////

#ifndef DECODER_H
#define DECODER_H

extern "C" {
#include "libavformat/avformat.h"
}

#include <SDL3/SDL.h>

class PacketQueue;
class FrameQueue;

class Decoder {
private:
   AVPacket* m_pkt {};
   PacketQueue* m_queue {};
   AVCodecContext* m_avctx {};
   int m_pkt_serial {};
   int m_finished {};
   bool m_packet_pending {};
   SDL_Condition* m_empty_queue_cond {};
   int64_t m_start_pts {};
   AVRational m_start_pts_tb {};
   int64_t m_next_pts {};
   AVRational m_next_pts_tb {};
   SDL_Thread* m_decoder_tid {};
public:
   Decoder() {};
   ~Decoder() {};

   // No copies allowed!
   Decoder(const Decoder&) = delete;
   Decoder& operator=(const Decoder&) = delete;

   int init(AVCodecContext *avctx, PacketQueue *queue, SDL_Condition *empty_queue_cond);
   int start(int (*fn)(void *), const char *thread_name, void* arg);
   void abort(FrameQueue *fq);
   int decode_frame(AVFrame *frame, AVSubtitle *sub);
   void destroy();

   AVCodecContext* getCodecContext() const { return m_avctx; }
   int getPktSerial() const { return m_pkt_serial; }
   int getFinished() const { return m_finished; }
   void setStartPts(int64_t pts) { m_start_pts = pts; }
   void setStartPtsTb(AVRational pts_tb) { m_start_pts_tb = pts_tb; }
};

#endif
