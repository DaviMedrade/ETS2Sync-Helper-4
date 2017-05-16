# ETS2Sync Helper v4.0+
[VERSÃO EM PORTUGUÊS](README-pt.md)

[LICENSE](LICENSE.md)

This is the repository for ETS2Sync helper 4.0 and newer. The program has been rewritten from scratch in C++, making it smaller and faster.

This app synchronizes the job list on Euro Truck Simulator 2, in order to make it easier to organize a convoy on Multiplayer.

As of now, the app is available only in English. Other languages might be (re-)added in the future.

## Download
http://files.dsantosdev.com/ets2sync_helper.zip

## Compatibility
This version of ETS2Sync Helper is compatible with Euro Truck Simulator 1.25, but only for saves created in text format (`g_save_format 2`).

If your game is not set up with `g_save_format 2`, the app will allow you to change that. In order to change the save format using the app, you must have the game closed, otherwise the change won't be recognized by the game.

If you have the Developer Console enabled in the game, you can just issue the command `g_save_format 2` before saving the game for sync, then you can issue `g_save_format 0` after that. That way the game won't lag when creating autosaves, which it might to if you drive with `g_save_format` set to Text (2). If you use the Developer Console to set the save format, you can ignore the yellow message in the app saying that the recommended save format is Text.

## Clear Jobs
One feature introduced in version 5.0.0 is the ability to empty the Freight Market. That way, if you are done using the synced list and want the game to generate a new list of jobs for you to play alone, you just need to do the same process as for syncing, but click on “Clear Jobs” instead of “Sync Jobs”. When you load the save, the Freight Market will be empty. Just jump to the mechanic with F7 and the game will generate new jobs for you.

Version 5.1.0 introduced the Economy Reset option. If you select a save and click “Economy Reset”, when you load that save the game will show a “Game Change Detected” message, which will instantly generate new jobs (i.e. no need to call Assistance or do Quick Travels to populate the Freight Market). A downside is that it will also move you to your home garage.

## Bugs/Issues
If you find a problem in the app, report it on the link below:

https://github.com/davidsantos-br/ETS2Sync-Helper-4/issues
