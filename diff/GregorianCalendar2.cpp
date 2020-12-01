#include <string>
#include <set>
#include <exception>
#include <iostream>
#include "boost/date_time/gregorian/gregorian.hpp"
#include "decl.h"

enum class TimeDuration { DAY, WEEK, BI_MONTH, MONTH, QUARTER, YEAR };
enum class TimePositin { BEGIN, END };
template <TimeDuration d, TimePositin p > struct TimeDurationRule
{
    static constexpr TimeDuration _duration = d;
    static constexpr TimePositin  _positin = p;
};

unsigned firstCalendarDate(TimeDurationRule<TimeDuration::WEEK, TimePositin::BEGIN > rule_, unsigned fromDate_);
unsigned firstCalendarDate(TimeDurationRule<TimeDuration::WEEK, TimePositin::END > rule_, unsigned fromDate_);
unsigned firstCalendarDate(TimeDurationRule<TimeDuration::BI_MONTH, TimePositin::BEGIN > rule_, unsigned fromDate_);
unsigned firstCalendarDate(TimeDurationRule<TimeDuration::BI_MONTH, TimePositin::END > rule_, unsigned fromDate_);
unsigned firstCalendarDate(TimeDurationRule<TimeDuration::MONTH, TimePositin::BEGIN > rule_, unsigned fromDate_);
unsigned firstCalendarDate(TimeDurationRule<TimeDuration::MONTH, TimePositin::END > rule_, unsigned fromDate_);
unsigned firstCalendarDate(TimeDurationRule<TimeDuration::QUARTER, TimePositin::BEGIN > rule_, unsigned fromDate_);
unsigned firstCalendarDate(TimeDurationRule<TimeDuration::QUARTER, TimePositin::END > rule_, unsigned fromDate_);
unsigned firstCalendarDate(TimeDurationRule<TimeDuration::YEAR, TimePositin::BEGIN > rule_, unsigned fromDate_);
unsigned firstCalendarDate(TimeDurationRule<TimeDuration::YEAR, TimePositin::END > rule_, unsigned fromDate_);

// must iterate from valid date!
unsigned nextCalendarDate(TimeDurationRule<TimeDuration::WEEK, TimePositin::BEGIN > rule_, unsigned fromDate_);
unsigned nextCalendarDate(TimeDurationRule<TimeDuration::WEEK, TimePositin::END > rule_, unsigned fromDate_);
unsigned nextCalendarDate(TimeDurationRule<TimeDuration::BI_MONTH, TimePositin::BEGIN > rule_, unsigned fromDate_);
unsigned nextCalendarDate(TimeDurationRule<TimeDuration::BI_MONTH, TimePositin::END > rule_, unsigned fromDate_);
unsigned nextCalendarDate(TimeDurationRule<TimeDuration::MONTH, TimePositin::BEGIN > rule_, unsigned fromDate_);
unsigned nextCalendarDate(TimeDurationRule<TimeDuration::MONTH, TimePositin::END > rule_, unsigned fromDate_);
unsigned nextCalendarDate(TimeDurationRule<TimeDuration::QUARTER, TimePositin::BEGIN > rule_, unsigned fromDate_);
unsigned nextCalendarDate(TimeDurationRule<TimeDuration::QUARTER, TimePositin::END > rule_, unsigned fromDate_);
unsigned nextCalendarDate(TimeDurationRule<TimeDuration::YEAR, TimePositin::BEGIN > rule_, unsigned fromDate_);
unsigned nextCalendarDate(TimeDurationRule<TimeDuration::YEAR, TimePositin::END > rule_, unsigned fromDate_);


namespace gd = boost::gregorian;

unsigned firstCalendarDate(TimeDurationRule<TimeDuration::WEEK, TimePositin::BEGIN > rule_, unsigned fromDate_)
{
    gd::date d = gd::from_undelimited_string(std::to_string(fromDate_));
    if ((d.day_of_week().as_number() == 1) ) {
        return std::stoi(to_iso_string(d));
    }

    gd::first_day_of_the_week_after fdaf(gd::Monday);
    d = fdaf.get_date(d);
    return std::stoi(to_iso_string(d));
}

unsigned nextCalendarDate(TimeDurationRule<TimeDuration::WEEK, TimePositin::BEGIN > rule_, unsigned fromDate_)
{
    gd::date d = gd::from_undelimited_string(std::to_string(fromDate_));
    d = d + gd::days(7);
    return std::stoi(to_iso_string(d));
}

