//
//  LunarDate.c
//  LunarCalendar
//
//  Created by Tian Youhui on 13-1-22.
//  Copyright (c) 2013年 Tian Youhui. All rights reserved.
//

#include "LunarDate.h"

#define dimof(_a) (sizeof(_a) / sizeof(_a[0]))

//长度为28的字符串，其定义如下：
//前12个字节代表1-12月：1为大月，0为小月；压缩成十六进制(1-3位)
//第13位为闰月的情况，1为大月30天，0为小月29天；(4位)
//第16-20位为闰月的月份，如果不是闰月为0，否则给出月份(5位)
//最后8位为当年农历新年的公历日期，如0131代表1月31日；当作数值转十六进制(6-7位)
//农历常量(1899~2100，共202年）
//数据来源：阿勇 2005/1/12

typedef struct _lunar_data {
    unsigned int new_year:8;
    unsigned int leap_mon:4;
    unsigned int leap_day:4;
    unsigned int m_1_12:12;
    unsigned int reserved:4;
} lunar_data_t;

static const unsigned int LunarCalendarTable[200] = {
    0x4AE00DB,0xA5700D0,0x54D0581,0xD2600D8,0xD9500CC,0x655147D,0x56A00D5,0x9AD00CA,0x55D027A,0x4AE00D2,
    0xA5B0682,0xA4D00DA,0xD2500CE,0xD25157E,0xB5500D6,0x56A00CC,0xADA027B,0x95B00D3,0x49717C9,0x49B00DC,
    0xA4B00D0,0xB4B0580,0x6A500D8,0x6D400CD,0xAB5147C,0x2B600D5,0x95700CA,0x52F027B,0x49700D2,0x6560682,
    0xD4A00D9,0xEA500CE,0x6A9157E,0x5AD00D6,0x2B600CC,0x86E137C,0x92E00D3,0xC8D1783,0xC9500DB,0xD4A00D0,
    0xD8A167F,0xB5500D7,0x56A00CD,0xA5B147D,0x25D00D5,0x92D00CA,0xD2B027A,0xA9500D2,0xB550781,0x6CA00D9,
    0xB5500CE,0x535157F,0x4DA00D6,0xA5B00CB,0x457037C,0x52B00D4,0xA9A0883,0xE9500DA,0x6AA00D0,0xAEA0680,
    0xAB500D7,0x4B600CD,0xAAE047D,0xA5700D5,0x52600CA,0xF260379,0xD9500D1,0x5B50782,0x56A00D9,0x96D00CE,
    0x4DD057F,0x4AD00D7,0xA4D00CB,0xD4D047B,0xD2500D3,0xD550883,0xB5400DA,0xB6A00CF,0x95A1680,0x95B00D8,
    0x49B00CD,0xA97047D,0xA4B00D5,0xB270ACA,0x6A500DC,0x6D400D1,0xAF40681,0xAB600D9,0x93700CE,0x4AF057F,
    0x49700D7,0x64B00CC,0x74A037B,0xEA500D2,0x6B50883,0x5AC00DB,0xAB600CF,0x96D0580,0x92E00D8,0xC9600CD,
    0xD95047C,0xD4A00D4,0xDA500C9,0x755027A,0x56A00D1,0xABB0781,0x25D00DA,0x92D00CF,0xCAB057E,0xA9500D6,
    0xB4A00CB,0xBAA047B,0xB5500D2,0x55D0983,0x4BA00DB,0xA5B00D0,0x5171680,0x52B00D8,0xA9300CD,0x795047D,
    0x6AA00D4,0xAD500C9,0x5B5027A,0x4B600D2,0x96E0681,0xA4E00D9,0xD2600CE,0xEA6057E,0xD5300D5,0x5AA00CB,
    0x76A037B,0x96D00D3,0x4AB0B83,0x4AD00DB,0xA4D00D0,0xD0B1680,0xD2500D7,0xD5200CC,0xDD4057C,0xB5A00D4,
    0x56D00C9,0x55B027A,0x49B00D2,0xA570782,0xA4B00D9,0xAA500CE,0xB25157E,0x6D200D6,0xADA00CA,0x4B6137B,
    0x93700D3,0x49F08C9,0x49700DB,0x64B00D0,0x68A1680,0xEA500D7,0x6AA00CC,0xA6C147C,0xAAE00D4,0x92E00CA,
    0xD2E0379,0xC9600D1,0xD550781,0xD4A00D9,0xDA400CD,0x5D5057E,0x56A00D6,0xA6C00CB,0x55D047B,0x52D00D3,
    0xA9B0883,0xA9500DB,0xB4A00CF,0xB6A067F,0xAD500D7,0x55A00CD,0xABA047C,0xA5A00D4,0x52B00CA,0xB27037A,
    0x69300D1,0x7330781,0x6AA00D9,0xAD500CE,0x4B5157E,0x4B600D6,0xA5700CB,0x54E047C,0xD1600D2,0xE960882,
    0xD5200DA,0xDAA00CF,0x6AA167F,0x56D00D7,0x4AE00CD,0xA9D047D,0xA2D00D4,0xD1500C9,0xF250279,0xD5200D1,
};

