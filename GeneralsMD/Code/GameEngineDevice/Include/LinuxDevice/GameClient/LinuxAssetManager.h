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

///// LinuxAssemManager.h ////////////////////////
// Matthew Gill, March 2026
//////////////////////////////////////////////////

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef LINUXASSETMANAGER_H
#define LINUXASSETMANAGER_H

#include "assetmgr.h"
#include "rendobj.h"
#include "Lib/BaseType.h"

class LinuxAssetManager: public WW3DAssetManager
{
public:
   LinuxAssetManager(void);
   virtual ~LinuxAssetManager(void);

   // virtual RenderObjClass * Create_Render_Obj(const char * name);
   virtual RenderObjClass* Create_Render_Obj(const char* name, float scale, const Color color, const char* oldTexure = NULL, const char* newTexture = NULL);
   virtual TextureClass* Get_Texture(const char* filename, MipCountType mip_level_count = MIP_LEVELS_ALL, WW3DFormat texture_format = WW3D_FORMAT_UNKNOWN,
      bool allow_compression = true, TextureBaseClass::TexAssetType type = TextureBaseClass::TEX_REGULAR, bool allow_reduction = true);

private:
};

#endif