unsigned firstCalendarDate(TimeDurationRule<TimeDuration::WEEK, TimePositin::END > rule_, unsigned fromDate_)
{
    gd::date d = gd::from_undelimited_string(std::to_string(fromDate_));
    if ((d.day_of_week().as_number() == 5)) {
        return std::stoi(to_iso_string(d));
    }

    gd::first_day_of_the_week_after fdaf(gd::Friday);
    d = fdaf.get_date(d);
    return std::stoi(to_iso_string(d));
}

unsigned nextCalendarDate(TimeDurationRule<TimeDuration::WEEK, TimePositin::END > rule_, unsigned fromDate_)
{
    gd::date d = gd::from_undelimited_string(std::to_string(fromDate_));
    d = d + gd::days(7);
    return std::stoi(to_iso_string(d));
}

unsigned firstCalendarDate(TimeDurationRule<TimeDuration::BI_MONTH, TimePositin::BEGIN > rule_, unsigned fromDate_)
{
    gd::date d = gd::from_undelimited_string(std::to_string(fromDate_));
    const unsigned cday = d.day().as_number();
    if (((cday == 1) || (cday == 15)) ) {
        return fromDate_;
    }

    if (cday < 15) {
        d = d + gd::days(15 - cday);
    }
    else {
        d = d.end_of_month();
        d = d + gd::days(1);
    }
    return std::stoi(to_iso_string(d));
}

unsigned nextCalendarDate(TimeDurationRule<TimeDuration::BI_MONTH, TimePositin::BEGIN > rule_, unsigned fromDate_)
{
    gd::date d = gd::from_undelimited_string(std::to_string(fromDate_));
    const unsigned cday = d.day().as_number();
    if (cday == 1) {
        d = d + gd::days(14);
    }
    else // must be 15
    {
        d = d.end_of_month();
        d = d + gd::days(1);  
    }
    return std::stoi(to_iso_string(d));
}

unsigned firstCalendarDate(TimeDurationRule<TimeDuration::BI_MONTH, TimePositin::END > rule_, unsigned fromDate_)
{
    gd::date d = gd::from_undelimited_string(std::to_string(fromDate_));
    const unsigned cday = d.day().as_number();
    const unsigned lday = d.end_of_month().day().as_number();
    if ((cday == 15) || (cday == lday)) {
        return fromDate_;
    }
    if (cday < 15) {
        d = d + gd::days(15 - cday);
    }
    else
    {
        d = d.end_of_month();
    }
   
    return std::stoi(to_iso_string(d));
}

unsigned nextCalendarDate(TimeDurationRule<TimeDuration::BI_MONTH, TimePositin::END > rule_, unsigned fromDate_)
{
    gd::date d = gd::from_undelimited_string(std::to_string(fromDate_));
    const unsigned cday = d.day().as_number();
    if (cday == 15) {
        d = d.end_of_month();
    }
    else // must be last day
    {
        d = d + gd::days(15);
    }
    return std::stoi(to_iso_string(d));
}

unsigned firstCalendarDate(TimeDurationRule<TimeDuration::MONTH, TimePositin::BEGIN > rule_, unsigned fromDate_)
{
    gd::date d = gd::from_undelimited_string(std::to_string(fromDate_));
    if (d.day().as_number() == 1) {
        return std::stoi(to_iso_string(d));
    }
    d = d.end_of_month();
    d = d + gd::days(1);
    return std::stoi(to_iso_string(d));
}

unsigned nextCalendarDate(TimeDurationRule<TimeDuration::MONTH, TimePositin::BEGIN > rule_, unsigned fromDate_)
{
    gd::date d = gd::from_undelimited_string(std::to_string(fromDate_));
    d = d.end_of_month();
    d = d + gd::days(1);
    return std::stoi(gd::to_iso_string(d));
}

unsigned firstCalendarDate(TimeDurationRule<TimeDuration::MONTH, TimePositin::END > rule_, unsigned fromDate_)
{
    gd::date d = gd::from_undelimited_string(std::to_string(fromDate_));
    d = d.end_of_month();
    return std::stoi(gd::to_iso_string(d));
}

unsigned nextCalendarDate(TimeDurationRule<TimeDuration::MONTH, TimePositin::END > rule_, unsigned fromDate_)
{
    gd::date d = gd::from_undelimited_string(std::to_string(fromDate_));
    d = d.end_of_month();
    d = d + gd::days(1);
    d = d.end_of_month();
    return std::stoi(gd::to_iso_string(d));
}
static bool isQuarterBegin(unsigned dt_)
{
    const gd::date d = gd::from_undelimited_string(std::to_string(dt_));
    const unsigned day = d.day().as_number();
    const unsigned month = d.month().as_number();
    if ((day == 1) && ((month == 1) || (month == 4) || (month == 7) || (month == 10))) {
        return true;
    }
    return false;
}

