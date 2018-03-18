#ifndef LED_DISPLAY_EXTENSION_HPP__
#define LED_DISPLAY_EXTENSION_HPP__

#include "i2c2_device.hpp"  // I2C Device Base class

class led_display_extension: public i2c2_device, public SingletonTemplate<led_display_extension>
{
public:
  bool init(void);
  void setLeftDigit(char alpha);
  void setRightDigit(char alpha);
  void led_up(void);
  void led_down(void);
  void led_left(void);
  void led_right(void);
  void led_error(void); 
private:
  char mNumAtDisplayExtension[2]; ///< The number currently being displayed

  led_display_extension() : i2c2_device(I2CAddr_LED_Display)
    {
    }
  friend class SingletonTemplate<led_display_extension>;  ///< Friend class used for Singleton Template
  
  /// Enumeration of the register map
  typedef enum {
    inputPort0, inputPort1,
    outputPort0, outputPort1,
    polarityPort0, polarityPort1,
    cfgPort0, cfgPort1
  } __attribute__ ((packed)) RegisterMap;

  enum direction_t
    {
      SPACE = 0,
      TOP   = 1,
      DOWN  = 2,
      LEFT  = 3,
      RIGHT = 4,
      ERROR = 5
    };
  
};

#endif /* LED_DISPLAY_EXTENSION_HPP__ */
