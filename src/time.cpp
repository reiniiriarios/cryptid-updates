#include "time.h"

TimeDisplay::TimeDisplay(Gfx *gfx_p) {
  gfx = gfx_p;
}

void TimeDisplay::setTime(time_t unix_ts) {
  timestamp = unix_ts;
  time = gmtime(&timestamp);
}

void TimeDisplay::updateScreen(void) {
  setGfxDay();
  setGfxDate();
  gfx->drawCircularGradientFont(getDayOfWeek(), 10, 2, &gcfg_day);
  gfx->drawCircularGradientFont(getMonth() + " " + String(time->tm_mday), 2, 17, &gcfg_date);
}

String TimeDisplay::getYear(void) {
  return String(time->tm_year - 100);
}

String TimeDisplay::getMonth(void) {
  switch (time->tm_mon) {
    case 0: return "Jan";
    case 1: return "Feb";
    case 2: return "Mar";
    case 3: return "Apr";
    case 4: return "May";
    case 5: return "Jun";
    case 6: return "Jul";
    case 7: return "Aug";
    case 8: return "Sep";
    case 9: return "Oct";
    case 10: return "Nov";
    case 11: return "Dec";
  }
  return "Err";
}

String TimeDisplay::getDayOfWeek(void) {
  switch (time->tm_wday) {
    case 0: return "Sun";
    case 1: return "Mon";
    case 2: return "Tue";
    case 3: return "Wed";
    case 4: return "Thu";
    case 5: return "Fri";
    case 6: return "Sat";
  }
  return "Err";
}

void TimeDisplay::setGfxDay(void) {
  switch (time->tm_wday) {
    case 0:
    case 6:
      gcfg_day.setStart(0);
      gcfg_day.setEnd(360);
      break;
    case 1:
      gcfg_day.setStart(60);
      gcfg_day.setEnd(120);
      break;
    case 2:
      gcfg_day.setStart(120);
      gcfg_day.setEnd(180);
      break;
    case 3:
      gcfg_day.setStart(180);
      gcfg_day.setEnd(240);
      break;
    case 4:
      gcfg_day.setStart(240);
      gcfg_day.setEnd(300);
      break;
    case 5:
      gcfg_day.setStart(200);
      gcfg_day.setEnd(360);
      break;
  }
  gcfg_day.setStart(0);
  gcfg_day.setEnd(60);
}

void TimeDisplay::setGfxDate(void) {
  switch (time->tm_mon) {
    case 0:
      gcfg_date.setStart(240);
      gcfg_date.setEnd(270);
      break;
    case 1:
      gcfg_date.setStart(270);
      gcfg_date.setEnd(300);
      break;
    case 2:
      gcfg_date.setStart(300);
      gcfg_date.setEnd(330);
      break;
    case 3:
      gcfg_date.setStart(330);
      gcfg_date.setEnd(360);
      break;
    case 4:
      gcfg_date.setStart(0);
      gcfg_date.setEnd(30);
      break;
    case 5:
      gcfg_date.setStart(30);
      gcfg_date.setEnd(60);
      break;
    case 6:
      gcfg_date.setStart(60);
      gcfg_date.setEnd(90);
      break;
    case 7:
      gcfg_date.setStart(90);
      gcfg_date.setEnd(120);
      break;
    case 8:
      gcfg_date.setStart(120);
      gcfg_date.setEnd(150);
      break;
    case 9:
      gcfg_date.setStart(150);
      gcfg_date.setEnd(180);
      break;
    case 10:
      gcfg_date.setStart(180);
      gcfg_date.setEnd(210);
      break;
    case 11:
      gcfg_date.setStart(210);
      gcfg_date.setEnd(240);
      break;
  }
}
