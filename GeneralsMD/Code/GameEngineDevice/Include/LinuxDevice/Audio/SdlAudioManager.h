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

///// SdlAudioManager.h //////////////////////////
// Matthew Gill, April 2025
//////////////////////////////////////////////////

#include "Common/AsciiString.h"
#include "Common/GameAudio.h"
#include "LinuxDevice/Common/SdlFileStream.h"
#include <SDL3/SDL.h>
#include <SDL3_sound/SDL_sound.h>

using HSAMPLE = UnsignedInt;
using HSTREAM = Sound_Sample*;
// FIXME: Temporary types until I figure them out.
typedef void* H3DSAMPLE;
typedef void* HDIGDRIVER;
typedef int H3DPOBJECT;
typedef void* HPROVIDER;
typedef int HAUDIO;


class AudioEventRTS;

enum { MAXPROVIDERS = 64 };

enum PlayingAudioType
{
   PAT_Sample,
   PAT_3DSample,
   PAT_Stream,
   PAT_INVALID
};

enum PlayingStatus
{
   PS_Playing,
   PS_Stopped,
   PS_Paused
};

enum PlayingWhich
{
   PW_Attack,
   PW_Sound,
   PW_Decay,
   PW_INVALID
};

struct PlayingAudio: public MemoryPoolObject
{
   MEMORY_POOL_GLUE_WITH_USERLOOKUP_CREATE(PlayingAudio, "PlayingAudio")

public:
   HSAMPLE m_sample {};
   H3DSAMPLE m_3DSample {};
   HSTREAM m_stream {};

   SDL_AudioStream* m_sdlStream;
   UnsignedInt m_audioLen;
   Bool m_streamEof;
   SdlFileStream* m_streamFileStream;

   PlayingAudioType m_type {};
   volatile PlayingStatus m_status {}; // This member is adjusted by another running thread.
   AudioEventRTS* m_audioEventRTS {};
   UnsignedByte* m_audioBuf {};
   Bool m_requestStop {};
   Bool m_cleanupAudioEventRTS {};
   Int m_framesFaded {};
   
   PlayingAudio() :
      m_sample(0),
      m_3DSample(0),
      m_stream(nullptr),
      m_sdlStream(nullptr),
      m_audioLen(0),
      m_streamEof(false),
      m_streamFileStream(nullptr),
      m_type(PAT_INVALID),
      m_audioEventRTS(NULL),
      m_requestStop(false),
      m_cleanupAudioEventRTS(true),
      m_framesFaded(0)
   { }

   // No copies allowed!
   PlayingAudio(const PlayingAudio&) = delete;
   PlayingAudio& operator=(const PlayingAudio&) = delete;
};

inline PlayingAudio::~PlayingAudio()
{
   if (m_streamFileStream) {
      delete m_streamFileStream;
   }
}

struct ProviderInfo
{
   AsciiString name;
   int id;
   Bool m_isValid;
};

struct OpenAudioFile
{
   SDL_AudioSpec m_audioSpec;
   UnsignedByte* m_audioBuf;
   UnsignedInt m_audioLen;
   UnsignedInt m_openCount;

   // Note: OpenAudioFile does not own this m_eventInfo, and should not delete it.
   const AudioEventInfo* m_eventInfo;  // Not mutable, unlike the one on AudioEventRTS.
};

typedef std::unordered_map<AsciiString, OpenAudioFile, rts::hash<AsciiString>, rts::equal_to<AsciiString>> OpenFilesHash;
typedef OpenFilesHash::iterator OpenFilesHashIt;

class AudioFileCache
{
public:
   AudioFileCache();

   // No copies allowed!
   AudioFileCache(const AudioFileCache&) = delete;
   AudioFileCache& operator=(const AudioFileCache&) = delete;

   // Protected by mutex
   virtual ~AudioFileCache();

   OpenAudioFile* openFile(AudioEventRTS* eventToOpenFrom);
   void releaseBuffer(UnsignedByte* buffer);
   void setMaxSize(UnsignedInt size);
   // End Protected by mutex

   // Note: These functions should be used for informational purposes only. For speed reasons,
   // they are not protected by the mutex, so they are not guarenteed to be valid if called from
   // outside the audio cache. They should be used as a rough estimate only.
   UnsignedInt getCurrentlyUsedSize() const { return m_currentlyUsedSize; }
   UnsignedInt getMaxSize() const { return m_maxSize; }

protected:
   void releaseOpenAudioFile(OpenAudioFile* fileToRelease);

