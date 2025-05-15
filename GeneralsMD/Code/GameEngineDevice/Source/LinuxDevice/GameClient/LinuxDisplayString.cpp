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

///// LinuxDisplayString.cpp /////////////////////
// Matthew Gill, April 2025
//////////////////////////////////////////////////

// SYSTEM INCLUDES ////////////////////////////////////////////////////////////

// USER INCLUDES //////////////////////////////////////////////////////////////
#include "GameClient/Color.h"
#include "GameClient/GameClient.h"
#include "LinuxDevice/GameClient/LinuxDisplayString.h"
#include "GameClient/HotKey.h"
#include "GameClient/GameFont.h"
#include "GameClient/GlobalLanguage.h"

// DEFINES ////////////////////////////////////////////////////////////////////
#ifdef _INTERNAL
// for occasional debugging...
//#pragma optimize("", off)
//#pragma MESSAGE("************************************** WARNING, optimization disabled for debugging purposes")
#endif

// PRIVATE TYPES //////////////////////////////////////////////////////////////

// PRIVATE DATA ///////////////////////////////////////////////////////////////

// PUBLIC DATA ////////////////////////////////////////////////////////////////

// PRIVATE PROTOTYPES /////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// LinuxDisplayString::LinuxDisplayString =====================================
/** */
//=============================================================================
LinuxDisplayString::LinuxDisplayString()
{
   
   m_textPos.x = 0;
   m_textPos.y = 0;
   m_currTextColor = 0;
   m_currDropColor = 0;
   m_size.x = 0;
   m_size.y = 0;
   m_clipRegion.lo.x = 0;
   m_clipRegion.lo.y = 0;
   m_clipRegion.hi.x = 0;
   m_clipRegion.hi.y = 0;
   m_lastResourceFrame = 0;
   m_useHotKey = FALSE;
   m_hotKeyPos.x = 0;
   m_hotKeyPos.y = 0;
   m_hotKeyColor = GameMakeColor(255,255,255,255);
   
}  // end LinuxDisplayString

// LinuxDisplayString::~LinuxDisplayString ====================================
/** */
//=============================================================================
LinuxDisplayString::~LinuxDisplayString()
{
   if (m_text) {
      TTF_DestroyText(m_text);
   }
}  // end ~LinuxDisplayString

// LinuxDisplayString::textChanged ============================================
/** This method automatically gets called from some methods in the display
   * class so that we can write our own code here to to appropriate things
   * on the changing of string data */
//=============================================================================
void LinuxDisplayString::notifyTextChanged()
{

   // extend functionality
   DisplayString::notifyTextChanged();
   // if(TheGlobalLanguageData)
   // {
   // 	if(TheGlobalLanguageData->m_ini.m_useHardWrap == TRUE)
   // 	{	
   // 		m_textRenderer.Set_Use_Hard_Word_Wrap(true);
   // 		m_textRendererHotKey.Set_Use_Hard_Word_Wrap(true);
   // 	}
   // 	else
   // 	{	
   // 		m_textRenderer.Set_Use_Hard_Word_Wrap(false);
   // 		m_textRendererHotKey.Set_Use_Hard_Word_Wrap(false);
   // 	}
   // }

   createText();

}  // end notifyTextChanged

// LinuxDisplayString::Draw ===================================================
/** Draw the text at the specified location in in the specified colors
   * in the parameters.  Since we keep an instance of the rendered text
   * texture around, this is the time to check to see if any of our
   * properties have changed since the last render, like color, or
   * position, or content.  If they have, we need to rebuild the sentence
   * texture for rendering */
