#ifndef H_CRYPTID_UPDATES
#define H_CRYPTID_UPDATES

  /**
   * @brief Blink onboard LED and print message over serial.
   * 
   * @param milliseconds How fast to blink the LED.
   * @param message      Error message.
   */
  void err(int milliseconds, String message);

  /**
   * @brief Run once on start.
   */
  void setup(void);

  /**
   * @brief Main loop.
   */
  void loop(void);

  /**
   * @brief Convert celcius to fahrenheit.
   * 
   * @param celsius degrees
   * @return Degrees fahrenheit.
   */
  float celsius2fahrenheit(float celsius);

  /**
   * @brief Calculate free SRAM.
   * 
   * @return int 
   */
  int freeMemory();

#endif
