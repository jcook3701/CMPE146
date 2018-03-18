//#include "io.hpp" // All IO Class definitions
#include "led_display_extension.hpp"

/**
 * The following diagram shows bit number corresponding to the LED
 *
 *      0
 *     ---
 *  5 | 6 | 1
 *     ---
 *  4 |   | 2
 *     ---
 *      3   *7
 */

static const unsigned char LED_DISPLAY_CHARMAP[6] =
  {
    0x00,  // 0: space
    0x01,  // 1: Top
    0x08,  // 2: Down
    0x30,  // 4: Left
    0x06,  // 2: Right
    0x80,  // 5: Error
  };

bool led_display_extension::init()
{
    bool devicePresent = checkDeviceResponse();
    if(devicePresent)
    {
        const unsigned char cfgAsOutput = 0x00;
        writeReg(cfgPort0, cfgAsOutput);
        writeReg(cfgPort1, cfgAsOutput);

        setLeftDigit('.');
        setRightDigit('.');
    }
    return devicePresent;
}

void led_display_extension::setLeftDigit(char alpha)
{
    mNumAtDisplayExtension[0] = alpha;
    writeReg(outputPort1, LED_DISPLAY_CHARMAP[(unsigned) (alpha & 0x7F) ]);
}

void led_display_extension::setRightDigit(char alpha)
{
    mNumAtDisplayExtension[1] = alpha;
    writeReg(outputPort0, LED_DISPLAY_CHARMAP[(unsigned) (alpha & 0x7F) ]);
}

void led_display_extension::led_up(void)
{
  mNumAtDisplayExtension[0] = TOP;
  mNumAtDisplayExtension[1] = TOP;
  writeReg(outputPort0, LED_DISPLAY_CHARMAP[(unsigned) TOP ]);
  writeReg(outputPort1, LED_DISPLAY_CHARMAP[(unsigned) TOP ]);
}

void led_display_extension::led_down(void)
{
  mNumAtDisplayExtension[0] = DOWN;
  mNumAtDisplayExtension[1] = DOWN;
  writeReg(outputPort0, LED_DISPLAY_CHARMAP[(unsigned) DOWN ]);
  writeReg(outputPort1, LED_DISPLAY_CHARMAP[(unsigned) DOWN ]);
}

void led_display_extension::led_right(void)
{
  mNumAtDisplayExtension[0] = RIGHT;
  mNumAtDisplayExtension[1] = SPACE;
  writeReg(outputPort0, LED_DISPLAY_CHARMAP[(unsigned) RIGHT ]);
  writeReg(outputPort1, LED_DISPLAY_CHARMAP[(unsigned) SPACE ]);
}

void led_display_extension::led_left(void)
{
  mNumAtDisplayExtension[0] = SPACE;
  mNumAtDisplayExtension[1] = LEFT;
  writeReg(outputPort0, LED_DISPLAY_CHARMAP[(unsigned) SPACE ]);
  writeReg(outputPort1, LED_DISPLAY_CHARMAP[(unsigned) LEFT ]);
}

void led_display_extension::led_error(void)
{
  mNumAtDisplayExtension[0] = ERROR;
  mNumAtDisplayExtension[1] = ERROR;
  writeReg(outputPort0, LED_DISPLAY_CHARMAP[(unsigned) ERROR ]);
  writeReg(outputPort1, LED_DISPLAY_CHARMAP[(unsigned) ERROR ]);
}