//=============================================================================
void LinuxDisplayString::draw(Int x, Int y, Color color, Color dropColor)
{
   draw(x,y, color, dropColor, 1, 1);
}
void LinuxDisplayString::draw(Int x, Int y, Color color, Color dropColor, Int xDrop, Int yDrop)
{
   // Bool needNewPolys = FALSE;

   // sanity
   if (getTextLength() == 0) {
      return;  // nothing to draw
   }
   
   // // if our font or text has changed we need to build a new sentence
   // if( m_fontChanged || m_textChanged )
   // {
   // 	if(m_useHotKey)
   // 	{
   // 		m_textRenderer.Set_Hot_Key_Parse(TRUE);
   // 		m_textRenderer.Build_Sentence( getText().str(), &m_hotKeyPos.x, &m_hotKeyPos.y );
   // 		m_hotkey.translate(TheHotKeyManager->searchHotKey(getText()));
   // 		if(!m_hotkey.isEmpty())
   // 			m_textRendererHotKey.Build_Sentence(m_hotkey.str(), NULL, NULL);
   // 		else
   // 		{
   // 			m_useHotKey = FALSE;
   // 			m_textRendererHotKey.Reset();
   // 		}
   // 	}
   // 	else
   // 		m_textRenderer.Build_Sentence( getText().str(), NULL, NULL );
   // 	m_fontChanged = FALSE;
   // 	m_textChanged = FALSE;
   // 	needNewPolys = TRUE;

   // }  // end if

   if (x != m_textPos.x || y != m_textPos.y || color != m_currTextColor || dropColor != m_currDropColor) {
      // save the new attributes of the text position and color
      m_textPos.x = x;
      m_textPos.y = y;
      m_currTextColor = color;
      m_currDropColor = dropColor;

      // // reset the quads
      // m_textRenderer.Reset_Polys();

      // // draw the shadow
      // m_textRenderer.Set_Location( Vector2( m_textPos.x + xDrop, m_textPos.y + yDrop) );
      // m_textRenderer.Draw_Sentence( m_currDropColor );

      // // draw the text
      // m_textRenderer.Set_Location( Vector2( m_textPos.x, m_textPos.y ) );
      // m_textRenderer.Draw_Sentence( m_currTextColor );

      // if(m_useHotKey)
      // {
      //    m_textRendererHotKey.Reset_Polys();
      //    m_textRendererHotKey.Set_Location( Vector2( m_textPos.x + m_hotKeyPos.x , m_textPos.y +m_hotKeyPos.y) );
      //    m_textRendererHotKey.Draw_Sentence( m_hotKeyColor );
      //    m_textRendererHotKey.Render();
      // }

   }  // end if

   // render the text
   UnsignedByte r, g, b, a;
   if (xDrop && yDrop) {
      GameGetColorComponents(m_currDropColor, &r, &g, &b, &a);
      TTF_SetTextColor(m_text, r, g, b, a);
      TTF_DrawRendererText(m_text, m_textPos.x + xDrop, m_textPos.y + yDrop);
   }
   GameGetColorComponents(m_currTextColor, &r, &g, &b, &a);
   TTF_SetTextColor(m_text, r, g, b, a);
   TTF_DrawRendererText(m_text, m_textPos.x, m_textPos.y);

   // // we are for sure using display resources now
   // if( TheGameClient )
   // 	usingResources( TheGameClient->getFrame() );

}  // end draw

// LinuxDisplayString::getSize ================================================
/** Get the render size width and height of the string in this instance
   * with the font associated with it */
//=============================================================================
void LinuxDisplayString::getSize(Int *width, Int *height)
{
   // assign the width and height we have stored to parameters present
   if (width) {
      *width = m_size.x;
   }
   if (height) {
      *height = m_size.y;
   }
}  // end getSize

// DisplayString::appendChar ==================================================
/** Get text with up to charPos characters, -1 = all characters */
//=============================================================================

Int LinuxDisplayString::getWidth(Int charPos)
{
   if (charPos >= 0) {
      DEBUG_LOG(("LinuxDisplayString::getWidth (%d) not yet implemented!\n", charPos));
   }

   return m_size.x;

   // FontCharsClass *	font;
   // Int width = 0;
   // Int count = 0;

   // font = m_textRenderer.Peek_Font();

   // if ( font )
   // {
   // 	const WideChar *text = m_textString.str();
   // 	WideChar ch;

   // 	while ( (ch = *text++ ) != 0 && ( charPos == -1 || count < charPos ) ) 
   // 	{
   // 		if ( ch != (WideChar)'\n' ) 
   // 		{
   // 			width += font->Get_Char_Spacing( ch );
   // 		}
   // 		count++;
   // 	}
   // }

   // return width;
}