static bool isQuarterEnd(unsigned dt_)
{
    const gd::date d = gd::from_undelimited_string(std::to_string(dt_));
    const unsigned month = d.month().as_number();
    if ((d == d.end_of_month()) && ((month == 3) || (month == 6) || (month ==9) || (month == 12))) {
        return true;
    }
    return false;
}

unsigned firstCalendarDate(TimeDurationRule<TimeDuration::QUARTER, TimePositin::BEGIN > rule_, unsigned fromDate_)
{
    unsigned date = firstCalendarDate(TimeDurationRule<TimeDuration::MONTH, TimePositin::BEGIN >(), fromDate_);
    for (int i = 0; i < 4; ++i) {
        if (isQuarterBegin(date) ) {
            break;
        }
        if (i == 3) {
            throw std::runtime_error("Codding error in getting CalendarDate ");
        }
        date = nextCalendarDate(TimeDurationRule<TimeDuration::MONTH, TimePositin::BEGIN >(), date);
    }
    return date;
}

unsigned nextCalendarDate(TimeDurationRule<TimeDuration::QUARTER, TimePositin::BEGIN > rule_, unsigned fromDate_)
{
    unsigned date = nextCalendarDate(TimeDurationRule<TimeDuration::MONTH, TimePositin::BEGIN >(), fromDate_);
    for (int i = 0; i < 4; ++i) {
        if (isQuarterBegin(date)) {
            break;
        }
        if (i == 3) {
            throw std::runtime_error("Codding error in getting CalendarDate ");
        }
        date = nextCalendarDate(TimeDurationRule<TimeDuration::MONTH, TimePositin::BEGIN >(), date);
    }
    return date;
}

unsigned firstCalendarDate(TimeDurationRule<TimeDuration::QUARTER, TimePositin::END > rule_, unsigned fromDate_)
{
    unsigned date = firstCalendarDate(TimeDurationRule<TimeDuration::MONTH, TimePositin::END >(), fromDate_);
    for (int i = 0; i < 4; ++i) {
        if (isQuarterEnd(date)) {
            break;
        }
        if (i == 3) {
            throw std::runtime_error("Codding error in getting CalendarDate ");
        }
        date = nextCalendarDate(TimeDurationRule<TimeDuration::MONTH, TimePositin::END >(), date);
    }
    return date;
}

unsigned nextCalendarDate(TimeDurationRule<TimeDuration::QUARTER, TimePositin::END > rule_, unsigned fromDate_)
{
    unsigned date = nextCalendarDate(TimeDurationRule<TimeDuration::MONTH, TimePositin::END >(), fromDate_);
    for (int i = 0; i < 4; ++i) {
        if (isQuarterEnd(date)) {
            break;
        }
        if (i == 3) {
            throw std::runtime_error("Codding error in getting CalendarDate ");
        }
        date = nextCalendarDate(TimeDurationRule<TimeDuration::MONTH, TimePositin::END >(), date);
    }
    return date;
}


unsigned firstCalendarDate(TimeDurationRule<TimeDuration::YEAR, TimePositin::BEGIN > rule_, unsigned fromDate_)
{
    gd::date d = gd::from_undelimited_string(std::to_string(fromDate_));
    if ((d.day().as_number() == 1) && ((d.month().as_number() == 1))) {// first day of the year
        return std::stoi(to_iso_string(d));
    }
    const gd::greg_year thisYear = d.year();
    const gd::date thisYearBegin(thisYear, gd::Jan, 1);
    const gd::date nextYearBegin = thisYearBegin + gd::years(1);
    return std::stoi(gd::to_iso_string(nextYearBegin));
}

unsigned nextCalendarDate(TimeDurationRule<TimeDuration::YEAR, TimePositin::BEGIN > rule_, unsigned fromDate_)
{
    gd::date d = gd::from_undelimited_string(std::to_string(fromDate_));
    return std::stoi(to_iso_string(d + gd::years(1)));
}

unsigned firstCalendarDate(TimeDurationRule<TimeDuration::YEAR, TimePositin::END > rule_, unsigned fromDate_)
{
    gd::date d = gd::from_undelimited_string(std::to_string(fromDate_));
    const gd::greg_year thisYear = d.year();
    const gd::date newD(thisYear, gd::Dec, 31);
    return std::stoi(gd::to_iso_string(newD));
}

