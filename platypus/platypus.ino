#include <math.h>
#include <Wire.h>
#include <hexbright.h>

// Modes
#define MODE_OFF_PREVIEW        0
#define MODE_OFF                1
#define MODE_STATIC_PREVIEW     2
#define MODE_STATIC             3
#define MODE_DYNAMIC_PREVIEW    4
#define MODE_FADE               5
#define MODE_BLINKING           6
#define MODE_STROBE             7
#define MODE_DAZZLE             8


#define BRIGHTNESS_COUNT 5
int brightness[BRIGHTNESS_COUNT] = {10, 250, 500, 750, 1000};
int current_brightness = 0;
int increase_brightness = 1;

// State
byte mode;
hexbright hb;

void setup()
{
  /// Set DEBUG to a non-zero value in hexbright.h to enable printing
  hb.init_hardware();
  mode = MODE_OFF;
}

void loop()
{
  hb.update();
  hb.print_charge(GLED);

  //if(mode == MODE_DYNAMIC_PREVIEW || mode == MODE_BLINKING) {
  if(mode == MODE_BLINKING) {
    static int i = 0;
    i = (i+1)%40;
    if(i==0) {
      hb.set_light(MAX_LEVEL, 0, 50);
    }
  }
  
  boolean button_just_released = hb.button_just_released();
  boolean button_pressed = hb.button_pressed();
  int button_pressed_time = hb.button_pressed_time();
  
  byte newMode = mode;
  
  switch (mode) {
    case MODE_OFF_PREVIEW:
      if(button_just_released) {
        newMode = MODE_OFF;
      }
      break;
    case MODE_OFF:
      if (button_pressed && button_pressed_time > 500) {
        newMode = MODE_DYNAMIC_PREVIEW;        
      } else if (button_pressed) {
        newMode = MODE_STATIC_PREVIEW;
      }
      break;
    case MODE_STATIC_PREVIEW:
      if (button_pressed && button_pressed_time > 2000) {
        newMode = MODE_OFF_PREVIEW;
      } else if(button_just_released) {
        newMode = MODE_STATIC;
      }
      break;
    case MODE_STATIC:
      if (button_pressed && button_pressed_time > 2000) {
        newMode = MODE_OFF_PREVIEW;
      } else if (button_pressed && button_pressed_time > 500) {
        newMode = MODE_DYNAMIC_PREVIEW;
      } else if (button_just_released) {
        cycleBrightness();
      }
      break;
    case MODE_DYNAMIC_PREVIEW:
      if (button_pressed && button_pressed_time > 2000) {
        newMode = MODE_OFF_PREVIEW;
      } else if(button_just_released) {
          //newMode = MODE_FADE;
          newMode = MODE_BLINKING;
          //newMode = MODE_STROBE;
          //newMode = MODE_DAZZLE;
      }
      break;
    case MODE_BLINKING:
      if (button_pressed && button_pressed_time > 2000) {
        newMode = MODE_OFF_PREVIEW;
      } else if (button_pressed && button_pressed_time > 500) {
        newMode = MODE_STATIC_PREVIEW;
      } else if(button_just_released) {
        newMode = MODE_BLINKING;
      }
      break;
  }

  // Do the mode transitions
  if (newMode != mode) {
    switch (newMode) {
      case MODE_OFF_PREVIEW:
        Serial.println("Mode = off_preview");
        hb.set_light(0, OFF_LEVEL, NOW);
        break;
      case MODE_OFF:
        Serial.println("Mode = off");
        current_brightness = 0;
        hb.set_light(0, OFF_LEVEL, NOW);
        break;
      case MODE_STATIC_PREVIEW:
        Serial.println("Mode = static_preview");
        hb.set_light(CURRENT_LEVEL, brightness[current_brightness], NOW);
        break;
      case MODE_STATIC:
        Serial.println("Mode = static");
        increase_brightness = 1;
        hb.set_light(CURRENT_LEVEL, brightness[current_brightness], NOW);
        break;
      case MODE_DYNAMIC_PREVIEW:
        Serial.println("Mode = dynamic_preview");
        break;
      case MODE_BLINKING:
        Serial.println("Mode = blinking");
        break;
    }
    mode = newMode;
  }

}

void cycleBrightness() {
  Serial.println("Cycle Brightness!");
  
  if(current_brightness == BRIGHTNESS_COUNT-1 && increase_brightness == 1) {
    increase_brightness = 0;
  } else if(current_brightness == 0 && increase_brightness == 0) {
    increase_brightness = 1;
  }
  
  if(increase_brightness == 1) {
    current_brightness = (current_brightness+1)%BRIGHTNESS_COUNT;
  } else {
    current_brightness = (current_brightness-1)%BRIGHTNESS_COUNT;
  }
  hb.set_light(CURRENT_LEVEL, brightness[current_brightness], NOW);
}