static const double SolarTermInfoTable[] = {
    0.00,         21208.24,     42467.01,     63835.73,     85337.11,     107014.43,
    128866.98,    150921.21,    173149.30,    195551.09,    218071.54,    240693.20,
    263342.68,    285989.12,    308562.77,    331033.37,    353350.09,    375494.33,
    397447.42,    419210.10,    440794.79,    462224.19,    483532.02,    504757.96,
};

static const int ChineseFestivalTable[] = {
    0x0101,0x0308,0x030C,0x0501,0x0504,0x0601,0x0701,0x0801,0x090A,0x0A01,
    0x1101,0x110F,0x1000,0x1505,0x1707,0x170F,0x180F,0x1909,0x1C08,0x1C1E,
};

void day_to_date(int days, solar_calendar_t *date)
{
    int a, b, c, d, e;
    int m, dd, mm, yy;
    a=days+2472632,b=4*a+3,b/=146097,c=-b*146097,c/=4,c+=a;
    d=4*c+3,d/=1461,e=-1461*d,e/=4,e+=c,m=5*e+2,m/=153,dd=153*m+2,dd/=5;
    dd=-dd+e+1,mm=-m/10,mm*=12,mm+=m+3,yy=b*100+d-4800+m/10;
    date->year = yy;
    date->month = mm;
    date->day = dd;
}

int date_to_day(solar_calendar_t date)
{
    int yy, mm, dd, y, m, z;
    int days;
    yy = date.year;
    mm = date.month;
    dd = date.day;
    z=14-mm,z/=12,y=yy+4800-z,m=mm+12*z-3,days=153*m+2;
    days=days/5+dd+y*365+y/4-y/100+y/400-2472633;
    return days;
}

int days_from_solar_term(int year, double mins)
{
    double days;
    int d;
    static const double x_1970_1_6_2_2 = 439320000.0;//milliseconds from 1970-01-01
    days = (x_1970_1_6_2_2 + 31556925974.7 * (year - 1970) + mins * 60000) / 86400000.0;
    d = (int)days;
    if (days - d >= ((24 * 60.0 - 15.0) / (24 * 60.0))) {//仍有误差。
        d += 1;
    }
    return d;
}

int days_from_spring(solar_calendar_t date)
{
    return date_to_day(date) - days_from_solar_term(date.year, SolarTermInfoTable[2]);
}

int solar_term_month_index(solar_calendar_t date)
{
    int index = (date.month - 1) << 1;
    long days = days_from_solar_term(date.year, SolarTermInfoTable[index]);
    if (date_to_day(date) - days < 0)
        index -= 2;
    return index;
}

