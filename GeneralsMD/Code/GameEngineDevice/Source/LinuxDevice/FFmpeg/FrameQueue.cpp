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

///// FrameQueue.cpp /////////////////////////////
// Matthew Gill, April 2025
//////////////////////////////////////////////////

#include "LinuxDevice/FFmpeg/FrameQueue.h"
#include "LinuxDevice/FFmpeg/PacketQueue.h"

extern "C" {
#include "libavutil/time.h"
}

int FrameQueue::init(PacketQueue *pktq, int max_size, bool keep_last) {
   int i;
   if (!(m_mutex = SDL_CreateMutex())) {
      SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "SDL_CreateMutex(): %s", SDL_GetError());
      return AVERROR(ENOMEM);
   }
   if (!(m_cond = SDL_CreateCondition())) {
      SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "SDL_CreateCondition(): %s", SDL_GetError());
      return AVERROR(ENOMEM);
   }
   m_pktq = pktq;
   m_max_size = FFMIN(max_size, FRAME_QUEUE_SIZE);
   m_keep_last = keep_last;
   for (i = 0; i < m_max_size; i++) {
      if (!(m_queue[i].frame = av_frame_alloc())) {
         return AVERROR(ENOMEM);
      }
   }
   return 0;
}

void FrameQueue::signal() {
   SDL_LockMutex(m_mutex);
   SDL_SignalCondition(m_cond);
   SDL_UnlockMutex(m_mutex);
}

Frame* FrameQueue::peek() {
   return &m_queue[(m_rindex + m_rindex_shown) % m_max_size];
}

Frame* FrameQueue::peek_next() {
   return &m_queue[(m_rindex + m_rindex_shown + 1) % m_max_size];
}

Frame* FrameQueue::peek_last() {
   return &m_queue[m_rindex];
}

Frame* FrameQueue::peek_writable() {
   /* wait until we have space to put a new frame */
   SDL_LockMutex(m_mutex);
   while (m_size >= m_max_size && !m_pktq->isAbortRequested()) {
      SDL_WaitCondition(m_cond, m_mutex);
   }
   SDL_UnlockMutex(m_mutex);

   if (m_pktq->isAbortRequested()) {
      return NULL;
   }

   return &m_queue[m_windex];
}

Frame* FrameQueue::peek_readable() {
   /* wait until we have a readable new frame */
   SDL_LockMutex(m_mutex);
   while (m_size - m_rindex_shown <= 0 && !m_pktq->isAbortRequested()) {
      SDL_WaitCondition(m_cond, m_mutex);
   }
   SDL_UnlockMutex(m_mutex);

   if (m_pktq->isAbortRequested()) {
      return NULL;
   }

   return &m_queue[(m_rindex + m_rindex_shown) % m_max_size];
}

void FrameQueue::push() {
   if (++m_windex == m_max_size) {
      m_windex = 0;
   }
   SDL_LockMutex(m_mutex);
   m_size++;
   SDL_SignalCondition(m_cond);
   SDL_UnlockMutex(m_mutex);
}

void FrameQueue::next() {
   if (m_keep_last && !m_rindex_shown) {
      m_rindex_shown = 1;
      return;
   }
   unref_item(&m_queue[m_rindex]);
   if (++m_rindex == m_max_size) {
      m_rindex = 0;
   }
   SDL_LockMutex(m_mutex);
   m_size--;
   SDL_SignalCondition(m_cond);
   SDL_UnlockMutex(m_mutex);
}

/* return the number of undisplayed frames in the queue */
int FrameQueue::nb_remaining() {
   return m_size - m_rindex_shown;
}

void FrameQueue::lock() {
   SDL_LockMutex(m_mutex);
}

void FrameQueue::unlock() {
   SDL_UnlockMutex(m_mutex);
}

void FrameQueue::unref_item(Frame *vp) {
   av_frame_unref(vp->frame);
   avsubtitle_free(&vp->sub);
}

void FrameQueue::destroy() {
   int i;
   for (i = 0; i < m_max_size; i++) {
      Frame *vp = &m_queue[i];
      unref_item(vp);
      av_frame_free(&vp->frame);
   }
   SDL_DestroyMutex(m_mutex);
   SDL_DestroyCondition(m_cond);
}
