/*
**	Command & Conquer Generals Zero Hour(tm)
**	Copyright 2025 Electronic Arts Inc.
**
**	This program is free software: you can redistribute it and/or modify
**	it under the terms of the GNU General Public License as published by
**	the Free Software Foundation, either version 3 of the License, or
**	(at your option) any later version.
**
**	This program is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**	GNU General Public License for more details.
**
**	You should have received a copy of the GNU General Public License
**	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// FILE: ScopedMutex.h ////////////////////////////////////////////////////////////////////////////
// Author: John McDonald, November 2002
// Desc:   A scoped mutex class to easily lock a scope with a pre-existing mutex object.
///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __SCOPEDMUTEX_H__
#define __SCOPEDMUTEX_H__

class ScopedMutex
{
	private:
		SDL_Mutex* m_mutex;

	public:
		ScopedMutex(SDL_Mutex* mutex) : m_mutex(mutex)
		{
			SDL_LockMutex(m_mutex);
		}

		~ScopedMutex()
		{
			SDL_UnlockMutex(m_mutex);
		}

		// No copies allowed!
		ScopedMutex(const ScopedMutex&) = delete;
		ScopedMutex& operator=(const ScopedMutex&) = delete;
};

#endif /* __SCOPEDMUTEX_H__ */