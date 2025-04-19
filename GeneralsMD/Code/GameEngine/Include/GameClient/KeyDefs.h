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

////////////////////////////////////////////////////////////////////////////////
//																																						//
//  (c) 2001-2003 Electronic Arts Inc.																				//
//																																						//
////////////////////////////////////////////////////////////////////////////////

// FILE: KeyDefs.h ////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
//                                                                          
//                       Westwood Studios Pacific.                          
//                                                                          
//                       Confidential Information					         
//                Copyright (C) 2001 - All Rights Reserved                  
//                                                                          
//-----------------------------------------------------------------------------
//
// Project:    RTS3
//
// File name:  KeyDefs.h
//
// Created:    Mike Morrison, 1995
//						 Colin Day, June 2001
//
// Desc:       Basic keyboard key definitions.
//
/** @todo NOTE: These key definitions are currently tied directly to the
*		Direct Input key codes, therefore making these definitions device 
*		dependent even though this code lives on the device INdependent side
*		of the engine.  In the future to be truly device independent we 
*		need to define our own key codes, and have a translation between 
*		what we read from the device to our own system*/
// MG: Changed to use SDL scan codes, which _are_ device independent (ha!)
//
//-----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __KEYDEFS_H_
#define __KEYDEFS_H_

// SYSTEM INCLUDES ////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <SDL3/SDL.h>

// USER INCLUDES //////////////////////////////////////////////////////////////

// FORWARD REFERENCES /////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// TYPE DEFINES ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#ifndef DIK_KANA
#define DIK_KANA            0x70    /* (Japanese keyboard)            */
#endif

#ifndef DIK_CONVERT
#define DIK_CONVERT         0x79    /* (Japanese keyboard)            */
#endif

#ifndef DIK_NOCONVERT
#define DIK_NOCONVERT       0x7B    /* (Japanese keyboard)            */
#endif

#ifndef DIK_CIRCUMFLEX
#define DIK_CIRCUMFLEX      0x90    /* (Japanese keyboard)            */
#endif

#ifndef DIK_KANJI
#define DIK_KANJI           0x94    /* (Japanese keyboard)            */
#endif

//=============================================================================
/** The key tables */
//=============================================================================

enum KeyDefType
{
	// keypad keys ---------------------------------------------------------------- 
	KEY_KP0 			= SDL_SCANCODE_KP_0,
	KEY_KP1 			= SDL_SCANCODE_KP_1,
	KEY_KP2 			= SDL_SCANCODE_KP_2,
	KEY_KP3 			= SDL_SCANCODE_KP_3,
	KEY_KP4 			= SDL_SCANCODE_KP_4,
	KEY_KP5 			= SDL_SCANCODE_KP_5,
	KEY_KP6 			= SDL_SCANCODE_KP_6,
	KEY_KP7 			= SDL_SCANCODE_KP_7,
	KEY_KP8 			= SDL_SCANCODE_KP_8,
	KEY_KP9 			= SDL_SCANCODE_KP_9,
	KEY_KPDEL 			= SDL_SCANCODE_KP_PERIOD,
	KEY_KPSTAR 			= SDL_SCANCODE_KP_MULTIPLY,
	KEY_KPMINUS 		= SDL_SCANCODE_KP_MINUS,
	KEY_KPPLUS 			= SDL_SCANCODE_KP_PLUS,

