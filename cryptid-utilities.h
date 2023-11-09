#ifndef H_CRYPTID_UTILITIES
#define H_CRYPTID_UTILITIES

#include <Arduino.h>

/**
 * @brief Convert celsius to fahrenheit.
 * 
 * @param celsius 
 * @return float 
 */
float celsius2fahrenheit(float celsius);
  
/**
 * @brief Attempt callback function n tries with delays.
 * 
 * @param callback  Function to try.
 * @param tries     Number of tries before failure.
 * @param wait      Delay in ms between tries.
 * @return bool     success
 */
template<typename T> bool attempt(T&& callback, int tries = 10, int wait = 500) {
  for (int i = 0; i < tries; i++) {
    if (callback()) {
      return true;
    }
    delay(wait);
  }
  return false;
}

/**
 * @brief Return current free memory.
 * 
 * @return int bytes
 */
int freeMemory();

#endif
