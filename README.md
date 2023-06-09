# mdBuildTime Library

An [Arduino](https://www.arduino.cc/) library for ESP32 microcontrollers
that returns the firmware build time as a UNIX (POSIX) `time_t` value.

## Purpose

Use to set the ESP32 real time clock or an external real time clock to an initial
value that is more reasonable than Jan 1, 1970, until an accurate time is available.

This is for **sketches compiled with the Arduino IDE**. The compile time is available as a built-in variable in[PlatformIO](https://platformio.org/).

## Dependencies

The C time library `<ctime>` (header file: `time.h`) which is included in the ESP32 Arduino core.

## Usage

See the example sketch.

## Notes

1. The compile time is obtained by parsing the predefined macros `__DATE__` and `__TIME__` which provide the date and time at which the compilation began. These strings give the local time at least in Debian. So if the RTC is to be set to UTC time, which is deemed preferable, the local time zone must be set before calling `unixbuildtime()` as shown in the above example. 

2. It looks as if the Arduino IDE caches some compiled modules which includes `__DATE__` and `__TIME__` or `unixbuildtime()`. The ouput directory has to be cleaned before compiling the sketch or else time may move backwards.

## PlatformIO 

The PlatformIO [built-in variable](https://docs.platformio.org/en/latest/projectconf/sections/env/options/build/build_flags.html#built-in-variables) [$UNIX_TIME](https://github.com/platformio/platformio-core/blob/develop/platformio/builder/main.py), which returns the host computer UTC time when the firmware is compiled, can be injected into a build flag.

Consequently the above is accomplished with an entry in the `platformio.ini` file:

```ini
[env]
framework = arduino
platform = espressif32
board = seeed_xiao_esp32c3
monitor_speed = 460800
build_flags =
  -DCOMPILE_TIME=$UNIX_TIME
```

and then the use of `COMPILE_TIME` in the `setup()` function without recourse to a library.

```cpp
void setup(void) {
...
  struct timeval tv;
  tv.tv_sec = COMPILE_TIME
  tv.tv_usec = 0;
  settimeofday(&tv, NULL);    // ESP32 RTC is now set with the firmware compile time (UTC)
...
}
```

## Similar Libraries and Credits 

The parsing of the `__DATE__` and `__TIME__` macros comes from [buildTime 1.0 by Alex Gyver](https://github.com/GyverLibs/buildTime) with only slight modification.

The [arduino_compiledate](https://github.com/mikaelsundin/arduino_compiledate) library by Mikael Sundin is similar . The note about the need for specifying the time zone remains valid with this library. Furthermore, the result returned can be off when daylight saving time applies. In that case it would be worthwhile to assign `-1` to `result.tm_isdst` in line 77 of `arduino_compiledate.cpp`.

Both of these libraries were released uder the MIT license.

## Licence

The **BSD Zero Clause** ([SPDX](https://spdx.dev/): [0BSD](https://spdx.org/licenses/0BSD.html)) licence applies.
