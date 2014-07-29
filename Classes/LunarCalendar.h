//
//  LunarCalendar.h
//  LunarCalendar
//
//  Created by yhtian on 14-1-28.
//  No copyright.
//

#ifndef LunarCalendar_LunarCalendar_h
#define LunarCalendar_LunarCalendar_h

#include <time.h>

#ifndef LCEXTERN
#ifndef __GNUC__
#ifdef __cplusplus
#define LCEXTERN  extern "C" __declspec(dllexport)
#else
#define LCEXTERN  extern __declspec(dllexport)
#endif
#else
#ifdef __cplusplus
#define LCEXTERN  extern "C" __attribute((visibility("default")))
#else
#define LCEXTERN  extern __attribute((visibility("default")))
#endif
#endif
#endif

typedef struct _LunarCalendar LunarCalendar;

struct _LunarCalendar {
	const char * (*lunarDate)(LunarCalendar *);
	const char * (*eightWord)(LunarCalendar *);
	const char * (*zodiac)(LunarCalendar *);
	const char * (*solarTerm)(LunarCalendar *);
	const char * (*festival)(LunarCalendar *);
};

LCEXTERN LunarCalendar *LCCreateLunarCalendar(void);
LCEXTERN LunarCalendar *LCCreateLunarCalendarWithTime(struct tm);
LCEXTERN void LCReleaseLunarCalendar(LunarCalendar *);

LCEXTERN const char *LCLunarDate(LunarCalendar *);
LCEXTERN const char *LCEightWord(LunarCalendar *);
LCEXTERN const char *LCZodiac(LunarCalendar *);
LCEXTERN const char *LCSolarTerm(LunarCalendar *);
LCEXTERN const char *LCFestival(LunarCalendar *);

#endif