int solar_term_with_date(solar_calendar_t *date)
{
    solar_calendar_t day;
    int i = (date->month - 1) << 1;
    if (date->day > 15) // half month past.
        ++i;
    day_to_date(days_from_solar_term(date->year, SolarTermInfoTable[i]), &day);
    if (date->year == day.year &&
        date->month == day.month &&
        date->day == day.day) {
        return i; // index of solar term.
    }
    return -1;
}

solar_calendar_t get_lunar_date(int days, lunar_data_t data, int year)
{
    int i, count = 0, leap;
    int sum = 0, leaped = 0, reverse = 0;
    solar_calendar_t lunar_date;
    static const int lunar_month_days = 29;
    static const int months_of_year = 12;
    days = days < 0 ? (reverse = 1, -days) : days;
    if (reverse) {
        for (i = 1; i < months_of_year + 1; i++) {
            count = lunar_month_days + ((data.m_1_12 >> (i - 1)) & 1);
            if ((days - sum) > count) {
                sum += count;
            } else {
                leaped = 0;
                break;
            }
            if ((int)data.leap_mon == i) {
                leap = lunar_month_days + data.leap_day;
                if ((days - sum) > leap) {
                    sum += leap;
                } else {
                    leaped = 1;
                    break;
                }
            }
        }
    } else {
        for (i = 1; i < months_of_year + 1; i++) {
            count = lunar_month_days + ((data.m_1_12 >> (months_of_year - i)) & 1);
            if ((days - sum) >= count) {
                sum += count;
            } else {
                leaped = 0;
                break;
            }
            if ((int)data.leap_mon == i) {
                leap = lunar_month_days + data.leap_day;
                if ((days - sum) >= leap) {
                    sum += leap;
                } else {
                    leaped = 1;
                    break;
                }
            }
        }
    }
    lunar_date.year = year;
    lunar_date.month = reverse ? (months_of_year + 1 - i) : i;
    lunar_date.day = (reverse ? (count - (days - sum)) : (days - sum)) + 1;
    leaped &= (data.leap_mon == lunar_date.month);
    lunar_date.reserved = (leaped << 1) | !(data.m_1_12 & 1);
    return lunar_date;
}

int days_of_lunar_8(solar_calendar_t date)
{
    int g, z;
    int c, m, y, d, p;
    y = date.year;
    m = date.month;
    d = date.day;
    if (m < 3) {
        y --;
        m += 12;
    }
    c = y / 100;
    y %= 100;
    p = 4 * c + c / 4 + 5 * y + y / 4 + 3 * (m + 1) / 5 + d;
    g = (p - 3) % 10;
    z = (4 * c + p + 7 + (((m % 2) == 1) ? 0 : 6)) % 12;
    return (g << 4) + z;
}

void calculate_lunar_date(lunar_calendar_t *lunar_calendar, solar_calendar_t lunar_date)
{
    lunar_calendar->month = lunar_date.month;
    lunar_calendar->date = lunar_date.day;
    lunar_calendar->flag |= ((lunar_date.reserved >> 1) ? kLunarCalendarIsLeap : 0);
}

void calculate_lunar_8word(lunar_calendar_t *lunar_calendar, solar_calendar_t lunar_date)
{
    int stem, branch, days;
    stem = (lunar_date.year - 4) % 10;
    days = (stem % 5 + 1) * 2 % 10;
    branch = (lunar_date.year - 4) % 12;
    if (days_from_spring(lunar_date) < 0) {
        --stem;
        --branch;
        stem = (10 + stem) % 10;
        branch = (12 + branch) % 12;
    }
    lunar_calendar->year8_stem = stem;
    lunar_calendar->year8_branch = branch;
    lunar_calendar->zodiac = branch;
    
    branch = (solar_term_month_index(lunar_date) - 2) >> 1;
    stem = (10 + days + branch) % 10;
    branch = (branch + 2) % 12;
    lunar_calendar->month8_stem = stem;
    lunar_calendar->month8_branch = branch;
    
    days = days_of_lunar_8(lunar_date);
    stem = ((days >> 4) + 9) % 10;
    branch = ((days & 0x0F) + 11) % 12;
    lunar_calendar->date8_stem = stem;
    lunar_calendar->date8_branch = branch;
    
    branch = (lunar_date.hour + 1) / 2 % 12;
    stem = (stem % 5 * 2 + branch) % 10;
    lunar_calendar->hour8_stem = stem;
    lunar_calendar->hour8_branch = branch;
}

