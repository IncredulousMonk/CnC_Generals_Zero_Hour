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

///// PacketQueue.cpp ////////////////////////////
// Matthew Gill, April 2025
//////////////////////////////////////////////////

#include "LinuxDevice/FFmpeg/PacketQueue.h"

extern "C" {
#include "libavutil/time.h"
}

/* packet queue handling */
int PacketQueue::init() {
   m_pkt_list = av_fifo_alloc(sizeof(Packet));
   if (!m_pkt_list) {
      return AVERROR(ENOMEM);
   }
   m_mutex = SDL_CreateMutex();
   if (!m_mutex) {
      SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "SDL_CreateMutex(): %s", SDL_GetError());
      return AVERROR(ENOMEM);
   }
   m_cond = SDL_CreateCondition();
   if (!m_cond) {
      SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "SDL_CreateCondition(): %s", SDL_GetError());
      return AVERROR(ENOMEM);
   }
   m_abort_requested = true;
   return 0;
}

void PacketQueue::start() {
   SDL_LockMutex(m_mutex);
   m_abort_requested = false;
   m_serial++;
   SDL_UnlockMutex(m_mutex);
}

void PacketQueue::abort() {
   SDL_LockMutex(m_mutex);
   m_abort_requested = true;
   SDL_SignalCondition(m_cond);
   SDL_UnlockMutex(m_mutex);
}

int PacketQueue::put_nullpacket(AVPacket *pkt, int stream_index) {
   pkt->stream_index = stream_index;
   return put(pkt);
}

int PacketQueue::put(AVPacket *pkt) {
   AVPacket *pkt1;
   int ret;

   pkt1 = av_packet_alloc();
   if (!pkt1) {
      av_packet_unref(pkt);
      return -1;
   }
   av_packet_move_ref(pkt1, pkt);

   SDL_LockMutex(m_mutex);
   ret = put_private(pkt1);
   SDL_UnlockMutex(m_mutex);

   if (ret < 0)
      av_packet_free(&pkt1);

   return ret;
}

int PacketQueue::put_private(AVPacket *pkt)
{
   Packet pkt1;

   if (m_abort_requested) {
      return -1;
   }

   if (av_fifo_space(m_pkt_list) < static_cast<long>(sizeof(pkt1))) {
      if (av_fifo_grow(m_pkt_list, sizeof(pkt1)) < 0) {
         return -1;
      }
   }

   pkt1.pkt = pkt;
   pkt1.serial = m_serial;

   av_fifo_generic_write(m_pkt_list, &pkt1, sizeof(pkt1), NULL);
   m_nb_packets++;
   m_size += pkt1.pkt->size + static_cast<long>(sizeof(pkt1));
   m_duration += pkt1.pkt->duration;
   /* XXX: should duplicate packet data in DV case */
   SDL_SignalCondition(m_cond);
   return 0;
}

/* return < 0 if aborted, 0 if no packet and > 0 if packet.  */
int PacketQueue::get(AVPacket *pkt, int block, int *serial) {
   Packet pkt1;
   int ret;

   SDL_LockMutex(m_mutex);

   for (;;) {
      if (m_abort_requested) {
         ret = -1;
         break;
      }

      if (av_fifo_size(m_pkt_list) >= static_cast<long>(sizeof(pkt1))) {
         av_fifo_generic_read(m_pkt_list, &pkt1, sizeof(pkt1), NULL);
         m_nb_packets--;
         m_size -= pkt1.pkt->size + static_cast<long>(sizeof(pkt1));
         m_duration -= pkt1.pkt->duration;
         av_packet_move_ref(pkt, pkt1.pkt);
         if (serial) {
            *serial = pkt1.serial;
         }
         av_packet_free(&pkt1.pkt);
         ret = 1;
         break;
      } else if (!block) {
         ret = 0;
         break;
      } else {
         SDL_WaitCondition(m_cond, m_mutex);
      }
   }
   SDL_UnlockMutex(m_mutex);
   return ret;
}

void PacketQueue::flush() {
   Packet pkt1;

   SDL_LockMutex(m_mutex);
   while (av_fifo_size(m_pkt_list) >= static_cast<long>(sizeof(pkt1))) {
      av_fifo_generic_read(m_pkt_list, &pkt1, sizeof(pkt1), NULL);
      av_packet_free(&pkt1.pkt);
   }
   m_nb_packets = 0;
   m_size = 0;
   m_duration = 0;
   m_serial++;
   SDL_UnlockMutex(m_mutex);
}

void PacketQueue::destroy() {
   flush();
   av_fifo_freep(&m_pkt_list);
   SDL_DestroyMutex(m_mutex);
   SDL_DestroyCondition(m_cond);
}
