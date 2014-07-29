//
//  LunarCalendarWrapper.m
//  SkyOne
//
//  Created by yhtian on 13-12-6.
//  No copyright.
//

#import "LunarCalendarWrapper.h"
#include "LunarCalendar.h"

@implementation LunarCalendarWrapper {
    LunarCalendar *_calendar;
    NSString *_lunarDate;
    NSString *_eightWord;
    NSString *_zodiac;
    NSString *_solarTerm;
    NSString *_festival;
}

+ (instancetype)lunar {
    return [self lunarWithDate:[NSDate date]];
}

+ (instancetype)lunarWithDate:(NSDate *)date {
    return [[self alloc] initWithDate:date];
}

- (id)init {
    return [self initWithDate:[NSDate date]];
}

- (id)initWithDate:(NSDate *)date {
    self = [super init];
    if (self) {
        self.date = date;
    }
    return self;
}

- (void)dealloc {
    if (_calendar) {
        LCReleaseLunarCalendar(_calendar);
        _calendar = NULL;
    }
}

- (NSString *)description {
    NSString *d = [super description];
    NSMutableString *desc = [NSMutableString stringWithFormat:@"<%@", [d substringWithRange:NSMakeRange(1, d.length - 2)]];
    if (self.lunarDate) {
        [desc appendFormat:@", %@", self.lunarDate];
    }
    if (self.eightWord) {
        [desc appendFormat:@", %@", self.eightWord];
    }
    if (self.zodiac) {
        [desc appendFormat:@", %@", self.zodiac];
    }
    if ([self.solarTerm length]) {
        [desc appendFormat:@", %@", self.solarTerm];
    }
    if ([self.festival length]) {
        [desc appendFormat:@", %@", self.festival];
    }
    [desc appendString:@">"];
    return desc;
}

- (NSString *)lunarDate {
    if (!_lunarDate) {
        const char *lunarDate = _calendar->lunarDate(_calendar);
        if (lunarDate) {
            _lunarDate = [[NSString alloc] initWithUTF8String:lunarDate];
        }
    }
    return _lunarDate;
}

- (NSString *)eightWord {
    if (!_eightWord) {
        const char *eightWord = _calendar->eightWord(_calendar);
        if (eightWord) {
            _eightWord = [[NSString alloc] initWithUTF8String:eightWord];
        }
    }
    return _eightWord;
}

- (NSString *)zodiac {
    if (!_zodiac) {
        const char *zodiac = _calendar->zodiac(_calendar);
        if (zodiac) {
            _zodiac = [[NSString alloc] initWithUTF8String:zodiac];
        }
    }
    return _zodiac;
}

- (NSString *)solarTerm {
    if (!_solarTerm) {
        const char *solarTerm = _calendar->solarTerm(_calendar);
        if (solarTerm) {
            _solarTerm = [[NSString alloc] initWithUTF8String:solarTerm];
        }
    }
    return _solarTerm;
}

- (NSString *)festival {
    if (!_festival) {
        const char *festival = _calendar->festival(_calendar);
        if (festival) {
            _festival = [[NSString alloc] initWithUTF8String:festival];
        }
    }
    return _festival;
}

- (void)setDate:(NSDate *)date {
    if (![date isEqualToDate:_date]) {
        @autoreleasepool {
            _date = date;
            _lunarDate = nil;
            _eightWord = nil;
            _zodiac = nil;
            _solarTerm = nil;
            _festival = nil;
            if (_calendar != NULL) {
                LCReleaseLunarCalendar(_calendar);
            }
            NSInteger componentsUnit = NSYearCalendarUnit | NSMonthCalendarUnit | NSDayCalendarUnit | NSHourCalendarUnit;
            NSDateComponents *components = [[NSCalendar currentCalendar] components:componentsUnit fromDate:date];
            struct tm time = { 0 };
            time.tm_year = (int)components.year - 1900;
            time.tm_mon = (int)components.month;
            time.tm_mday = (int)components.day;
            time.tm_hour = (int)components.hour;
            _calendar = LCCreateLunarCalendarWithTime(time);
            if (_calendar == NULL) {
                perror("Insufficient memory!\n");
            }
        }
    }
}

@end