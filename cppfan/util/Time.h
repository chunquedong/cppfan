/*
 * Copyright (c) 2012-2016, chunquedong
 *
 * This file is part of cppfan project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#ifndef TimeUtil_hpp
#define TimeUtil_hpp

#include "cppfan/base/common.h"
#include <string>

CF_BEGIN_NAMESPACE

typedef int64_t NanosTime;
typedef int64_t MillisTime;

namespace TimeUtil {
    const int64_t NanoPerSecond = 1000000000L;
    
    /**
     * return the milliseconds from midnight, January 1, 1970 UTC.
     */
    MillisTime currentTimeMillis();
    
    /**
     * returns a relative time.
     * Typically it is the number of nanosecond ticks which have elapsed since system startup.
     */
    NanosTime nanoTicks();
    
    inline MillisTime millisTicks() { return nanoTicks()/1000000L; }
}

/**
* RelativeTime
*/
class Duration {
    int64_t ticks;
public:
    Duration() : ticks(0) {}
    Duration(int64_t nanoTicks) : ticks(nanoTicks) {}
    
    int64_t nanos() { return ticks; }
    int64_t millis() { return ticks / 1000000L; }
    int64_t seconds() { return ticks / 1000000000L; }
    int64_t minutes() { return seconds()/60; }
    int64_t hour() { return minutes()/60; }
    int64_t day() { return hour()/24; }
    
    static Duration fromNanos(int64_t t) { return Duration(t); }
    static Duration fromMillis(int64_t t) { return Duration(t*1000000L); }
    static Duration fromSeconds(int64_t t) { return Duration(t*1000000000L); }
    static Duration fromMinutes(int64_t t) { return Duration(t*1000000000L*60); }
    static Duration fromHour(int64_t t) { return Duration(t*1000000000L*60*60); }
    static Duration fromDay(int64_t t) { return Duration(t*1000000000L*60*60*24); }
    static Duration now() {
        return Duration(TimeUtil::nanoTicks());
    }
    
    Duration operator+(Duration &t) { return Duration(ticks+t.ticks); }
    Duration operator-(Duration &t) { return Duration(ticks-t.ticks); }
    
    bool operator== (const Duration &t) const { return ticks == t.ticks; }
    bool operator!= (const Duration &t) const { return ticks != t.ticks; }
    bool operator< (const Duration &t) const { return ticks < t.ticks; }
    bool operator> (const Duration &t) const { return ticks > t.ticks; }
    bool operator<= (const Duration &t) const { return ticks <= t.ticks; }
    bool operator>= (const Duration &t) const { return ticks >= t.ticks; }
};

/**
 * Absolute date and time
 */
class DateTime {
    int64_t sinceEpoch;//millisecond since 1970
public:
    DateTime() : sinceEpoch(TimeUtil::currentTimeMillis()) {}
    DateTime(int64_t millisecond) : sinceEpoch(millisecond) {}
    DateTime(tm *t);
    
    std::string format(const char *format=NULL);
    void components(tm *t);
    
    int64_t millis() { return sinceEpoch; }
    time_t toUnix() { return sinceEpoch/1000; }
    
    static DateTime fromUnix(time_t t) { return DateTime(t * 1000L); }
    
    Duration operator+(DateTime &t) {
        int64_t millis = this->millis() + t.millis();
        return Duration::fromMillis(millis);
    }
    Duration operator-(DateTime &t) {
        int64_t millis = this->millis() - t.millis();
        return Duration::fromMillis(millis);
    }
    bool operator== (const DateTime &t) const { return sinceEpoch == t.sinceEpoch; }
    bool operator!= (const DateTime &t) const { return sinceEpoch != t.sinceEpoch; }
    bool operator< (const DateTime &t) const { return sinceEpoch < t.sinceEpoch; }
    bool operator> (const DateTime &t) const { return sinceEpoch > t.sinceEpoch; }
    bool operator<= (const DateTime &t) const { return sinceEpoch <= t.sinceEpoch; }
    bool operator>= (const DateTime &t) const { return sinceEpoch >= t.sinceEpoch; }
};


CF_END_NAMESPACE

#endif /* TimeUtil_hpp */
