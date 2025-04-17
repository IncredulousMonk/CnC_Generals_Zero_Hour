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

///// FrameQueue.h ///////////////////////////////
// Matthew Gill, April 2025
//////////////////////////////////////////////////

#ifndef FRAME_QUEUE_H
#define FRAME_QUEUE_H

extern "C" {
   #include "libavformat/avformat.h"
}

#include <SDL3/SDL.h>

class PacketQueue;

#define VIDEO_PICTURE_QUEUE_SIZE 3
#define SUBPICTURE_QUEUE_SIZE 16
#define SAMPLE_QUEUE_SIZE 9
#define FRAME_QUEUE_SIZE FFMAX(SAMPLE_QUEUE_SIZE, FFMAX(VIDEO_PICTURE_QUEUE_SIZE, SUBPICTURE_QUEUE_SIZE))

/* Common struct for handling all types of decoded data and allocated render buffers. */
struct Frame {
   AVFrame *frame;
   AVSubtitle sub;
   int serial;
   double pts;           /* presentation timestamp for the frame */
   double duration;      /* estimated duration of the frame */
   int64_t pos;          /* byte position of the frame in the input file */
   int width;
   int height;
   int format;
   AVRational sar;
   bool uploaded;
   int flip_v;
};

class FrameQueue {
private:
   Frame m_queue[FRAME_QUEUE_SIZE] {};
   int m_rindex {};
   int m_windex {};
   int m_size {};
   int m_max_size {};
   bool m_keep_last {};
   int m_rindex_shown {};
   SDL_Mutex* m_mutex {};
   SDL_Condition* m_cond {};
   PacketQueue* m_pktq {};
public:
   FrameQueue() {};
   ~FrameQueue() {};

   // No copies allowed!
   FrameQueue(const FrameQueue&) = delete;
   FrameQueue& operator=(const FrameQueue&) = delete;

   int init(PacketQueue *pktq, int max_size, bool keep_last);
   void signal();
   Frame* peek();
   Frame* peek_next();
   Frame* peek_last();
   Frame* peek_writable();
   Frame* peek_readable();
   void push();
   void next();
   int nb_remaining();
   void lock();
   void unlock();
   void unref_item(Frame *vp);
   void destroy();

   int getRindexShown() const { return m_rindex_shown; }
};

#endif
