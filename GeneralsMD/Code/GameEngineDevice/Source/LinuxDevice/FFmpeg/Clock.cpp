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

///// Clock.cpp //////////////////////////////////
// Matthew Gill, April 2025
//////////////////////////////////////////////////

#include "LinuxDevice/FFmpeg/Clock.h"

#include <math.h>

extern "C" {
#include "libavutil/time.h"
}

void Clock::init(int queue_serial) {
   m_speed = 1.0;
   m_paused = 0;
   m_queue_serial = queue_serial;
   set(NAN, -1);
}

double Clock::get() {
   if (m_queue_serial != m_serial) {
      return NAN;
   }
   if (m_paused) {
      return m_pts;
   } else {
      double time = av_gettime_relative() / 1000000.0;
      return m_pts_drift + time - (time - m_last_updated) * (1.0 - m_speed);
   }
}

void Clock::set(double pts, int serial) {
   double time = av_gettime_relative() / 1000000.0;
   set_at(pts, serial, time);
}

void Clock::set_at(double pts, int serial, double time) {
   m_pts = pts;
   m_last_updated = time;
   m_pts_drift = m_pts - time;
   m_serial = serial;
}

void Clock::sync_to_slave(Clock& slave) {
   double clock = get();
   double slave_clock = slave.get();
   if (!isnan(slave_clock) && (isnan(clock) || fabs(clock - slave_clock) > AV_NOSYNC_THRESHOLD)) {
      set(slave_clock, slave.getSerial());
   }
}
