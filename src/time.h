#ifndef CRYPTID_TIME_H
#define CRYPTID_TIME_H

#include <time.h>

#include "gfx.h"

class TimeDisplay {
  public:
    /**
     * @brief Construct a new time object
     * 
     * @param gfx_p A pointer to the graphics object.
     */
    TimeDisplay(Gfx *gfx_p);

    /**
     * @brief Set the current time.
     * 
     * @param yyyymmddw %Y%m%d%w
     */
    void setTime(String yyyymmddw);

    /**
     * @brief Get the current day.
     *
     * @return 1-31, 0 if unset
     */
    uint8_t getDay(void);

    /**
     * @brief Update the screen.
     */
    void updateScreen(void);

    /**
     * @brief Get the current year.
     *
     * @return String 00-99
     */
    String getYear(void);

    /**
     * @brief Get the current month.
     *
     * @return String Jan-Dec
     */
    String getMonth(void);

    /**
     * @brief Get the current day.
     *
     * @return String Sun-Sat
     */
    String getDayOfWeek(void);

    /**
     * @brief Last updated millis().
     */
    uint32_t last_updated;
  
  private:
    /**
     * @brief Year.
     */
    uint16_t year;

    /**
     * @brief Month.
     */
    uint8_t month;

    /**
     * @brief Day.
     */
    uint8_t day;

    /**
     * @brief Day of week.
     */
    uint8_t weekday;

    /**
     * @brief A pointer to the graphics object.
     */
    Gfx *gfx;

    /**
     * @brief Config for day display.
     */
    gradient_config_t gcfg_day = gradient_config_t(10, 32, 16, 260, 350, false);

    /**
     * @brief Config for date display.
     */
    gradient_config_t gcfg_date = gradient_config_t(10, 32, 16, 260, 350, false);

    /**
     * @brief Set the gfx differently depending on the day of the week.
     */
    void setGfxDay(void);

    /**
     * @brief Set the gfx differently depending on the month.
     */
    void setGfxDate(void);
};

#endif
