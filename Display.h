#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED

#include "libs/LiquidCrystal_I2C/LiquidCrystal_I2C.h"

class Display : public LiquidCrystal_I2C
{
public:
  enum {BELL, NOTE, CLOCK, HEART, DUCK, CHECK, CROSS, RETARROW } Symbols;

  Display(int8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows);
  virtual ~Display()
  {}

  void setup();
  void draw_frame();
};

#endif //DISPLAY_H_INCLUDED
