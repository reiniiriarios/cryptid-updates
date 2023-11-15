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
  
  private:
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
