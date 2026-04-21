# Crumar_BIT99_Sysex

The Crumar BIT99 and BIT01 were analog synthesizers with digital control built between 1985 and about 1987 by the Italian firm Crumar. They offer a very interesting analog sound with 6 voices, resonant filters and a dynamic keyboard. They can be programmed with 99 patches (hence the name) that can be sent and received via Sysex.

This project is a small console project in C running on macOS that allows to control a Crumar BIT99/BIT01 via Sysex. It also offers the possibility of interpreting the received data (or the file contents). This is very useful to get a full description of all parameters of a patch of the BIT99.

The code should be reasonably portable, in the sense that all OS-related code is contained in a single file. Feel free to adapt to your favourite OS!

The user interface is minimalistic, but it should be reasonably usable.

Some defaults are hardcoded at the beginning of the main.c file. Change them according to your preferences if you really wants to.

The tool can send commands from one of the available interfaces (always on channel 0) and will receive Sysex data on all available MIDI sources.

As a hint to 1980-style programming, I took the liberty of using a liberal quantity of old-fashioned `goto` commands in [main.c](../blob/master/main.c) ;-) 

## Screenshots

![Screenshot of the main menu of the tool](https://raw.githubusercontent.com/DarwinNE/Crumar_BIT99_Sysex/refs/heads/main/screenshots/main_screen.jpg)

![Screenshot of the patch 22 interpreted](https://raw.githubusercontent.com/DarwinNE/Crumar_BIT99_Sysex/refs/heads/main/screenshots/patch_22.jpg)