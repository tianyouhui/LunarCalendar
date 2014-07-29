#include "LunarCalendar.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "LunarDate.h"

#ifdef _MSC_VER
#define kLunarCalendarCharacterSize	2
#else
#define kLunarCalendarCharacterSize	3
#endif
static const char *const kLunarCalendarCharacter =
"正二三四五六七八九十冬腊"
"初一初二初三初四初五初六初七初八初九初十十一十二十三十四十五"
"十六十七十八十九二十廿一廿二廿三廿四廿五廿六廿七廿八廿九三十"
"小寒大寒立春雨水惊蛰春分清明谷雨立夏小满芒种夏至"
"小暑大暑立秋处暑白露秋分寒露霜降立冬小雪大雪冬至"
"元旦妇女植树劳动青年儿童建党建军教师国庆"
"春节元宵清明端午七夕中元中秋重阳腊八除夕"
"甲乙丙丁戊己庚辛壬癸"
"子丑寅卯辰巳午未申酉戌亥"
"鼠牛虎兔龙蛇马羊猴鸡狗猪"
"闰年月日时节 ";

typedef enum _LunarCalendarCharacterIndex {
    kLunarCalendarMonthIndex = 0,
    kLunarCalendarDateIndex = kLunarCalendarMonthIndex + 12 * kLunarCalendarCharacterSize, // 36
    kLunarCalendarSolarTermIndex = kLunarCalendarDateIndex + 60 * kLunarCalendarCharacterSize, // 216
    kLunarCalendarInterFestivalIndex = kLunarCalendarSolarTermIndex + 48 * kLunarCalendarCharacterSize, // 360
    kLunarCalendarTradFestivalIndex = kLunarCalendarInterFestivalIndex + 20 * kLunarCalendarCharacterSize, // 420
    kLunarCalendarStemIndex = kLunarCalendarTradFestivalIndex + 20 * kLunarCalendarCharacterSize, // 480
    kLunarCalendarBranchIndex = kLunarCalendarStemIndex + 10 * kLunarCalendarCharacterSize, // 510
    kLunarCalendarZodiacIndex = kLunarCalendarBranchIndex + 12 * kLunarCalendarCharacterSize, // 546
    kLunarCalendarLeapIndex = kLunarCalendarZodiacIndex + 12 * kLunarCalendarCharacterSize, // 582
} LunarCalendarCharacterIndex;

typedef struct _LunarCalendar_private {
	struct _LunarCalendar _lunar_calendar;
	lunar_calendar_t _lunar_date;
	solar_calendar_t _solar_date;
    int _calendar_workout;
	size_t _str_size;
	char * _characters;
} LunarCalendar_private;

static int characters(char *a, int index, int offset, int len)
{
	int i;
	for (i = 0; i < len * kLunarCalendarCharacterSize; i++) {
		a[i] = kLunarCalendarCharacter[index + offset * kLunarCalendarCharacterSize * len + i];
	}
	return i;
}

static void reset_characters(LunarCalendar_private *calendar, size_t size)
{
	if (calendar->_characters == NULL) {
		calendar->_characters = (char *)malloc(sizeof(char) * size);
	} else {
		calendar->_characters = (char *)realloc(calendar->_characters, size);
	}
	memset(calendar->_characters, 0, size);
	calendar->_str_size = 0;
}

static int calculate_lunar(LunarCalendar_private *calendar)
{
    if (calendar->_calendar_workout == 0) {
        if (lunar_calendar(&calendar->_lunar_date, calendar->_solar_date)) {
            char error[32] = { 0 };
#if /*_MSC_VER >= 1500 && */__STDC_WANT_SECURE_LIB__
			sprintf_s(error, 256, "Unknown date %d-%02d-%02d!\n",
				calendar->_solar_date.year,
				calendar->_solar_date.month,
				calendar->_solar_date.day);
#else
            sprintf(error, "Unknown date %d-%02d-%02d!\n",
                    calendar->_solar_date.year,
                    calendar->_solar_date.month,
                    calendar->_solar_date.day);
#endif
            perror(error);
            calendar->_calendar_workout = -1;
        } else {
            calendar->_calendar_workout = 1;
        }
    }
    return calendar->_calendar_workout;
}

