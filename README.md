# SPM Flipside Pit Randomizer Mod
This mod randomizes the contents of the Flipside Pit of 100 Trials and adds a multitude of new features and balance changes.

Install this mod as you would any other Riivolution mod on console or Dolphin; the download can be found [here.](https://github.com/shiken-yme/spm-pit-randomizer/releases)

You may also install the mod through L5050's Flipside Mod Manager (FliMM), which can be found [here.](https://github.com/L5050/Flipside-Mod-Manager) This is highly recommended for playing mods on console, because SPM mods loaded through Riivolution will always crash upon resetting the game.

Please do not enable any more than one SPM mod at one time, as they are not cross-compatible.

### For Console (Wii or vWii):
- If you have a legally-obtained ISO/WBFS rip of the game & a modded Wii/Wii U, you can use [Flipside Mod Manager](https://github.com/L5050/Flipside-Mod-Manager) to directly patch the game and then use your favorite USB Loader to load the game (I recommend using [Wii Backup Manager](https://wiibackupmanager.co.uk/WiiBackupManager_Build78.html) to load your game onto your console).
- Be sure to enable Gecko Codes on your USB Loader and include the REL Loader GCT for your version; instructions for how to do this can be found [here.](https://github.com/SeekyCt/spm-practice-codes/blob/main/INSTALLING.md#console-with-usb-loader-and-patched-iso)

**ALTERNATIVELY,** you may load the mod through Riivolution. Download the ZIP for your version in Releases, then drag & drop the contents of the ZIP to the root of your SD card, insert the SD card into your console, and boot up Riivolution. (To be clear, you want the "PitRando" and "riivolution" folders on the root of your SD card.)

### For Dolphin:
- Find your Riivolution folder (you can find this by right-clicking your game, clicking *Start with Riivolution Patches*, then *Open Riivolution XML*; this will open the directory you need to be in), then drag and drop the contents of the ZIP to that folder (this means a "riivolution" and "PitRando" folder. Close the folder and right-click, select *Start with Riivolution Patches*, then enable the mod.
- Be sure to add/enable the REL Loader Gecko code for your game version before running the game with Riivolution patches. You can find the codes [here.](https://github.com/SeekyCt/spm-rel-loader/tree/master/spm-rel-loader/loader)

## Features
* Randomizes spawn/exit doors & room layouts while (ideally) never creating any areas that are impossible to access without glitches
* Randomizes enemy configurations, with relative difficulty scaling per 25 floors
* Randomizes chest loot in Floors 10/20/etc., with the weight of more valuable items scaling with Pit progress
* Adds Movers, inspired by TTYD Pit: allows you to pay to skip 2 or 5 floors
* Changes some enemy stats and AI
* Reskins some Dark enemies to become new enemy variants & adds previously unused enemies to the Pit, all of which are given a number of stat changes and possibly AI changes
* Overhauls Flimm's inventory, adding many recipes and Catch Cards
* Changes the final boss of the Flipside Pit and gives the player new loot
* Changes a small amount of dialogue
* Makes Pit pipes slightly faster and a little less annoying to travel through
* Quickstart: Create a new save file to get started right away with premade stats, if you'd like
* Removes the Flopside Pit (for now...)

## Planned Features
* "Blessing/curse" system
* Add new enemies and modify some enemy AI to be more distinct/difficult
* Make the Flopside Pit a roguelike, with difficulty and rewards scaling with the player's level
* Make the entire Pit generation process seeded, with everything (save for enemy positions and which one has the key) based entirely on the name of your save file
* Many more things that don't immediately come to mind!

## Known Issues (Will be patched in a future update)
* Muths can spawn directly below the player when entering a room, causing instant unavoidable damage
* Tattles and catch card descriptions are usually quite inaccurate
* Dark Bowser may do 10 damage in some situations when he should not
* Many more things that I'm not thinking of

## Special Thanks
* SeekyCt, JohnP55, and L5050 for their contributions to spm-headers and spm-decomp, as well as showing me the ropes with SPM modding
* SeekyCt, JohnP55, L5050, Starry_Arie, roblox8192, and Zephiles for code help
* Igrmi, JohnP55, follyfoxe, L5050, PistonMiner, and Zephiles for creating crucial tools I relied on *heavily* during the development process
* JohnP55 for creating/implementing the evtpatch engine
* Little Crap, FRN6Phantom, KoraFloof, Nevu_z, and SeekyCt for testing the mod in its alpha stage
* Various members of the SPM Speedrunning community and my friends for encouraging me to continue this project <3

This is my first ever mod, and I could not have created it or learned nearly as much as I have without all of you. Thank you all so much. Please enjoy the mod!