   // This function will return TRUE if it was able to free enough space, and FALSE otherwise.
   Bool freeEnoughSpaceForSample(const OpenAudioFile& sampleThatNeedsSpace);

   OpenFilesHash m_openFiles {};
   UnsignedInt m_currentlyUsedSize {};
   UnsignedInt m_maxSize {};
   SDL_Mutex* m_mutex {};
};

class SdlAudioManager: public AudioManager
{

public:
#if defined(_DEBUG) || defined(_INTERNAL)
   virtual void audioDebugDisplay(DebugDisplayInterface* dd, void*, FILE* fp = NULL);
   virtual AudioHandle addAudioEvent(const AudioEventRTS* eventToAdd);	///< Add an audio event (event must be declared in an INI file)
#endif

   // from AudioDevice
   virtual void init();
   virtual void postProcessLoad();
   virtual void reset();
   virtual void update();

   SdlAudioManager();
   virtual ~SdlAudioManager();

   // No copies allowed!
   SdlAudioManager(const SdlAudioManager&) = delete;
   SdlAudioManager& operator=(const SdlAudioManager&) = delete;

   virtual void nextMusicTrack();
   virtual void prevMusicTrack();
   virtual Bool isMusicPlaying() const;
   virtual Bool hasMusicTrackCompleted(const AsciiString& trackName, Int numberOfTimes) const;
   virtual AsciiString getMusicTrackName() const;

   virtual void openDevice();
   virtual void closeDevice();
   virtual void* getDevice() { return m_digitalHandle; }

   virtual void stopAudio(AudioAffect which);
   virtual void pauseAudio(AudioAffect which);
   virtual void resumeAudio(AudioAffect which);
   virtual void pauseAmbient(Bool shouldPause);

   virtual void killAudioEventImmediately(AudioHandle audioEvent);

   ///< Return whether the current audio is playing or not. 
   ///< NOTE NOTE NOTE !!DO NOT USE THIS FOR GAMELOGIC PURPOSES!! NOTE NOTE NOTE
   virtual Bool isCurrentlyPlaying(AudioHandle handle);

   virtual void notifyOfAudioCompletion(uintptr_t audioCompleted, UnsignedInt flags);
   virtual PlayingAudio* findPlayingAudioFrom(uintptr_t audioCompleted, UnsignedInt flags);

   virtual UnsignedInt getProviderCount() const;
   virtual AsciiString getProviderName(UnsignedInt providerNum) const;
   virtual UnsignedInt getProviderIndex(AsciiString providerName) const;
   virtual void selectProvider(UnsignedInt providerNdx);
   virtual void unselectProvider();
   virtual UnsignedInt getSelectedProvider() const;
   virtual void setSpeakerType(UnsignedInt speakerType);
   virtual UnsignedInt getSpeakerType();

   virtual void* getHandleForBink();
   virtual void releaseHandleForBink();

   virtual void friend_forcePlayAudioEventRTS(const AudioEventRTS* eventToPlay);

   virtual UnsignedInt getNum2DSamples() const;
   virtual UnsignedInt getNum3DSamples() const;
   virtual UnsignedInt getNumStreams() const;

   virtual Bool doesViolateLimit(AudioEventRTS* event) const;
   virtual Bool isPlayingLowerPriority(AudioEventRTS* event) const;
   virtual Bool isPlayingAlready(AudioEventRTS* event) const;
   virtual Bool isObjectPlayingVoice(UnsignedInt objID) const;
   Bool killLowestPrioritySoundImmediately(AudioEventRTS* event);
   AudioEventRTS* findLowestPrioritySound(AudioEventRTS* event);

   virtual void adjustVolumeOfPlayingAudio(AsciiString eventName, Real newVolume);

   virtual void removePlayingAudio(AsciiString eventName);
   virtual void removeAllDisabledAudio();

   virtual void processRequestList();
   virtual void processPlayingList();
   virtual void processFadingList();
   virtual void processStoppedList();

   Bool shouldProcessRequestThisFrame(AudioRequest* req) const;
   void adjustRequest(AudioRequest* req);
   Bool checkForSample(AudioRequest* req);

   virtual void setHardwareAccelerated(Bool accel);
   virtual void setSpeakerSurround(Bool surround);
   
