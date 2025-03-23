
# Command & Conquer Generals Zero Hour native Linux port

I'm attempting to create a native Linux port of Zero Hour. I suspect that it's a fool's errand, but it should be an interesting experience.


## Progress so far

- Displays a window with the splash screen.
- The DynamicMemoryAllocator works.


## Still to do

- Everything else.


## Dependencies

I'm using SDL3 for the window management, so you'll have to install that.

You need to create a symlink called `assets` in the `GeneralsMD` directory that points to the directory where you have Zero Hour installed, such that typing `ls assets/AudioZH.big` should work. At the moment it's used to load the splash screen, but eventually it will be used to load all of the assets.


## Compiling

I'm using a standard makefile. Open a terminal in the `GeneralsMD/Code` directory, and type `make`, or `make DEBUG=1` for a debug build.

The compiled executable is `GeneralsMD/Run/rts`.

When you run it, if you get an error message that says:

    ERROR: Could not load bitmap: Couldn't open ../assets/Install_Final.bmp: No such file or directory

then you forgot to create the symlink mentioned above.

Click on the close button in the splash screen window to exit the "game".
