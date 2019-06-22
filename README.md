# ETS2Sync Helper v4.0+
[VERSÃO EM PORTUGUÊS](README-pt.md)

[LICENSE](LICENSE.md)

* [Download](#download)
* [Setting Up](#setting-up)
  * [Simple Method](#setting-up-simple-method)
  * [Advanced Method](#setting-up-advanced-method)
    * [Using the Console](#advanced-method-using-the-console)
* [How To Sync](#how-to-sync)
* [Features](#features)
  * [Clear Jobs](#clear-jobs)
  * [Economy Reset](#economy-reset)
  * [Job List](#job-list)
* [Common Errors](#common-errors)
* [Compiling The Source Code](#compiling-the-source-code)

This is the repository for ETS2Sync helper 4.0 and newer. The program has been rewritten from scratch in C++, making it smaller and faster.

This app synchronizes the job list on Euro Truck Simulator 2 or American Truck Simulator, in order to make it easier to organize a convoy on Multiplayer.

As of now, the app is available only in English. Other languages might be (re-)added in the future.

## Download
https://github.com/davidsantos-br/ETS2Sync-Helper-4/releases

## Setting Up
Before we start syncing, you need to choose whether you'll use the simple or the advanced method. The simple method is easier, but it makes ETS2/ATS freeze for a second or two every few minutes when the game creates an autosave. That might make people crash into you since for them your truck will suddenly stop and then continue moving after a second or two. For that reason, I recommend the advanced method. It is a bit more complex, but it avoids those autosave lags. Follow the section below corresponding to the method of your choice. You'll only need to do this once for each game (i.e. once for ETS2 and once for ATS).

### Setting Up: Simple Method
Open ETS2Sync Helper and select the game (ETS2 or ATS). Look in the “Game Settings” panel. If there is a message in green saying “Settings OK”, you're all set. If the message is yellow or red, you need to do this:
1. Make sure the game is closed. This won't work if you do it with the game running.
2. In ETS2Sync Helper, in the “Game Settings” panel, in the “Save Format” line, click “Set to ‘Text’”. The yellow or red message should turn green and say “Settings OK”. That's it. Now your game is set up for syncing, so you can go to [How To Sync](#how-to-sync).

### Setting Up: Advanced Method
For this method, you need to open `config.cfg` in the game folder in Documents, and change `g_developer` to 1 (one), `g_save_format`to 0 (zero), and `g_console` to 1 (one). The first and the third changes enable the Developer Console—so you can skip them if it's already enabled—and the second one changes the Save Format to Binary—so you can skip it if ETS2Sync Helper says it's already set to Binary. Here's a step-by-step on how to do those changes:

1. Make sure the game is closed. This won't work if you do it with the game running.
2. Open ETS2Sync Helper. If you're playing ATS, select “ATS” in the “Game” option.
3. In the “Game Settings” panel, there is a line saying “Settings Folder: <folder with your ETS2/ATS settings> - Options”. Click “Options” → “Open game config file”. A Notepad window will appear, with the game settings file open.
4. In that Notepad window, go to “Edit” → “Find…” (or hit <kbd>Ctrl</kbd>+<kbd>F</kbd>). In the search window, type `g_developer` (notice the underscore) and hit <kbd>Enter</kbd>. It should highlight a line in the file that reads `uset g_developer "0"`. If the number isn't already `1`, change it to `1`, so that it reads `uset g_developer "1"`.
4. Go to “Edit” → “Find…” (or hit <kbd>Ctrl</kbd>+<kbd>F</kbd>) again. In the search window, type `g_save_format` and hit <kbd>Enter</kbd>. It should highlight a line in the file that reads `uset g_save_format "2"` or another number. If the number is not zero, change the number to `0`, so that it reads `uset g_save_format "0"`.
5. Go to “Edit” → “Find…” (or hit <kbd>Ctrl</kbd>+<kbd>F</kbd>) again. In the search window, type `g_console` and hit <kbd>Enter</kbd>. It should highlight a line in the file that reads `uset g_console "0"`. If the number isn't already `1`, change it to `1`, so that it reads `uset g_console "1"`.
6. Save the file and close Notepad. You can ignore the yellow message in ETS2Sync Helper that says the recommended format is Text.

#### Advanced Method: Using the Console
Once you have the Console enabled (as per the [Setting Up: Advanced Method](#setting-up-advanced-method) section), when you're in the game, you can open and close the Console with the key above <kbd>Tab</kbd>, to the left of the <kbd>1</kbd> key in the alphanumeric keyboard.

When you need to issue a command in the Console, just open the Console, type the command as written (or copy it and hit <kbd>Ctrl</kbd>+<kbd>V</kbd> in the Console to paste it), then hit <kbd>Enter</kbd>. If a red message that says `unknown command` appears, you probably made a mistake when typing the command. Be sure to type exactly as shown: in lowercase, with underscores and spaces in the right places, and without mixing up `1` (one) with `l` (lowercase `L`) or `0` (zero) with `O` (uppercase `O`).

You can retype a command you used before by using <kbd>↑</kbd> (up arrow) and <kbd>↓</kbd> (down arrow) to browse the command history. Once you find the command you want, you can change it if needed and then hit <kbd>Enter</kbd> to execute it.

If you type the beginning of a command and hit <kbd>Tab</kbd>, the Console will autocomplete it for you, but only if there aren't any other commands that begin the same way. If there are the Console will show a list of possible commands, then you can type a few more characters to disambiguate and hit <kbd>Tab</kbd> again. If you hit <kbd>Tab</kbd> and nothing happens, it means the characters you typed don't match up with the beginning of any valid command. Please note that autocompletion only works for the first “word” of the command (i.e. the part before the first space).

## How To Sync
1. Go to the main screen (if you are driving, hit <kbd>Esc</kbd>).
2. **Advanced Method only:** In the Console, issue the command `g_save_format 2`. You can check [Advanced Method: Using the Console](#advanced-method-using-the-console) to learn how to do that.
3. Click on “Save & Load” → “Save Game”.
4. Type a name for the save or select an existing save to overwrite it.
5. Click “Save”.
6. Hit `Alt`+`Tab` to minimize the game.
7. Open ETS2Sync Helper if it isn't already open. Make sure the correct game is selected  in the “Game” option of the “Game Settings” panel.
8. Make sure the correct profile and save are selected in the “Select ETS2/ATS Profile” and “Select ETS2/ATS Save” panels.
9. Select the DLCs you and your friends have (do not select a DLC unless everyone you're playing with owns it too).
10. Click “Sync Jobs”.
11. When the message “Sync Complete” appears, click “Close”.
12. Go back to the game.
13. Click on “Save & Load” → “Load Game”.
14. Select the save you just synced (the one you created/replaced in step 4).
15. Click “Load”.
16. **Advanced Method only:** In the Console, issue the command `g_save_format 0` (notice that it's a digit zero, not an uppercase “O”).
17. Check the Freight Market. If all jobs have the same time to expire (“Offer expires in”), and it's about 400 to 500 hours, then the sync was successful.

## Features
### Clear Jobs
This feature gives you the ability to empty the Freight Market. That way, if you are done using the synced list and want the game to generate a new list of jobs for you to play alone, you just need to do the same process as for syncing, but click on “Clear Jobs” instead of “Sync Jobs”. When you load the save, the Freight Market will be empty. Just jump to the mechanic with F7 or do a Quick Trip or two and the game will generate new jobs for you.

### Economy Reset
This feature works like Clear Jobs, but it's faster because when you load the save the game will show a “Game Change Detected” message, which will instantly generate new jobs (i.e. no need to call Assistance or do Quick Travels to populate the Freight Market). A downside is that it will also move you to your home garage.

### Job List
When you sync, the app downloads a list of jobs from the server and inserts it into your save. There are eight job lists for ETS2 and eight for ATS on the server. One of those eight lists is the one that is active for the week—the one that will be used when you leave the “Job List” option in the “Automatic” setting. Every Wednesday the active list switches to the next one (i.e. if List 7 is the active list, then on Wednesday it will switch to List 8, and on the following Wednesday it goes back to List 1).

The job lists themselves don't change, except for when a game update makes it necessary. So, if you want the list of jobs not to change—for example, if it's Monday and you are planning a convoy for the weekend—, just manually select one of the eight lists and be sure to tell everyone in the convoy to pick the same job list in the app before syncing.

## Common Errors

### Unfortunately, binary saves aren't supported yet. Please create the save using a text format.
**Simple Method**: You selected an old save or the game isn't set up properly.  
* If ETS2Sync Helper doesn't show a green “Settings OK” message in the “Game Settings” panel, you need to do the steps in the [Setting Up: Simple Method](#setting-up-simple-method) section again.
* If it was already showing “Settings OK”, make sure the correct profile and save are selected before syncing.
* If it still doesn't work, close ETS2Sync Helper and close the game, then reopen ETS2Sync Helper and see if the green “Settings OK” message is there. If it isn't, see above. If it is, open the game and make sure to create a new save before syncing again.

**Advanced Method**: The wrong profile or save is selected in ETS2Sync Helper, or you forgot to issue `g_save_format 2` in the Console before creating the save you tried to sync.

### Could not download the job list. HTTP 426 Upgrade Required
You are using an outdated version of ETS2Sync Helper. Check the [Releases](https://github.com/davidsantos-br/ETS2Sync-Helper-4/releases) page and download the latest version. If this happens even after you downloaded the latest release, it means a new version is about to be released. Wait a few minutes and check the Releases page again.

### Could not download the job list. InternetOpenURL error
### Could not download the job list. HTTP 500 Internal Server Error
### Could not download the job list. HTTP 401 Unauthorized
### Could not download the job list. HTTP 403 Forbidden
### Could not download the job list. HTTP 404 Not Found
This is likely a temporary issue with your Internet connection or with the server from which ETS2Sync Helper downloads the job list. If your Internet connection is working properly, wait a few minutes and try again.

### Advanced Method: `unknown command` in the Console
You probably typed the command incorrectly. Please note that the `g_save_format` command needs to be typed exactly as shown—in lowercase, with the underscores—and there needs to be a space before the `2` or the `0`.

## Bugs/Issues
If you find a problem in the app, report it on the link below:

https://github.com/davidsantos-br/ETS2Sync-Helper-4/issues

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
