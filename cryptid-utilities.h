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
 * @param callback 
 * @param tries 
 * @param wait 
 * @return true 
 * @return false 
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
 * @return int 
 */
int freeMemory();

#endif
