# arduLCDkeypad

![arduLCDkeyboard](arduLCDkeyboard.JPG)

- Designed to reduce the workload and pin count required to incorporate a LCD and keypad.  Pease copy and improve this project in any way you wish.

## Hardware
- ATmega328 based LCD piggy back board with 5x4 Keypad port.
- ATmega328 using internal 8Mhz clock.
- uses MCUdude Minicore bootloader.
- Arduino source code.
- Piggyback for standard LCD up to 20x4 LCD.
- Serial and I2c interface.
- Variable contrast control.
- Variable Backlight control.
- Piezo, click sound on key press.

## Firmware
- Communicate Commands by either 9600 serial or I2c (address 42).
- Basic LCD fuctions like Clear, Cursor, Print, Print at,
### Extra features
- Predefine a field (template) with a format string (as in C printf format).  Just update a variable value and it will format and print it at the right location.
- Handles Float Formats (basic).  eg., "%03.5f"
- Predefined Time formats.  ("HH:mm:ss") or ("YYYY-MM-dd HH:mm:ss") Using the long value (time_t ) as the variable type.
- Auto time mode: using Time library to maintain a real time clock; if used it will autonomously refresh the time field every second.
- Dual settings for backlight level.  Keypress activates higher backlight level and times out to lower level.

## Library
- Simple interfacing to arduLCDkeypad.

