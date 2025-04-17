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

///// PacketQueue.h //////////////////////////////
// Matthew Gill, April 2025
//////////////////////////////////////////////////

#ifndef PACKET_QUEUE_H
#define PACKET_QUEUE_H

extern "C" {
#include "libavutil/fifo.h"
#include "libavformat/avformat.h"
}

#include <SDL3/SDL.h>

struct Packet {
   AVPacket *pkt;
   int serial;
};

class PacketQueue {
private:
   AVFifoBuffer *m_pkt_list {};
   int m_nb_packets {};
   int m_size {};
   int64_t m_duration {};
   bool m_abort_requested {};
   int m_serial {0};
   SDL_Mutex *m_mutex {};
   SDL_Condition *m_cond {};
   int put_private(AVPacket *pkt);
public:
   PacketQueue() {};
   ~PacketQueue() {};

   // No copies allowed!
   PacketQueue(const PacketQueue&) = delete;
   PacketQueue& operator=(const PacketQueue&) = delete;

   int init();
   void start();
   void abort();
   int put_nullpacket(AVPacket *pkt, int stream_index);
   int put(AVPacket *pkt);
   int get(AVPacket *pkt, int block, int *serial);
   void flush();
   void destroy();

   int getPacketCount() const { return m_nb_packets; }
   int getSize() const { return m_size; }
   int getDuration() const { return m_duration; }
   int getSerial() const { return m_serial; }
   bool isAbortRequested() const { return m_abort_requested; }
};

#endif
