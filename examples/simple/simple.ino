/*
  *
 * Tested with
 *    ESP32 (WEMOS LOLIN32 Lite), ESP32-S2 (WEMOS MINI S2), ESP32-C3 (SEEED XIAO-ESP32C3), ESP32-S3 (SEEED XIAO-ESP32S3)
 */

// SPDX-License-Identifier: 0BSD

#include <Arduino.h>           // for PlatformIO
#include "unixbuildtime.hpp"

const char* timeZone = "AST4ADT,M3.2.0,M11.1.0";

#if defined ARDUINO_LOLIN32_LITE
#define SERIAL_BAUD 115200  
#endif

void setup() {
  struct timeval tv;
  struct tm* tminfo;
  char buildtimebuffer[120];
  setenv("TZ", timeZone, 1);  // needed to convert local time to UTC time
  tv.tv_sec = unixbuildtime();      // UTC UNIX (POSIX) timestamp 
  tv.tv_usec = 0;
  settimeofday(&tv, NULL);          // use it to set the rtc
  tminfo = localtime(&tv.tv_sec);
  strftime (buildtimebuffer,120," Initial ESP32 RTC time: %b %d %Y %H:%M:%S", tminfo);

  #ifdef SERIAL_BAUD
  Serial.begin(SERIAL_BAUD);  // Serial through serial-usb adapter, i.e. ESP32 Lolin Lite
  #else  
  Serial.begin();             // Serial over on board USB CDC, i.e. ESP32C3, ESP32S2, ESP32S3
  #endif

  delay(10000); // 10 seconds to "find" the serial monitor

  Serial.println("Simple unixbuildtime demonstration sketch");
  Serial.printf("unixbuildtime: %u\n", (uint32_t) tv.tv_sec);
  Serial.println("The ESP32 RTC was set with the unixbuildtime about 10 seconds ago."); 
  Serial.printf("  __DATE__ and __TIME__: %s\n", __DATE__ " " __TIME__);
  Serial.printf("%s\n", buildtimebuffer);
} 

void loop(void) {
  time_t now;
  struct tm timeinfo; 
  char buffer[120]; 
  time(&now);
  localtime_r(&now, &timeinfo); 
  strftime (buffer,120,"     Current local time: %b %d %Y %H:%M:%S", &timeinfo);
  Serial.println(buffer);
  delay(10000);
}
