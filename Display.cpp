#include "Display.h"

uint8_t bell[8]  = {0x4,0xe,0xe,0xe,0x1f,0x0,0x4};
uint8_t note[8]  = {0x2,0x3,0x2,0xe,0x1e,0xc,0x0};
uint8_t clock[8] = {0x0,0xe,0x15,0x17,0x11,0xe,0x0};
uint8_t heart[8] = {0x0,0xa,0x1f,0x1f,0xe,0x4,0x0};
uint8_t duck[8]  = {0x0,0xc,0x1d,0xf,0xf,0x6,0x0};
uint8_t check[8] = {0x0,0x1,0x3,0x16,0x1c,0x8,0x0};
uint8_t cross[8] = {0x0,0x1b,0xe,0x4,0xe,0x1b,0x0};
uint8_t retarrow[8] = {  0x1,0x1,0x5,0x9,0x1f,0x8,0x4};

Display::Display(int8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows)
: LiquidCrystal_I2C(lcd_Addr, lcd_cols, lcd_rows)
{}

void Display::setup()
{
  init();                      // initialize the lcd 
  backlight();
  
  createChar(BELL, bell);
  createChar(NOTE, note);
  createChar(CLOCK, clock);
  createChar(HEART, heart);
  createChar(DUCK, duck);
  createChar(CHECK, check);
  createChar(CROSS, cross);
  createChar(RETARROW, retarrow);
  home();
}

void Display::draw_frame()
{
  setCursor(0, 0);
  for(int i = 0; i < _cols; i++)
    write(Display::CROSS);
    
  setCursor(0, 1);
  write(Display::CROSS);
  setCursor(_cols-1, 1);
  write(Display::CROSS);
  
  setCursor(0, 2);
  write(Display::CROSS);
  setCursor(_cols-1, 2);
  write(Display::CROSS);
  
  setCursor(0, 3);
  for(int i = 0; i < _cols; i++)
    write(Display::CROSS);
}