const char *_LunarCalendar_private_lunarDate(LunarCalendar *lunar_calendar)
{
	size_t size;
 	LunarCalendar_private *calendar = (LunarCalendar_private *)lunar_calendar;
    if (calculate_lunar(calendar) != 1) {
        return NULL;
    }
	reset_characters(calendar, 5 * kLunarCalendarCharacterSize + 1);
	if ((calendar->_lunar_date.flag & kLunarCalendarIsLeap) == kLunarCalendarIsLeap) {
		size = characters(calendar->_characters, kLunarCalendarLeapIndex, 0, 1);
		calendar->_str_size += size;
	}
	size = characters(calendar->_characters + calendar->_str_size,
                      kLunarCalendarMonthIndex, calendar->_lunar_date.month - 1, 1);
	calendar->_str_size += size;
	size = characters(calendar->_characters + calendar->_str_size, kLunarCalendarLeapIndex, 2, 1);
	calendar->_str_size += size;
	size = characters(calendar->_characters + calendar->_str_size,
                      kLunarCalendarDateIndex, calendar->_lunar_date.date - 1, 2);
	calendar->_str_size += size;
	return calendar->_characters;
}

const char *_LunarCalendar_private_eightWorld(LunarCalendar *lunar_calendar)
{
	size_t size;
 	LunarCalendar_private *calendar = (LunarCalendar_private *)lunar_calendar;
    if (calculate_lunar(calendar) != 1) {
        return NULL;
    }
	reset_characters(calendar, 12 * kLunarCalendarCharacterSize + 1);
	size = characters(calendar->_characters + calendar->_str_size,
                      kLunarCalendarStemIndex, calendar->_lunar_date.year8_stem, 1);
	calendar->_str_size += size;
	size = characters(calendar->_characters + calendar->_str_size,
                      kLunarCalendarBranchIndex, calendar->_lunar_date.year8_branch, 1);
	calendar->_str_size += size;
	size = characters(calendar->_characters + calendar->_str_size, kLunarCalendarLeapIndex, 1, 1);
	calendar->_str_size += size;
	size = characters(calendar->_characters + calendar->_str_size,
                      kLunarCalendarStemIndex, calendar->_lunar_date.month8_stem, 1);
	calendar->_str_size += size;
	size = characters(calendar->_characters + calendar->_str_size,
                      kLunarCalendarBranchIndex, calendar->_lunar_date.month8_branch, 1);
	calendar->_str_size += size;
	size = characters(calendar->_characters + calendar->_str_size, kLunarCalendarLeapIndex, 2, 1);
	calendar->_str_size += size;
	size = characters(calendar->_characters + calendar->_str_size,
                      kLunarCalendarStemIndex, calendar->_lunar_date.date8_stem, 1);
	calendar->_str_size += size;
	size = characters(calendar->_characters + calendar->_str_size,
                      kLunarCalendarBranchIndex, calendar->_lunar_date.date8_branch, 1);
	calendar->_str_size += size;
	size = characters(calendar->_characters + calendar->_str_size, kLunarCalendarLeapIndex, 3, 1);
	calendar->_str_size += size;
    size = characters(calendar->_characters + calendar->_str_size,
                      kLunarCalendarStemIndex, calendar->_lunar_date.hour8_stem, 1);
    calendar->_str_size += size;
    size = characters(calendar->_characters + calendar->_str_size,
                      kLunarCalendarBranchIndex, calendar->_lunar_date.hour8_branch, 1);
    calendar->_str_size += size;
    size = characters(calendar->_characters + calendar->_str_size, kLunarCalendarLeapIndex, 4, 1);
    calendar->_str_size += size;
	return calendar->_characters;
}

const char *_LunarCalendar_private_solarTerm(LunarCalendar *lunar_calendar)
{
	size_t size;
 	LunarCalendar_private *calendar = (LunarCalendar_private *)lunar_calendar;
    if (calculate_lunar(calendar) != 1) {
        return NULL;
    }
	reset_characters(calendar, 2 * kLunarCalendarCharacterSize + 1);
	if ((calendar->_lunar_date.flag & kLunarCalendarSolarTerm) == kLunarCalendarSolarTerm) {
		size = characters(calendar->_characters + calendar->_str_size,
                          kLunarCalendarSolarTermIndex, calendar->_lunar_date.solar_term, 2);
		calendar->_str_size += size;
	}
	return calendar->_characters;
}

const char *_LunarCalendar_private_zodiac(LunarCalendar *lunar_calendar)
{
	size_t size;
 	LunarCalendar_private *calendar = (LunarCalendar_private *)lunar_calendar;
    if (calculate_lunar(calendar) != 1) {
        return NULL;
    }
	reset_characters(calendar, 1 * kLunarCalendarCharacterSize + 1);
	size = characters(calendar->_characters + calendar->_str_size,
                      kLunarCalendarZodiacIndex, calendar->_lunar_date.zodiac, 1);
	calendar->_str_size += size;
	return calendar->_characters;
}