	// regular keys ---------------------------------------------------------------
	KEY_ESC 			= SDL_SCANCODE_ESCAPE,
	KEY_BACKSPACE 		= SDL_SCANCODE_BACKSPACE,
	KEY_ENTER 			= SDL_SCANCODE_RETURN,
	KEY_SPACE 			= SDL_SCANCODE_SPACE,
	KEY_TAB 			= SDL_SCANCODE_TAB,
	KEY_F1 				= SDL_SCANCODE_F1,
	KEY_F2 				= SDL_SCANCODE_F2,
	KEY_F3 				= SDL_SCANCODE_F3,
	KEY_F4 				= SDL_SCANCODE_F4,
	KEY_F5 				= SDL_SCANCODE_F5,
	KEY_F6 				= SDL_SCANCODE_F6,
	KEY_F7 				= SDL_SCANCODE_F7,
	KEY_F8 				= SDL_SCANCODE_F8,
	KEY_F9 				= SDL_SCANCODE_F9,
	KEY_F10 			= SDL_SCANCODE_F10,
	KEY_F11 			= SDL_SCANCODE_F11,
	KEY_F12 			= SDL_SCANCODE_F12,
	KEY_A 				= SDL_SCANCODE_A,
	KEY_B 				= SDL_SCANCODE_B,
	KEY_C 				= SDL_SCANCODE_C,
	KEY_D 				= SDL_SCANCODE_D,
	KEY_E 				= SDL_SCANCODE_E,
	KEY_F 				= SDL_SCANCODE_F,
	KEY_G 				= SDL_SCANCODE_G,
	KEY_H 				= SDL_SCANCODE_H,
	KEY_I 				= SDL_SCANCODE_I,
	KEY_J 				= SDL_SCANCODE_J,
	KEY_K 				= SDL_SCANCODE_K,
	KEY_L 				= SDL_SCANCODE_L,
	KEY_M 				= SDL_SCANCODE_M,
	KEY_N 				= SDL_SCANCODE_N,
	KEY_O 				= SDL_SCANCODE_O,
	KEY_P 				= SDL_SCANCODE_P,
	KEY_Q 				= SDL_SCANCODE_Q,
	KEY_R 				= SDL_SCANCODE_R,
	KEY_S 				= SDL_SCANCODE_S,
	KEY_T 				= SDL_SCANCODE_T,
	KEY_U 				= SDL_SCANCODE_U,
	KEY_V 				= SDL_SCANCODE_V,
	KEY_W 				= SDL_SCANCODE_W,
	KEY_X 				= SDL_SCANCODE_X,
	KEY_Y 				= SDL_SCANCODE_Y,
	KEY_Z 				= SDL_SCANCODE_Z,
	KEY_1 				= SDL_SCANCODE_1,
	KEY_2 				= SDL_SCANCODE_2,
	KEY_3 				= SDL_SCANCODE_3,
	KEY_4 				= SDL_SCANCODE_4,
	KEY_5 				= SDL_SCANCODE_5,
	KEY_6 				= SDL_SCANCODE_6,
	KEY_7 				= SDL_SCANCODE_7,
	KEY_8 				= SDL_SCANCODE_8,
	KEY_9 				= SDL_SCANCODE_9,
	KEY_0 				= SDL_SCANCODE_0,
	KEY_MINUS 			= SDL_SCANCODE_MINUS,
	KEY_EQUAL 			= SDL_SCANCODE_EQUALS,
	KEY_LBRACKET 		= SDL_SCANCODE_LEFTBRACKET,
	KEY_RBRACKET 		= SDL_SCANCODE_RIGHTBRACKET,
	KEY_SEMICOLON 		= SDL_SCANCODE_SEMICOLON,
	KEY_APOSTROPHE 		= SDL_SCANCODE_APOSTROPHE,
	KEY_TICK 			= SDL_SCANCODE_GRAVE,
	KEY_BACKSLASH 		= SDL_SCANCODE_BACKSLASH,
	KEY_COMMA 			= SDL_SCANCODE_COMMA,
	KEY_PERIOD 			= SDL_SCANCODE_PERIOD,
	KEY_SLASH 			= SDL_SCANCODE_SLASH,

	// special keys ---------------------------------------------------------------
	KEY_SYSREQ 			= SDL_SCANCODE_PRINTSCREEN,

