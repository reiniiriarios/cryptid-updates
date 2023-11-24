#include "time.h"

TimeDisplay::TimeDisplay(Gfx *gfx_p) {
  gfx = gfx_p;
}

void TimeDisplay::setTime(String yyyymmddw) {
  year    = yyyymmddw.substring(0, 4).toInt();
  month   = yyyymmddw.substring(4, 6).toInt();
  day     = yyyymmddw.substring(6, 8).toInt();
  weekday = yyyymmddw.substring(8, 9).toInt();
  last_updated = millis();
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
      gcfg_date.setStart(230);
      gcfg_date.setEnd(280);
      return;
    case 2:
      gcfg_date.setStart(260);
      gcfg_date.setEnd(310);
      return;
    case 3:
      gcfg_date.setStart(290);
      gcfg_date.setEnd(340);
      return;
    case 4:
      gcfg_date.setStart(320);
      gcfg_date.setEnd(370);
      return;
    case 5:
      gcfg_date.setStart(360); // -10
      gcfg_date.setEnd(400); // 40
      return;
    case 6:
      gcfg_date.setStart(20);
      gcfg_date.setEnd(70);
      return;
    case 7:
      gcfg_date.setStart(50);
      gcfg_date.setEnd(100);
      return;
    case 8:
      gcfg_date.setStart(80);
      gcfg_date.setEnd(130);
      return;
    case 9:
      gcfg_date.setStart(110);
      gcfg_date.setEnd(160);
      return;
    case 10:
      gcfg_date.setStart(140);
      gcfg_date.setEnd(190);
      return;
    case 11:
      gcfg_date.setStart(170);
      gcfg_date.setEnd(220);
      return;
    case 12:
      gcfg_date.setStart(200);
      gcfg_date.setEnd(250);
      return;
  }
  gcfg_date.setStart(0);
  gcfg_date.setEnd(360);
}
