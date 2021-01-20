# ETS2&ATS Sync Helper

## ETS2 support: version 1.39 and all DLCs.
## ATS support: version 1.39 and all DLCS. (in pre release state)

* [Version française](README-fr.md)

[LICENSE](LICENSE.md)

* [Download](#download)
* [Setting Up](#setting-up)
* [How To Sync](#how-to-sync)
* [Features](#features)
  * [Clear Jobs](#clear-jobs)
  * [Economy Reset](#economy-reset)
  * [Job List](#job-list)
* [Common Errors](#common-errors)
* [Bugs/Issues](#Bugs/Issues)
* [Compiling The Source Code](#compiling-the-source-code)
* [Dependencies](#Dependencies)
* [Compilation](#Compilation)
* [Backend](#Backend)

This is the repository for ETS2&ATS Sync helper. The program has been rewritten from scratch in C++ by David Santos, I am now keeping up to date to support game's update and New DLCs

This app synchronizes the job list on Euro Truck Simulator 2 or American Truck Simulator, in order to make it easier to organize a convoy on Multiplayer.

As of now, the app is available only in English. Other languages might be added in the future.

## Download
https://github.com/Inklay/ETS2-ATS-Sync-Helper/releases

## Setting Up
Open ETS2Sync Helper and select the game (ETS2 or ATS). Look in the “Game Settings” panel. If there is a message in green saying “Settings OK”, you're all set. If the message is yellow or red, you need to do this:
1. Make sure the game is closed. This won't work if you do it with the game running.
2. In ETS2Sync Helper, in the “Game Settings” panel, in the “Save Format” line, click “Set to ‘Text’”. The yellow or red message should turn green and say “Settings OK”. That's it. Now your game is set up for syncing, so you can go to [How To Sync](#how-to-sync).


## How To Sync
1. Go to the main screen (if you are driving, hit <kbd>Esc</kbd>).
2. Click on “Save & Load” → “Save Game”.
3. Type a name for the save or select an existing save to overwrite it.
4. Click “Save”.
5. Hit `Alt`+`Tab` to minimize the game.
6. Open ETS2Sync Helper if it isn't already open. Make sure the correct game is selected in the “Game” option of the “Game Settings” panel.
7. Make sure the correct profile and save are selected in the “Select ETS2/ATS Profile” and “Select ETS2/ATS Save” panels.
8. Select the DLCs you and your friends have (do not select a DLC unless everyone you're playing with owns it too).
9. Click “Sync Jobs”.
10. When the message “Sync Complete” appears, click “Close”.
11. Go back to the game.
12. Click on “Save & Load” → “Load Game”.
13. Select the save you just synced (the one you created/replaced in step 4).
14. Click “Load”.
15. Check the Freight Market and or the Cargo Market. If all jobs have the same time to expire (“Offer expires in”), and it's about 400 to 500 hours, then the sync was successful.

## Features
### Clear Jobs
This feature gives you the ability to empty the Freight Market. That way, if you are done using the synced list and want the game to generate a new list of jobs for you to play alone, you just need to do the same process as for syncing, but click on “Clear Jobs” instead of “Sync Jobs”. When you load the save, the Freight Market will be empty. Just jump to the mechanic with F7 or do a Quick Trip or two and the game will generate new jobs for you.

### Economy Reset
This feature works like Clear Jobs, but it's faster because when you load the save the game will show a “Game Change Detected” message, which will instantly generate new jobs (i.e. no need to call Assistance or do Quick Travels to populate the Freight Market). A downside is that it will also move you to your home garage.

### Job List
When you sync, the app downloads a list of jobs from the server and inserts it into your save. There are eight job lists for ETS2 and eight for ATS on the server. One of those eight lists is the one that is used when you leave the “Job List” option in the “Automatic” setting. Every Wednesday the active list switches to the next one (i.e. if List 7 is the active list, then on Wednesday it will switch to List 8, and on the following Wednesday it goes back to List 1).

The job lists themselves don't change, except for when a game update makes it necessary (a game or DLC update or a new DLC). So, if you want the list of jobs not to change—for example, if it's Monday and you are planning a convoy for the weekend—, just manually select one of the eight lists and be sure to tell everyone in the convoy to pick the same job list in the app before syncing.

## Common Errors

### Unfortunately, binary saves aren't supported. Please create the save using a text format.
You selected an old save or the game isn't set up properly.  
* If ETS2Sync Helper doesn't show a green “Settings OK” message in the “Game Settings” panel, you need to do the steps in the [Setting Up](#setting-up) section again.
* If it was already showing “Settings OK”, make sure the correct profile and save are selected before syncing.
* If it still doesn't work, close ETS2Sync Helper and close the game, then reopen ETS2Sync Helper and see if the green “Settings OK” message is there. If it isn't, see above. If it is, open the game and make sure to create a new save before syncing again.

### Could not download the job list. InternetOpenURL error
### Could not download the job list. HTTP 500 Internal Server Error
### Could not download the job list. HTTP 401 Unauthorized
### Could not download the job list. HTTP 403 Forbidden
### Could not download the job list. HTTP 404 Not Found
This is likely a temporary issue with your Internet connection or with the server from which ETS2&ATS Sync Helper downloads the job list. If your Internet connection is working properly, wait a few minutes and try again.

## Bugs/Issues
If you find a problem in the app, report it on the link below:

https://github.com/Inklay/ETS2-ATS-Sync-Helper/issues

## Compiling The Source Code
Before you can compile, you need to set up the dependencies.

### Dependencies
For those that need to be compiled, compilation instructions can often be found in the downloaded package or in the library's documentation.

* **wxWidgets 3.1** — download and compile, then set the environment variable `WXWIN` to its directory.
   
   wxWidgets provides several utility classes used in the app, most importantly the GUI and thread support. The app code would be a lot more complex if those had to be implemented using the native Windows API.
* **zlib** — download and compile, then set the environment variable `ZLIBDIR` to its directory.

   When encrypted `.sii` files are generated by the games, the data is compressed and then encrypted. Zlib is used to decompress the data, after it is decrypted.
* **Ragel 6** — download and install a binary version for Windows (`ragel.exe`) or download and compile the source code, then add the directory of `ragel.exe` to your `PATH`.

   Ragel is used to generate the code for the state machines that parse `.cfg` and text-mode `.sii` files.
* **GraphViz** — download and install a binary version for Windows or download and compile, then add the directory of `dot.exe` to your `PATH`.

   GraphViz generates a visual representation of the parsers generated by Ragel. That makes it easier to understand how those parsers work and therefore how to fix problems in them if they arise.

### Compilation
Once the dependencies are in place, just open `ETS2Sync-Helper-4.sln` in Visual Studio 2017 and compile normally. Be aware that in a debug build the sync process is noticeably slower.

### Backend
The backend code can be found here: https://github.com/Inklay/WebService
