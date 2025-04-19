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

//////// FFmpegVideoPlayer.cpp ////////////////////////
// Matthew Gill, April 2025
///////////////////////////////////////////////////////

#include "LinuxDevice/FFmpeg/FFmpegVideoPlayer.h"
#include "Common/LocalFileSystem.h"

//----------------------------------------------------------------------------
//         Defines                                                         
//----------------------------------------------------------------------------
// FIXME: Need to sort out the ./assets prefix.  Environment variable?  Command-line option?
#define VIDEO_LANG_PATH_FORMAT "../assets/Data/%s/Movies/%s.%s"
// #define VIDEO_LANG_PATH_FORMAT "Data/%s/Movies/%s.%s"
#define VIDEO_PATH "Data\\Movies"
#define VIDEO_EXT_1 "bik"
#define VIDEO_EXT_2 "BIK" // Movies have lower case extension, except for the logo movie, which is upper case.  Grr!

extern SDL_Renderer* renderer; // FIXME: How to handle this?

//============================================================================
// FFmpegVideoPlayer::FFmpegVideoPlayer
//============================================================================

FFmpegVideoPlayer::FFmpegVideoPlayer() {
}

//============================================================================
// FFmpegVideoPlayer::~FFmpegVideoPlayer
//============================================================================

FFmpegVideoPlayer::~FFmpegVideoPlayer() {
   deinit();
}

//============================================================================
// FFmpegVideoPlayer::init
//============================================================================

void	FFmpegVideoPlayer::init( void )
{
   // Need to load the stuff from the ini file.
   VideoPlayer::init();
}

//============================================================================
// FFmpegVideoPlayer::deinit
//============================================================================

void FFmpegVideoPlayer::deinit( void )
{
   VideoPlayer::deinit();
}

//============================================================================
// FFmpegVideoPlayer::reset
//============================================================================

void	FFmpegVideoPlayer::reset( void )
{
   VideoPlayer::reset();
}

//============================================================================
// FFmpegVideoPlayer::update
//============================================================================

void	FFmpegVideoPlayer::update( void )
{
   VideoPlayer::update();

}

//============================================================================
// FFmpegVideoPlayer::loseFocus
//============================================================================

void	FFmpegVideoPlayer::loseFocus( void )
{
   VideoPlayer::loseFocus();
}

//============================================================================
// FFmpegVideoPlayer::regainFocus
//============================================================================

void	FFmpegVideoPlayer::regainFocus( void )
{
   VideoPlayer::regainFocus();
}

//============================================================================
// FFmpegVideoPlayer::createStream
//============================================================================

VideoStreamInterface* FFmpegVideoPlayer::createStream(FFmpegVideo* video)
{

   if (video == NULL) {
      return NULL;
   }

   FFmpegVideoStream *stream = NEW FFmpegVideoStream;

   if (stream) {

      stream->m_video = video;
      stream->m_next = m_firstStream;
      stream->m_player = this;
      m_firstStream = stream;
   }

   return stream;
}

//============================================================================
// FFmpegVideoPlayer::open
//============================================================================

VideoStreamInterface* FFmpegVideoPlayer::open(AsciiString movieTitle)
{
   VideoStreamInterface* stream = NULL;

   const Video* pVideo = getVideo(movieTitle);
   if (pVideo) {
      DEBUG_LOG(("FFmpegVideoPlayer::createStream() - About to open movie file\n"));

      // if (TheGlobalData->m_modDir.isNotEmpty())
      // {
      //    char filePath[ _MAX_PATH ];
      //    sprintf( filePath, "%s%s\\%s.%s", TheGlobalData->m_modDir.str(), VIDEO_PATH, pVideo->m_filename.str(), VIDEO_EXT );
      //    HBINK handle = BinkOpen(filePath , BINKPRELOADALL );
      //    DEBUG_ASSERTLOG(!handle, ("opened bink file %s\n", filePath));
      //    if (handle)
      //    {
      //       return createStream( handle );
      //    }
      // }

      char localizedFilePath[PATH_MAX] {};
      // sprintf( localizedFilePath, VIDEO_LANG_PATH_FORMAT, GetRegistryLanguage().str(), pVideo->m_filename.str(), VIDEO_EXT );
      const char* language {"English"};
      sprintf(localizedFilePath, VIDEO_LANG_PATH_FORMAT, language, pVideo->m_filename.str(), VIDEO_EXT_1);
      if (!TheLocalFileSystem->doesFileExist(localizedFilePath)) {
         sprintf(localizedFilePath, VIDEO_LANG_PATH_FORMAT, language, pVideo->m_filename.str(), VIDEO_EXT_2);
      }
      FFmpegVideo* video = NEW FFmpegVideo {};
      if (video->open(localizedFilePath, renderer, 0, 0, 800, 600)) { // FIXME: Hard-coded size is bad.
         DEBUG_ASSERTLOG(!video, ("opened localized movie file %s\n", localizedFilePath));
         // if (!video) {
         //    char filePath[PATH_MAX] {};
         //    sprintf( filePath, "%s\\%s.%s", VIDEO_PATH, pVideo->m_filename.str(), VIDEO_EXT );
         //    video = BinkOpen(filePath , BINKPRELOADALL );
         //    DEBUG_ASSERTLOG(!video, ("opened movie file %s\n", localizedFilePath));
         // }

         DEBUG_LOG(("FFmpegVideoPlayer::createStream() - About to create stream\n"));
         stream = createStream(video);
      }
   }

   return stream;	
}

