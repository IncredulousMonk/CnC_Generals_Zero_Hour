
# Command & Conquer Generals Zero Hour native Linux port

I'm attempting to create a native Linux port of Zero Hour. I suspect that it's a fool's errand, but it should be an interesting project.

You can learn a lot by reading source code, but you can learn a lot more by trying to get that code working using a different compiler on a different operating system.


## Progress so far

**I've added some progress screenshots to the wiki page.**

- Displays a window with the splash screen.
- Plays the logo movie.
- Displays the main menu.
- The credits menu works (including the music!)
- The DynamicMemoryAllocator works.
- Subsystems implemented:
  - TheNameKeyGenerator
  - TheCommandList
  - TheFileSystem
  - TheLocalFileSystem
  - TheArchiveFileSystem
  - TheWritableGlobalData
  - TheGameLODManager
  - TheGameText
  - TheScienceStore
  - TheMultiplayerSettings
  - TheTerrainTypes
  - TheTerrainRoads
  - TheGlobalLanguageData
  - TheAudio
  - TheFunctionLexicon
  - TheModuleFactory (0 out of 199 modules implemented)
  - TheParticleSystemManager (minimal)
  - TheMessageStream
  - TheFXListStore
  - TheWeaponStore
  - TheDamageFXStore
  - TheArmorStore
  - TheThingFactory
  - TheGameClient (partial)
    - TheDrawGroupInfo
    - TheDisplayStringManager
    - TheDisplay
    - TheKeyboard
    - TheMouse
    - TheMappedImageCollection
    - TheAnim2DCollection
    - TheFontLibrary
    - TheHeaderTemplateManager
    - TheWindowManager
    - TheVideoPlayer
    - TheLanguageFilter
    - TheIMEManager
    - TheShell
  - TheGameLogic (minimal)

## Still to do

- The main-menu screen needs tweaking. The transparency is a bit off. The exit dialog shows transparency problems too.
- Most of the main-menu buttons don't work yet. The Options menu is likely to be my next task.
- 2D sound works, music works, but I don't have an SDL library for 3D (positional) sound. I may have to look at OpenAL.
- Everything else.


## Dependencies

I'm using SDL3, SDL_ttf 3 (fonts), SDL_image 3 (loading .tga images), and SDL_sound (decoding .mp3 files), so you'll have to install those libraries.

I'm using FFmpeg to replace Bink, so I installed libavformat-dev, libswscale-dev, and libavdevice-dev.

You need to create a symlink called `assets` in the `GeneralsMD` directory that points to the directory where you have Zero Hour installed, such that typing `ls assets/AudioZH.big` should work. At the moment it's used to load the splash screen, but eventually it will be used to load all of the assets.


## Compiling

I'm using a standard makefile. Open a terminal in the `GeneralsMD/Code` directory, and type `make`, or `make DEBUG=1` for a debug build.

The compiled executable is `GeneralsMD/Run/rts`.


## Running

You can type `make run` to run the executable.

If you get an error message that says:

    ERROR: Could not load bitmap: Couldn't open ../assets/Install_Final.bmp: No such file or directory

then you forgot to create the symlink mentioned above.

Use the Exit button in the main menu to exit the game.

<!---
## What I've learned so far

My professional programming experience is in corporate systems, not games, so I was interested so see how a commercial game was put together. Zero Hour seems like a good case study
because, while it relies on a number of third-party libraries, it doesn't use a separate game engine.

The DynamicMemoryAllocator works. If you compile in debug mode it dumps out memory pool stats, and tells you where you're leaking memory... neat! (I can't take any credit for that. It's all built in.)

The module system uses multiple inheritance. I'm not a big fan of multiple inheritance, and it seems that I'm not the only one, because I found this comment in the code: "I can't take it any more.  Let the record show that I think the UpgradeMux multiple inheritence is CRAP."
-->
