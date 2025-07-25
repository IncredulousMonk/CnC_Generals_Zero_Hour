/*
** Command & Conquer Generals Zero Hour(tm)
** Copyright 2025 Electronic Arts Inc.
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

///// LinuxParticleSys.h /////////////////////////
// Matthew Gill, June 2025
//////////////////////////////////////////////////

#pragma once

#ifndef __LinuxParticleSys_H_
#define __LinuxParticleSys_H_

#include "GameClient/ParticleSys.h"
// #include "WW3D2/PointGr.h"
// #include "WW3D2/streak.h"
// #include "WW3D2/RInfo.h"
// #include "WWLib/BitType.h"

class LinuxParticleSystemManager: public ParticleSystemManager {

public:
   LinuxParticleSystemManager();
   ~LinuxParticleSystemManager();

   virtual void doParticles(RenderInfoClass &rinfo);
   virtual void queueParticleRender();
   ///< returns the number of particles shown on screen per frame
   virtual Int getOnScreenParticleCount() { return m_onScreenParticleCount; }

private:
   enum { MAX_POINTS_PER_GROUP = 512 };

   // PointGroupClass *m_pointGroup;							///< the point group that contains all of the particles
   // StreakLineClass *m_streakLine;							///< the streak class that contains all of the streaks
   // ShareBufferClass<Vector3> *m_posBuffer;			///< array of particle positions
   // ShareBufferClass<Vector4> *m_RGBABuffer;		///< array of particle color and alpha
   // ShareBufferClass<float> *m_sizeBuffer;			///< array of particle sizes
   // ShareBufferClass<uint8> *m_angleBuffer;			///< array of particle orientations
   // Bool m_readyToRender;											///< if true, it is OK to render
};

#endif  // end __LinuxParticleSys_H_
