//
//  LunarDate.h
//  LunarCalendar
//
//  Created by Tian Youhui on 13-1-22.
//  No copyright.
//

#ifndef LunarCalendar_LunarDate_h
#define LunarCalendar_LunarDate_h

typedef enum {
  kLunarCalendarNone,
  kLunarCalendarInterFestival = 1, // 国际节日
  kLunarCalendarTradFestival = 1 << 1, //传统节日
  kLunarCalendarSolarTerm = 1 << 2, //是否节气
  kLunarCalendarIsLeap = 1 << 3, // 是否闰月
  kLunarCalendarMax = 1 << 5,
} LunarCalendarUnit;

typedef struct _lunar_calendar {
  unsigned int month : 4; // 正二三四五六七八九十冬腊
  unsigned int date : 6; // 初一初二初三初四初五初六初七初八初九初十十一十二十三十四十五十六十七十八十九二十廿一廿二廿三廿四廿五廿六廿七廿八廿九三十
  unsigned int solar_term : 6; // 小寒大寒立春雨水惊蛰春分清明谷雨立夏小满芒种夏至小暑大暑立秋处暑白露秋分寒露霜降立冬小雪大雪冬至
  unsigned int festival_international : 4; // 元旦妇女植树劳动青年儿童建党建军教师国庆
  unsigned int festival_traditional : 4; // 春节元宵清明端午七夕中元中秋重阳腊八除夕
  unsigned int year8_stem : 4; // 甲乙丙丁戊己庚辛壬癸
  unsigned int year8_branch : 4; // 子丑寅卯辰巳午未申酉戌亥
  unsigned int month8_stem : 4; // 甲乙丙丁戊己庚辛壬癸
  unsigned int month8_branch : 4; // 子丑寅卯辰巳午未申酉戌亥
  unsigned int date8_stem : 4; // 甲乙丙丁戊己庚辛壬癸
  unsigned int date8_branch : 4; // 子丑寅卯辰巳午未申酉戌亥
  unsigned int hour8_stem : 4; // 甲乙丙丁戊己庚辛壬癸
  unsigned int hour8_branch : 4; // 子丑寅卯辰巳午未申酉戌亥
  unsigned int zodiac : 4; // 鼠牛虎兔龙蛇马羊猴鸡狗猪
  unsigned int flag : 4; // LunarCalendarUnit
} lunar_calendar_t;

typedef struct _solar_calendar {
  unsigned int year : 12;
  unsigned int month : 4;
  unsigned int day : 6;
  unsigned int hour : 6;
  unsigned int reserved : 4;
} solar_calendar_t;

int lunar_calendar(lunar_calendar_t *lunar_date, solar_calendar_t solar_date);

#endif