//============================================================================
// FFmpegVideoPlayer::load
//============================================================================

VideoStreamInterface* FFmpegVideoPlayer::load(AsciiString movieTitle)
{
   return open(movieTitle); // load() used to have the same body as open(), so I'm combining them.  Munkee.
}

//============================================================================
// FFmpegVideoStream::FFmpegVideoStream
//============================================================================

FFmpegVideoStream::FFmpegVideoStream() {
}

//============================================================================
// FFmpegVideoStream::~FFmpegVideoStream
//============================================================================

FFmpegVideoStream::~FFmpegVideoStream() {
   if (m_video) {
      m_video->close();
      delete m_video;
      m_video = nullptr;
   }
}

//============================================================================
// FFmpegVideoStream::update
//============================================================================

void FFmpegVideoStream::update( void )
{
   if (m_video && !m_video_finished) {
      double remaining_time {0.0};
      m_video_finished = !m_video->update(&remaining_time);
   }
}

//============================================================================
// FFmpegVideoStream::isVideoFinished
//============================================================================

Bool FFmpegVideoStream::isVideoFinished( void )
{
   return m_video_finished;
}

//============================================================================
// FFmpegVideoStream::isFrameReady
//============================================================================

Bool FFmpegVideoStream::isFrameReady( void )
{
   return FALSE;
}

//============================================================================
// FFmpegVideoStream::frameDecompress
//============================================================================

void FFmpegVideoStream::frameDecompress( void )
{
}

//============================================================================
// FFmpegVideoStream::frameRender
//============================================================================

void FFmpegVideoStream::frameRender( VideoBuffer *buffer )
{
   if ( buffer )
   {
      // void *mem = buffer->lock();

      // u32 flags;

      // switch ( buffer->format())
      // {
      //    case VideoBuffer::TYPE_X8R8G8B8:
      //       flags = BINKSURFACE32;
      //       break;

      //    case VideoBuffer::TYPE_R8G8B8:
      //       flags = BINKSURFACE24;
      //       break;

      //    case VideoBuffer::TYPE_R5G6B5:
      //       flags = BINKSURFACE565;
      //       break;

      //    case VideoBuffer::TYPE_X1R5G5B5:
      //       flags = BINKSURFACE555;
      //       break;

      //    default:
      //       return;
      // }
      
      // if ( mem != NULL )
      // {

      //    BinkCopyToBuffer ( m_handle, mem, buffer->pitch(), buffer->height(),
      //                                  buffer->xPos(), buffer->yPos(), flags );
      //    buffer->unlock();
      // }
   }

}

//============================================================================
// FFmpegVideoStream::frameNext
//============================================================================

void FFmpegVideoStream::frameNext( void )
{
}

//============================================================================
// FFmpegVideoStream::frameIndex
//============================================================================

Int FFmpegVideoStream::frameIndex( void )
{
   return 0;
}

//============================================================================
// FFmpegVideoStream::totalFrames
//============================================================================

Int FFmpegVideoStream::frameCount( void )
{
   return 0;
}

//============================================================================
// FFmpegVideoStream::frameGoto
//============================================================================

void FFmpegVideoStream::frameGoto( Int index )
{
   (void) index;
}

//============================================================================
// VideoStream::height
//============================================================================

Int FFmpegVideoStream::height( void )
{
   return 0;
}

//============================================================================
// VideoStream::width
//============================================================================

Int FFmpegVideoStream::width( void )
{
   return 0;
}
