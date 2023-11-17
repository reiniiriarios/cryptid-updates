#include "time.h"

TimeDisplay::TimeDisplay(Gfx *gfx_p) {
  gfx = gfx_p;
}

void TimeDisplay::setTime(String yyyymmddw) {
  year    = yyyymmddw.substring(0, 4).toInt();
  month   = yyyymmddw.substring(4, 6).toInt();
  day     = yyyymmddw.substring(6, 8).toInt();
  weekday = yyyymmddw.substring(8, 9).toInt();
}

uint8_t TimeDisplay::getDay(void) {
  return day;
}

void TimeDisplay::updateScreen(void) {
  setGfxDay();
  setGfxDate();
  String dayabbr = getDayOfWeek();
  String date = getMonth() + " " + day;
  gfx->drawCircularGradientFont(dayabbr, gfx->getCenterFontX(&dayabbr), 2, &gcfg_day);
  gfx->drawCircularGradientFont(date, gfx->getCenterFontX(&date), 17, &gcfg_date);
}

String TimeDisplay::getMonth(void) {
  switch (month) {
    case 1: return "Jan";
    case 2: return "Feb";
    case 3: return "Mar";
    case 4: return "Apr";
    case 5: return "May";
    case 6: return "Jun";
    case 7: return "Jul";
    case 8: return "Aug";
    case 9: return "Sep";
    case 10: return "Oct";
    case 11: return "Nov";
    case 12: return "Dec";
  }
  return "Err";
}

String TimeDisplay::getDayOfWeek(void) {
  switch (weekday) {
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
  switch (weekday) {
    case 0:
    case 6:
      gcfg_day.setStart(0);
      gcfg_day.setEnd(360);
      return;
    case 1:
      gcfg_day.setStart(60);
      gcfg_day.setEnd(120);
      return;
    case 2:
      gcfg_day.setStart(120);
      gcfg_day.setEnd(180);
      return;
    case 3:
      gcfg_day.setStart(180);
      gcfg_day.setEnd(240);
      return;
    case 4:
      gcfg_day.setStart(240);
      gcfg_day.setEnd(300);
      return;
    case 5:
      gcfg_day.setStart(200);
      gcfg_day.setEnd(360);
      return;
  }
  gcfg_day.setStart(0);
  gcfg_day.setEnd(60);
}

void TimeDisplay::setGfxDate(void) {
  switch (month) {
    case 1:
      gcfg_date.setStart(240);
      gcfg_date.setEnd(270);
      return;
    case 2:
      gcfg_date.setStart(270);
      gcfg_date.setEnd(300);
      return;
    case 3:
      gcfg_date.setStart(300);
      gcfg_date.setEnd(330);
      return;
    case 4:
      gcfg_date.setStart(330);
      gcfg_date.setEnd(360);
      return;
    case 5:
      gcfg_date.setStart(0);
      gcfg_date.setEnd(30);
      return;
    case 6:
      gcfg_date.setStart(30);
      gcfg_date.setEnd(60);
      return;
    case 7:
      gcfg_date.setStart(60);
      gcfg_date.setEnd(90);
      return;
    case 8:
      gcfg_date.setStart(90);
      gcfg_date.setEnd(120);
      return;
    case 9:
      gcfg_date.setStart(120);
      gcfg_date.setEnd(150);
      return;
    case 10:
      gcfg_date.setStart(150);
      gcfg_date.setEnd(180);
      return;
    case 11:
      gcfg_date.setStart(180);
      gcfg_date.setEnd(210);
      return;
    case 12:
      gcfg_date.setStart(210);
      gcfg_date.setEnd(240);
      return;
  }
  gcfg_date.setStart(0);
  gcfg_date.setEnd(360);
}