void calculate_lunar_solarterm(lunar_calendar_t *lunar_calendar, solar_calendar_t lunar_date)
{
    int index = solar_term_with_date(&lunar_date);
    if (index >= 0) {
        lunar_calendar->solar_term = index;
        lunar_calendar->flag |= kLunarCalendarSolarTerm;
    }
}

void calculate_inter_festival(lunar_calendar_t *lunar_calendar, solar_calendar_t solar_date)
{
    int i;
    int end = dimof(ChineseFestivalTable) / 2;
    int fest = ((solar_date.month << 8) | solar_date.day) & 0x0FFF;
    for (i = 0; i < end; ++i) {
        if (ChineseFestivalTable[i] == fest) {
            lunar_calendar->festival_international = i;
            lunar_calendar->flag |= kLunarCalendarInterFestival;
            break;
        }
    }
}

void calculate_lunar_festival(lunar_calendar_t *lunar_calendar, solar_calendar_t lunar_date)
{
    int i;
    int start = dimof(ChineseFestivalTable) / 2;
    int fest = (((lunar_date.month << 8) | lunar_date.day) | 0x1000) & 0x1FFFF;
    for (i = start; i < dimof(ChineseFestivalTable); ++i) {
        if (ChineseFestivalTable[i] == fest) {
            lunar_calendar->festival_traditional = i - start;
            lunar_calendar->flag |= kLunarCalendarTradFestival;
            break;
        }
    }
    if (lunar_date.month == 1 && (lunar_date.day == 2 || lunar_date.day == 3)) {
        lunar_calendar->festival_traditional = 0;
        lunar_calendar->flag |= kLunarCalendarTradFestival;
    } else if (lunar_date.month == 12 && lunar_date.day == 29 && (lunar_date.reserved & 1)) {
        lunar_calendar->festival_traditional = 9;
        lunar_calendar->flag |= kLunarCalendarTradFestival;
    }
}

solar_calendar_t lunar_date_from_table(solar_calendar_t date)
{
    int days;
    lunar_data_t lunar_data;
    solar_calendar_t new_year;
    int index = date.year - 1900;
    lunar_data = *(lunar_data_t *)(&LunarCalendarTable[index - 1]);
    new_year.year = date.year;
    new_year.month = lunar_data.new_year / 100;
    new_year.day = lunar_data.new_year % 100;
    days = date_to_day(date);
    days -= date_to_day(new_year);
    if (days < 0) {
        lunar_data = *(lunar_data_t *)(&LunarCalendarTable[index - 2]);
        date.year--;
    }
    return get_lunar_date(days, lunar_data, date.year);
}

int lunar_calendar(lunar_calendar_t *lunar_calendar, solar_calendar_t solar_calendar)
{
    solar_calendar_t lunar_date;
    solar_calendar.reserved = 0;
    lunar_date = lunar_date_from_table(solar_calendar);
    if (solar_calendar.year - 1900 < 2 ||
        solar_calendar.year - 1900 > dimof(LunarCalendarTable) - 1)
        return -1;
    calculate_lunar_date(lunar_calendar, lunar_date);
    calculate_lunar_8word(lunar_calendar, solar_calendar);
    calculate_lunar_solarterm(lunar_calendar, solar_calendar);
    calculate_inter_festival(lunar_calendar, solar_calendar);
    calculate_lunar_festival(lunar_calendar, lunar_date);
    return 0;
}