const char *_LunarCalendar_private_festival(LunarCalendar *lunar_calendar)
{
	size_t size;
 	LunarCalendar_private *calendar = (LunarCalendar_private *)lunar_calendar;
    if (calculate_lunar(calendar) != 1) {
        return NULL;
    }
	reset_characters(calendar, 5 * kLunarCalendarCharacterSize + 2);
	if ((calendar->_lunar_date.flag & kLunarCalendarInterFestival) == kLunarCalendarInterFestival) {
		size = characters(calendar->_characters + calendar->_str_size,
                          kLunarCalendarInterFestivalIndex, calendar->_lunar_date.festival_international, 2);
		calendar->_str_size += size;
		size = characters(calendar->_characters + calendar->_str_size, kLunarCalendarLeapIndex, 5, 1);
		calendar->_str_size += size;
	}
	if ((calendar->_lunar_date.flag & kLunarCalendarInterFestival) == kLunarCalendarInterFestival &&
		(calendar->_lunar_date.flag & kLunarCalendarTradFestival) == kLunarCalendarTradFestival) {
		size = characters(calendar->_characters + calendar->_str_size, kLunarCalendarLeapIndex, 6, 1);
		calendar->_str_size += size;
	}
	if ((calendar->_lunar_date.flag & kLunarCalendarTradFestival) == kLunarCalendarTradFestival) {
		size = characters(calendar->_characters + calendar->_str_size,
                          kLunarCalendarTradFestivalIndex, calendar->_lunar_date.festival_traditional, 2);
		calendar->_str_size += size;
	}
	return calendar->_characters;
}

__inline const char *LCLunarDate(LunarCalendar *lunarCalendar)
{
	return lunarCalendar->lunarDate(lunarCalendar);
}

__inline const char *LCEightWord(LunarCalendar *lunarCalendar)
{
	return lunarCalendar->eightWord(lunarCalendar);
}

__inline const char *LCZodiac(LunarCalendar *lunarCalendar)
{
	return lunarCalendar->zodiac(lunarCalendar);
}

__inline const char *LCSolarTerm(LunarCalendar *lunarCalendar)
{
	return lunarCalendar->solarTerm(lunarCalendar);
}

__inline const char *LCFestival(LunarCalendar *lunarCalendar)
{
	return lunarCalendar->festival(lunarCalendar);
}

LunarCalendar *LCCreateLunarCalendar(void)
{
    time_t t;
#if __STDC_WANT_SECURE_LIB__
	struct tm now;
	time(&t);
	localtime_s(&now, &t);
    now.tm_mon += 1;
	return LCCreateLunarCalendarWithTime(now);
#else
	time(&t);
	struct tm *now;
    now = localtime(&t);
    now->tm_mon += 1;
	return LCCreateLunarCalendarWithTime(*now);
#endif
}

LunarCalendar *LCCreateLunarCalendarWithTime(struct tm time)
{
	LunarCalendar_private *calendar = (LunarCalendar_private *)malloc(sizeof(LunarCalendar_private));
	memset(calendar, 0, sizeof(LunarCalendar_private));
	calendar->_lunar_calendar.lunarDate = _LunarCalendar_private_lunarDate;
	calendar->_lunar_calendar.eightWord = _LunarCalendar_private_eightWorld;
	calendar->_lunar_calendar.solarTerm = _LunarCalendar_private_solarTerm;
	calendar->_lunar_calendar.zodiac    = _LunarCalendar_private_zodiac;
	calendar->_lunar_calendar.festival  = _LunarCalendar_private_festival;
    calendar->_calendar_workout = 0;
	calendar->_solar_date.year  = time.tm_year + 1900;
	calendar->_solar_date.month = time.tm_mon;
	calendar->_solar_date.day   = time.tm_mday;
	calendar->_solar_date.hour  = time.tm_hour;
	return &calendar->_lunar_calendar;
}

void LCReleaseLunarCalendar(LunarCalendar *lunar_calendar)
{
	LunarCalendar_private *calendar = (LunarCalendar_private *)lunar_calendar;
	calendar->_lunar_calendar.lunarDate = NULL;
	calendar->_lunar_calendar.eightWord = NULL;
	calendar->_lunar_calendar.solarTerm = NULL;
	calendar->_lunar_calendar.zodiac    = NULL;
	calendar->_lunar_calendar.festival  = NULL;
	calendar->_str_size = 0;
	if (calendar->_characters) {
		free(calendar->_characters);
		calendar->_characters = NULL;
	}
	free(calendar);
	calendar = NULL;
}
