#ifndef CRYPTID_AAAHHH_H
#define CRYPTID_AAAHHH_H

#include "gfx.h"

class Aaahhh {
  public:
    /**
     * @brief Construct a new aaahhh object
     * 
     * @param gfx_p A pointer to the graphics object.
     */
    Aaahhh(Gfx *gfx_p);

    /**
     * @brief Update the screen.
     */
    void display(void);

    /**
     * @brief Set current acceleration.
     * 
     * @param x 
     * @param y 
     * @param z 
     */
    void setMovement(float x, float y, float z);

    /**
     * @brief Set current acceleration.
     * 
     * @param x 
     * @param y 
     * @param z 
     */
    void setMovement(double x, double y, double z);

    /**
     * @brief Is the display currently moving quickly.
     */
    bool isMovingFast(void);
  
  private:
    /**
     * @brief Acceleration.
     */
    double accel_x, accel_y, accel_z;

    /**
     * @brief Is the display moving quickly.
     */
    bool moving_fast = false;

    /**
     * @brief Millis since the display started moving quickly.
     */
    long started_moving_fast = 0;

    /**
     * @brief Text to display.
     */
    String displayString = "AAH!";

    /**
     * @brief A pointer to the graphics object.
     */
    Gfx *gfx;

    /**
     * @brief Gradient config.
     */
    gradient_config_t gcfg = gradient_config_t(10, 32, 16, 260, 350, false);
};

#endif
