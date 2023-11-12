#ifndef H_CRYPTID_UPDATES
#define H_CRYPTID_UPDATES

typedef enum {
  CURRENT_DISPLAY_NONE = 0,
  CURRENT_DISPLAY_TEST = 1,
  CURRENT_DISPLAY_RAINBOW = 2,
  CURRENT_DISPLAY_INT_TEMP_HUMID = 10,
  CURRENT_DISPLAY_EXT_TEMP_HUMID = 20,
} current_display_t;

/**
 * @brief Blink onboard LED and print message over serial.
 * 
 * @param milliseconds How fast to blink the LED.
 * @param message      Error message.
 */
void err(int milliseconds, String message);

/**
  vector <int> v;
 * @brief Run once on start.
 */
void setup(void);

/**
 * @brief Main loop.
 */
void loop(void);

#endif
