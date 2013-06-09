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

// State
byte mode;
hexbright hb;
char fade_direction = -1;

/***
 *
 * Edit these 4 parameters for your own brightness levels,
 * which level to use first and wether to increase or
 * decrease from that level to the next
 *
 ***/

#define BRIGHTNESS_COUNT 5
int brightness[BRIGHTNESS_COUNT] = {10, 250, 500, 750, 1000};
int current_brightness = 0;
int increase_brightness = 1;

/***
 *
 * Define the time (in milliseconds) for switching between
 * static/non-static mode and until switching off
 *
 ***/

int HOLD_SHORT = 500;
int HOLD_LONG = 2000;


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

  boolean button_just_released = hb.button_just_released();
  boolean button_pressed = hb.button_pressed();
  int button_pressed_time = hb.button_pressed_time();
  
  byte newMode = mode;
  
  switch (mode) {
    case MODE_OFF_PREVIEW:
      if (button_just_released) {
        newMode = MODE_OFF;
      }
      break;
    case MODE_OFF:
      if (button_pressed && button_pressed_time > HOLD_SHORT) {
        newMode = MODE_DYNAMIC_PREVIEW;        
      } else if (button_pressed) {
        newMode = MODE_STATIC_PREVIEW;
      }
      break;
    case MODE_STATIC_PREVIEW:
      if (button_pressed && button_pressed_time > HOLD_LONG) {
        newMode = MODE_OFF_PREVIEW;
      } else if (button_just_released) {
        newMode = MODE_STATIC;
      }
      break;
    case MODE_STATIC:
      if (button_pressed && button_pressed_time > HOLD_LONG) {
        newMode = MODE_OFF_PREVIEW;
      } else if (button_pressed && button_pressed_time > HOLD_SHORT) {
        newMode = MODE_DYNAMIC_PREVIEW;
      } else if (button_just_released) {
        cycleBrightness();
      }
      break;
    case MODE_DYNAMIC_PREVIEW:
      if (button_pressed && button_pressed_time > HOLD_LONG) {
        newMode = MODE_OFF_PREVIEW;
      } else if (button_just_released) {
          /***
           *
           * Edit this for the first non-static mode to use
           *
           ***/
          //newMode = MODE_FADE;
          newMode = MODE_BLINKING;
          //newMode = MODE_STROBE;
          //newMode = MODE_DAZZLE;
      }
      break;
    case MODE_FADE:
      if (button_pressed && button_pressed_time > HOLD_LONG) {
        newMode = MODE_OFF_PREVIEW;
      } else if (button_pressed && button_pressed_time > HOLD_SHORT) {
        newMode = MODE_STATIC_PREVIEW;
      } else if (button_just_released) {
        newMode = MODE_BLINKING;
      }
      break;
    case MODE_BLINKING:
      if (button_pressed && button_pressed_time > HOLD_LONG) {
        newMode = MODE_OFF_PREVIEW;
      } else if (button_pressed && button_pressed_time > HOLD_SHORT) {
        newMode = MODE_STATIC_PREVIEW;
      } else if (button_just_released) {
        newMode = MODE_STROBE;
      }
      break;
    case MODE_STROBE:
      if (button_pressed && button_pressed_time > HOLD_LONG) {
        newMode = MODE_OFF_PREVIEW;
      } else if (button_pressed && button_pressed_time > HOLD_SHORT) {
        newMode = MODE_STATIC_PREVIEW;
      } else if (button_just_released) {
        newMode = MODE_DAZZLE;
      }
      break;
    case MODE_DAZZLE:
      if (button_pressed && button_pressed_time > HOLD_LONG) {
        newMode = MODE_OFF_PREVIEW;
      } else if (button_pressed && button_pressed_time > HOLD_SHORT) {
        newMode = MODE_STATIC_PREVIEW;
      } else if (button_just_released) {
        newMode = MODE_FADE;
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
      case MODE_FADE:
        Serial.println("Mode = fade");
        break;
      case MODE_BLINKING:
        Serial.println("Mode = blinking");
        break;
      case MODE_STROBE:
        Serial.println("Mode = strobe");
        break;
      case MODE_DAZZLE:
        Serial.println("Mode = dazzle");
        break;
    }
    mode = newMode;
  }
  
  /***
   *
   * Change the conditional check of the non-static mode
   * you have set as your first to already use it while
   * the button is still pressed (>500 to <2000 ms)
   *
   ***/
  
  //if (mode == MODE_DYNAMIC_PREVIEW || mode == MODE_BLINKING) {
  if (mode == MODE_BLINKING) {
    static int i = 0;
    i = (i+1)%40;
    if(i==0) {
      hb.set_light(MAX_LEVEL, 0, 50);
    }
  }
  
  //if (mode == MODE_DYNAMIC_PREVIEW || mode == MODE_STROBE) {
  if (mode == MODE_STROBE) {
    static unsigned long flash_time = millis();
    if(flash_time+70<millis()) {
      flash_time = millis();
      hb.set_light(MAX_LEVEL, 0, 20);
    }
  }
  
  //if (mode == MODE_DYNAMIC_PREVIEW || mode == MODE_DAZZLE) {
  if (mode == MODE_DAZZLE) {
    hb.set_light(0, (random(4)<1)*1000, NOW);
  }
  
  //if (mode == MODE_DYNAMIC_PREVIEW || mode == MODE_FADE) {
  if (mode == MODE_FADE) {
    if (!hb.light_change_remaining()) {
      if (fade_direction<0) {
        hb.set_light(CURRENT_LEVEL, 1000, 1000-(hb.get_light_level()));
        fade_direction = 1;
      } else {
        hb.set_light(CURRENT_LEVEL, 1, hb.get_light_level());
        fade_direction = -1;
      }
    }
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