   virtual void setPreferredProvider(AsciiString provider) { m_pref3DProvider = provider; }
   virtual void setPreferredSpeaker(AsciiString speakerType) { m_prefSpeaker = speakerType; }

   virtual Real getFileLengthMS(AsciiString strToLoad) const;

   virtual void closeAnySamplesUsingFile(const void* fileToClose);

   virtual Bool has3DSensitiveStreamsPlaying() const; 

protected:
   // 3-D functions
   virtual void setDeviceListenerPosition();
   const Coord3D* getCurrentPositionFromEvent(AudioEventRTS* event);
   Bool isOnScreen(const Coord3D* pos) const;
   Real getEffectiveVolume(AudioEventRTS* event) const;

   // Looping functions
   Bool startNextLoop(PlayingAudio* looping);

   void playStream(AudioEventRTS* event, PlayingAudio* audio, Real volume);
   void playSample(AudioEventRTS* event, PlayingAudio* audio);
   void* playSample3D(AudioEventRTS* event, H3DSAMPLE sample3D);

protected:
   void buildProviderList();
   void createListener();
   void initDelayFilter();
   Bool isValidProvider();
   void initSamplePools();
   void processRequest(AudioRequest* req);

   void playAudioEvent(AudioEventRTS* event);
   void stopAudioEvent(AudioHandle handle);
   void pauseAudioEvent(AudioHandle handle);

   OpenAudioFile* loadFileForRead(AudioEventRTS* eventToLoadFrom);
   void releaseBuffer(UnsignedByte* buffer);

   PlayingAudio* allocatePlayingAudio();
   void releaseHandle(PlayingAudio* release);
   void releasePlayingAudio(PlayingAudio* release);
   
   void stopAllAudioImmediately();
   void freeAllHandles();

   HSAMPLE getFirst2DSample(AudioEventRTS* event);
   H3DSAMPLE getFirst3DSample(AudioEventRTS* event);

   void adjustPlayingVolume(PlayingAudio* audio);
   
   void stopAllSpeech();

protected:
   void initFilters(PlayingAudio* sample, const AudioEventRTS* eventInfo);
   void initFilters3D(H3DSAMPLE sample, const AudioEventRTS* eventInfo, const Coord3D* pos);

protected:
   SDL_AudioDeviceID m_device {};

   ProviderInfo m_provider3D[MAXPROVIDERS] {};
   UnsignedInt m_providerCount {};
   UnsignedInt m_selectedProvider {};
   UnsignedInt m_lastProvider {};
   UnsignedInt m_selectedSpeakerType {};

   AsciiString m_pref3DProvider {};
   AsciiString m_prefSpeaker {};

   HDIGDRIVER m_digitalHandle {};
   H3DPOBJECT m_listener {};
   HPROVIDER m_delayFilter {};

   // This is a list of all handles that are forcibly played. They always play as UI sounds.
   std::list<HAUDIO> m_audioForcePlayed {};

   // Available handles for play. Note that there aren't handles open in advance for 
   // streaming things, only 2-D and 3-D sounds.
   std::list<HSAMPLE> m_availableSamples {};
   std::list<H3DSAMPLE> m_available3DSamples {};

   // Currently Playing stuff. Useful if we have to preempt it. 
   // This should rarely if ever happen, as we mirror this in Sounds, and attempt to 
   // keep preemption from taking place here.
   std::list<PlayingAudio*> m_playingSounds {};
   std::list<PlayingAudio*> m_playing3DSounds {};
   std::list<PlayingAudio*> m_playingStreams {};

   // Currently fading stuff. At this point, we just want to let it finish fading, when it is
   // done it should be added to the completed list, then "freed" and the counts should be updated
   // on the next update
   std::list<PlayingAudio*> m_fadingAudio {};

   // Stuff that is done playing (either because it has finished or because it was killed)
   // This stuff should be cleaned up during the next update cycle. This includes updating counts
   // in the sound engine
   std::list<PlayingAudio*> m_stoppedAudio {};

   AudioFileCache* m_audioCache {};
   PlayingAudio* m_binkHandle {};
   UnsignedInt m_num2DSamples {};
   UnsignedInt m_num3DSamples {};
   UnsignedInt m_numStreams {};

#if defined(_DEBUG) || defined(_INTERNAL)
   typedef std::set<AsciiString> SetAsciiString;
   typedef SetAsciiString::iterator SetAsciiStringIt;
   SetAsciiString m_allEventsLoaded {};
   void dumpAllAssetsUsed();
#endif

};