unsigned nextCalendarDate(TimeDurationRule<TimeDuration::YEAR, TimePositin::END > rule_, unsigned fromDate_)
{
    gd::date d = gd::from_undelimited_string(std::to_string(fromDate_));
    const gd::greg_year thisYear = d.year();
    const gd::date thisYearEnd(thisYear, gd::Dec, 31);
    const gd::date nextYearEnd = thisYearEnd + gd::years(1);
    return std::stoi(gd::to_iso_string(nextYearEnd));
}





int main()
{
    foo<int>();
    /*
    unsigned fwb = firstCalendarDate(TimeDurationRule<TimeDuration::WEEK, TimePositin::BEGIN >(), 20200420);
    unsigned fwb2 = firstCalendarDate(TimeDurationRule<TimeDuration::WEEK, TimePositin::BEGIN >(), 20200421);
    unsigned nwb = nextCalendarDate(TimeDurationRule<TimeDuration::WEEK, TimePositin::BEGIN >(), fwb2);

    unsigned fwe = firstCalendarDate(TimeDurationRule<TimeDuration::WEEK, TimePositin::END >(), 20200425);
    unsigned fwe2 = firstCalendarDate(TimeDurationRule<TimeDuration::WEEK, TimePositin::END >(), 20200426);
    unsigned nwe = nextCalendarDate(TimeDurationRule<TimeDuration::WEEK, TimePositin::END>(), fwe2);
  

    unsigned fwbmb = firstCalendarDate(TimeDurationRule<TimeDuration::BI_MONTH, TimePositin::BEGIN >(), 20200420);
    unsigned fbmb2 = firstCalendarDate(TimeDurationRule<TimeDuration::BI_MONTH, TimePositin::BEGIN >(), 20200421);
    unsigned nbnb = nextCalendarDate(TimeDurationRule<TimeDuration::BI_MONTH, TimePositin::BEGIN >(), fbmb2);

    unsigned fbme = firstCalendarDate(TimeDurationRule<TimeDuration::BI_MONTH, TimePositin::END >(), 20200425);
    unsigned fbme2 = firstCalendarDate(TimeDurationRule<TimeDuration::BI_MONTH, TimePositin::END >(), 20200426);
    unsigned nbme = nextCalendarDate(TimeDurationRule<TimeDuration::BI_MONTH, TimePositin::END>(), fbme2);
   

    unsigned fwmb = firstCalendarDate(TimeDurationRule<TimeDuration::MONTH, TimePositin::BEGIN >(), 20200420);
    unsigned fmb2 = firstCalendarDate(TimeDurationRule<TimeDuration::MONTH, TimePositin::BEGIN >(), 20200421);
    unsigned nmb = nextCalendarDate(TimeDurationRule<TimeDuration::MONTH, TimePositin::BEGIN >(), fmb2);

    unsigned fme = firstCalendarDate(TimeDurationRule<TimeDuration::MONTH, TimePositin::END >(), 20200425);
    unsigned fme2 = firstCalendarDate(TimeDurationRule<TimeDuration::MONTH, TimePositin::END >(), 20200426);
    unsigned nme = nextCalendarDate(TimeDurationRule<TimeDuration::MONTH, TimePositin::END>(), fme2);
    
    
    unsigned fyb = firstCalendarDate(TimeDurationRule<TimeDuration::YEAR, TimePositin::BEGIN >(), 20200421);
    unsigned nyb = nextCalendarDate(TimeDurationRule<TimeDuration::YEAR, TimePositin::BEGIN >(), fyb);

    unsigned fye = firstCalendarDate(TimeDurationRule<TimeDuration::YEAR, TimePositin::END >(), 20200426);
    unsigned nye = nextCalendarDate(TimeDurationRule<TimeDuration::YEAR, TimePositin::END>(), fye);

     */

    unsigned fqb = firstCalendarDate(TimeDurationRule<TimeDuration::QUARTER, TimePositin::BEGIN >(), 20200101);
    unsigned nqb = nextCalendarDate(TimeDurationRule<TimeDuration::QUARTER, TimePositin::BEGIN >(), fqb);

    unsigned fqe = firstCalendarDate(TimeDurationRule<TimeDuration::QUARTER, TimePositin::END >(), 20181019);
    unsigned nqe = nextCalendarDate(TimeDurationRule<TimeDuration::QUARTER, TimePositin::END>(), fqe);

    int i = 1;
    
   
    return 0;
}





