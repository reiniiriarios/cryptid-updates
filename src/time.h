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
     * @param unix_ts
     */
    void setTime(time_t unix_ts);

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
     * @brief Get the timestamp.
     * 
     * @return time_t
     */
    time_t getTimestamp(void);
  
  private:
    /**
     * @brief The current time. Must be set.
     */
    time_t timestamp;

    /**
     * @brief The current time info.
     */
    tm *time;

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
