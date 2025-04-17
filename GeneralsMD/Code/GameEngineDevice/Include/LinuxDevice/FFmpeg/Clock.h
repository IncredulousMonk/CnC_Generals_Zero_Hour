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

///// Clock.h ////////////////////////////////////
// Matthew Gill, April 2025
//////////////////////////////////////////////////

#ifndef CLOCK_H
#define CLOCK_H

/* no AV sync correction is done if below the minimum AV sync threshold */
#define AV_SYNC_THRESHOLD_MIN 0.04
/* AV sync correction is done if above the maximum AV sync threshold */
#define AV_SYNC_THRESHOLD_MAX 0.1
/* If a frame duration is longer than this, it will not be duplicated to compensate AV sync */
#define AV_SYNC_FRAMEDUP_THRESHOLD 0.1
/* no AV correction is done if too big error */
#define AV_NOSYNC_THRESHOLD 10.0

class Clock {
private:
   double m_pts {};           /* clock base */
   double m_pts_drift {};     /* clock base minus time at which we updated the clock */
   double m_last_updated {};
   double m_speed {};
   int m_serial {};           /* clock is based on a packet with this serial */
   int m_paused {};
   int m_queue_serial {};     /* the current packet queue serial, used for obsolete clock detection */
public:
   Clock() {};
   ~Clock() {};

   // No copies allowed!
   Clock(const Clock&) = delete;
   Clock& operator=(const Clock&) = delete;

   void init(int queue_serial);
   double get();
   void set(double pts, int serial);
   void set_at(double pts, int serial, double time);
   void sync_to_slave(Clock& slave);

   int getPts() const { return m_pts; }
   int getSerial() const { return m_serial; }
};

#endif
