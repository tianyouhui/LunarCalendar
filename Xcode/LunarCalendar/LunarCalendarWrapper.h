//
//  LunarCalendarWrapper.h
//  LunarCalendar
//
//  Created by yhtian on 13-12-6.
//  No copyright.
//

#import <Foundation/Foundation.h>

@interface LunarCalendarWrapper : NSObject

@property(nonatomic, strong) NSDate *date;

@property(nonatomic, readonly) NSString *lunarDate; // 农历如[闰]六月初五,七月十五,八月廿五
@property(nonatomic, readonly) NSString *eightWord; // 八字如甲子年乙丑月丙寅日丁卯时
@property(nonatomic, readonly) NSString *zodiac; // 十二生肖
@property(nonatomic, readonly) NSString *solarTerm; // 二十四节气
@property(nonatomic, readonly) NSString *festival; // (公共|传统)节日

+ (instancetype)lunar; // today
+ (instancetype)lunarWithDate:(NSDate *)date;

- (id)initWithDate:(NSDate *)date;

@end