// LinuxDisplayString::setFont ================================================
/** Set the font for this particular display string */
//=============================================================================
void LinuxDisplayString::setFont(GameFont *font) {

   // sanity
   if (font == NULL) {
      return;
   }

   // if the new font is the same as our existing font do nothing
   if (m_font == font) {
      return;
   }

   // extending functionality
   DisplayString::setFont( font );

   createText();

}  // end setFont

// LinuxDisplayString::setClipRegion ==========================================
/** Set the clipping region for the text */
//=============================================================================
void LinuxDisplayString::setClipRegion( IRegion2D *region )
{
   // FIXME: LinuxDisplayString::setClipRegion

   // extend functionality
   DisplayString::setClipRegion( region );

   // // only consider regions that are actual changes
   // if( region->lo.x != m_clipRegion.lo.x ||
   // 		region->lo.y != m_clipRegion.lo.y ||
   // 		region->hi.x != m_clipRegion.hi.x ||
   // 		region->hi.y != m_clipRegion.hi.y )
   // {

   // 	// assign new region
   // 	m_clipRegion = *region;

   // 	// set new region in renderer
   // 	m_textRenderer.Set_Clipping_Rect( RectClass( m_clipRegion.lo.x,
   // 																							 m_clipRegion.lo.y,
   // 																							 m_clipRegion.hi.x,
   // 																							 m_clipRegion.hi.y ) );
   // 	m_textRendererHotKey.Set_Clipping_Rect( RectClass( m_clipRegion.lo.x,
   // 																							 m_clipRegion.lo.y,
   // 																							 m_clipRegion.hi.x,
   // 																							 m_clipRegion.hi.y ) );
   // }  // end if

}  // end setClipRegion

// LinuxDisplayString::computeExtents =========================================
/** Update the width and height of our string */
//=============================================================================
void LinuxDisplayString::computeExtents()
{
   Int len = getTextLength();

   // if we have no string, or no font we don't have a size yet
   if (len == 0 || !m_font || !m_text) {
      m_size.x = 0;
      m_size.y = 0;
   } else {
      TTF_GetTextSize(m_text, &m_size.x, &m_size.y);
   }
}  // end computeExtents

//=============================================================================
void LinuxDisplayString::setWordWrap(Int wordWrap)
{
   // set the Word Wrap
   if (m_text) {
      TTF_SetTextWrapWidth(m_text, wordWrap);
      notifyTextChanged();
   }
}

//=============================================================================
void LinuxDisplayString::setUseHotkey(Bool useHotkey, Color hotKeyColor)
{
   if (useHotkey) {
      DEBUG_LOG(("LinuxDisplayString::setUseHotkey not yet implemented!\n"));
   }
   // m_useHotKey = useHotkey;
   // m_hotKeyColor = hotKeyColor;
   // m_textRenderer.Set_Hot_Key_Parse(useHotkey);
   // notifyTextChanged();
}

// LinuxDisplayString::setWordWrapCentered ====================================
/** Set the whether or not we want to center each new line in a text string */
//=============================================================================
void LinuxDisplayString::setWordWrapCentered(Bool isCentered)
{
   if (isCentered) {
      DEBUG_LOG(("LinuxDisplayString::setWordWrapCentered not yet implemented!\n"));
   }
   // // set the Word Wrap
   //  if( m_textRenderer.Set_Word_Wrap_Centered(isCentered) )
   // 	notifyTextChanged();
}// void setWordWrap( Int wordWrap )

///////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS //////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//=============================================================================
void LinuxDisplayString::createText()
{
   if (m_text) {
      TTF_DestroyText(m_text);
   }
   if (getTextLength() > 0 && m_font) {
      m_text = m_renderer->createText(m_textString, *m_font);

      // get our new text extents
      computeExtents();
   }
}
