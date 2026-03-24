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

#include <always.h>
#include "LinuxDevice/GameClient/LinuxAssetManager.h"

//---------------------------------------------------------------------
// LinuxAssetManager
//---------------------------------------------------------------------

//---------------------------------------------------------------------
LinuxAssetManager::LinuxAssetManager(void)
{
}

//---------------------------------------------------------------------
LinuxAssetManager::~LinuxAssetManager(void)
{
}

//---------------------------------------------------------------------
RenderObjClass* LinuxAssetManager::Create_Render_Obj(const char* name, float scale, const Color color, const char* oldTexure, const char* newTexture)
{
   DEBUG_LOG(("*** LinuxAssetManager::Create_Render_Obj: Create 3d object called %s (scale = %f, colour = 0x%x, old texture = 0x%lx, new texture = 0x%lx\n",
      name, scale, color, reinterpret_cast<intptr_t>(oldTexure), reinterpret_cast<intptr_t>(newTexture)));
   return WW3DAssetManager::Create_Render_Obj(name);
}

//---------------------------------------------------------------------
TextureClass* LinuxAssetManager::Get_Texture(const char* filename, MipCountType mip_level_count, WW3DFormat texture_format,
   bool allow_compression, TextureBaseClass::TexAssetType type, bool allow_reduction)
{
   // Just call the base implementation after adjusting reduction to deal with our special types.

   if (filename && *filename && strncasecmp(filename, "ZHC", 3) == 0) {
      allow_reduction = false;	//don't allow reduction on our infantry textures.
   }

   return WW3DAssetManager::Get_Texture(filename, mip_level_count, texture_format, allow_compression, type, allow_reduction);
}
