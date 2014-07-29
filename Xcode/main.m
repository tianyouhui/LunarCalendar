//
//  main.m
//  LunarCalendar
//
//  Created by yhtian on 14-7-2.
//  Copyright (c) 2014年 Tian Youhui. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "LunarCalendarWrapper.h"

int main(int argc, const char * argv[])
{
    @autoreleasepool {
        LunarCalendarWrapper *wrapper = [LunarCalendarWrapper lunar];
        NSLog(@"%@", wrapper);
    }
    return 0;
}

