#ifndef STRING_FORMATTER_H
#define STRING_FORMATTER_H

#include <Arduino.h>
#include <stdarg.h>

class StringFormatter
{
public:
  static String formatString(char *fmt, ... )
  {
    char buf[128]; // resulting string limited to 128 chars
    va_list args;
    va_start (args, fmt );
    vsnprintf(buf, 128, fmt, args);
    va_end (args);
  
    buf[127] = 0;
    return String(buf);
  }

  static String formatString(const __FlashStringHelper *fmt, ... )
  {
    char buf[128]; // resulting string limited to 128 chars
    va_list args;
    va_start (args, fmt);
#ifdef __AVR__
    vsnprintf_P(buf, sizeof(buf), (const char *)fmt, args); // progmem for AVR
#else
    vsnprintf(buf, sizeof(buf), (const char *)fmt, args); // for the rest of the world
#endif
    va_end(args);

    buf[127] = 0;
    return String(buf);
  }
};

#endif // STRING_FORMATTER_H