	KEY_CAPS 			= SDL_SCANCODE_CAPSLOCK,
	KEY_NUM 			= SDL_SCANCODE_NUMLOCKCLEAR,
	KEY_SCROLL 			= SDL_SCANCODE_SCROLLLOCK,
	KEY_LCTRL 			= SDL_SCANCODE_LCTRL,
	KEY_LALT 			= SDL_SCANCODE_LALT,
	KEY_LSHIFT 			= SDL_SCANCODE_LSHIFT,
	KEY_RSHIFT 			= SDL_SCANCODE_RSHIFT,

	KEY_UP 				= SDL_SCANCODE_UP,
	KEY_DOWN 			= SDL_SCANCODE_DOWN,
	KEY_LEFT 			= SDL_SCANCODE_LEFT,
	KEY_RIGHT 			= SDL_SCANCODE_RIGHT,
	KEY_RALT 			= SDL_SCANCODE_RALT,
	KEY_RCTRL 			= SDL_SCANCODE_RCTRL,
	KEY_HOME 			= SDL_SCANCODE_HOME,
	KEY_END 			= SDL_SCANCODE_END,
	KEY_PGUP 			= SDL_SCANCODE_PAGEUP,
	KEY_PGDN 			= SDL_SCANCODE_PAGEDOWN,
	KEY_INS 			= SDL_SCANCODE_INSERT,
	KEY_DEL 			= SDL_SCANCODE_DELETE,
	KEY_KPENTER 		= SDL_SCANCODE_KP_ENTER,
	KEY_KPSLASH 		= SDL_SCANCODE_KP_DIVIDE,

	KEY_102 			= SDL_SCANCODE_NONUSBACKSLASH,

	// Japanese keyboard keys -----------------------------------------------------
	KEY_KANA 			= DIK_KANA,
	KEY_CONVERT 		= DIK_CONVERT,
	KEY_NOCONVERT 		= DIK_NOCONVERT,
	KEY_YEN 			= SDL_SCANCODE_INTERNATIONAL3,
	KEY_CIRCUMFLEX 		= DIK_CIRCUMFLEX,
	KEY_KANJI 			= DIK_KANJI,

	// specials -------------------------------------------------------------------
	KEY_NONE			= 0x00,		///< to report end of key stream
	KEY_LOST			= 0xFF		///< to report lost keyboard focus

};	// end KeyDefType

// state for keyboard IO ------------------------------------------------------
enum
{
	KEY_STATE_NONE			= 0x0000, // No modifier state
	KEY_STATE_UP			= 0x0001,	// Key is up (default state)
	KEY_STATE_DOWN			= 0x0002,	// Key is down
	KEY_STATE_LCONTROL		= 0x0004,	// Left control is pressed
	KEY_STATE_RCONTROL		= 0x0008,	// Right control is pressed
	KEY_STATE_LSHIFT		= 0x0010,	// left shift is pressed
	KEY_STATE_RSHIFT		= 0x0020,	// right shift is pressed
	KEY_STATE_LALT			= 0x0040,	// left alt is pressed
	KEY_STATE_RALT			= 0x0080,	// right alt is pressed
	KEY_STATE_AUTOREPEAT	= 0x0100,	// Key is down due to autorepeat (only seen in conjunction with KEY_STATE_DOWN)
	KEY_STATE_CAPSLOCK		= 0x0200, // Caps Lock key is on.
	KEY_STATE_SHIFT2		= 0x0400, // Alternate shift key is pressed (I think this is for foreign keyboards..)

	// modifier combinations when left/right isn't a factor
	KEY_STATE_CONTROL		= (KEY_STATE_LCONTROL | KEY_STATE_RCONTROL),
	KEY_STATE_SHIFT			= (KEY_STATE_LSHIFT | KEY_STATE_RSHIFT | KEY_STATE_SHIFT2 ),
	KEY_STATE_ALT			= (KEY_STATE_LALT | KEY_STATE_RALT)

};	// end KeyStateType

// INLINING ///////////////////////////////////////////////////////////////////

// EXTERNALS //////////////////////////////////////////////////////////////////

#endif // __KEYDEFS_H_
