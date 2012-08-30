#ifndef DATETIME_HPP_1324965118_2
#define DATETIME_HPP_1324965118_2
/**
 * File: datetime.hpp
 * Description: 
 *
 * Copyright (c) 2011 Jadesoul (Home Page: http://jadesoul.org)
 * 
 * Date: 2011-12-27 13:51:58.196000
 * Written In: Peking University, beijing, China
 */

#include "includes.hpp"

/**
 *	datetime
 */

class datetime {
	time_t secs;
#ifdef OS_WIN32
	SYSTEMTIME sys;
#else
	struct timeval now;
#endif
public:
	datetime():secs(time(NULL)) {
		//init as now time
#ifdef OS_WIN32
		GetLocalTime(&sys);
#else
		gettimeofday(&now, NULL);
#endif
	}
	
	friend ostream& operator <<(ostream& o, datetime& dt) {
		char str[30];
#ifdef OS_WIN32
		const char* fmt="%4d-%02d-%02d %02d:%02d:%02d.%03d %1d/7";
		sprintf(str, fmt, 
				dt.sys.wYear,
				dt.sys.wMonth,
				dt.sys.wDay,
				dt.sys.wHour,
				dt.sys.wMinute, 
				dt.sys.wSecond,
				dt.sys.wMilliseconds,
				dt.sys.wDayOfWeek);
#else
		const char* fmt="%Y-%m-%d %H:%M:%S";
		strftime(str, 30, fmt, localtime(&dt.secs));
#endif
		return o<<str;
	}
	
	size_t millisecs() {
		size_t ms=0;
#ifdef OS_WIN32
		ms=secs*1000+sys.wMilliseconds;
#else
		ms=now.tv_usec;
#endif
		return ms;
	}
	
	int gap(datetime& dt) {
		return this->millisecs() - dt.millisecs();
	}
};

map<uint, datetime> __gdt;	//global datetime table

void time_init(uint id=0) {
	__gdt[id]=datetime();
}

void time_gap(const char* msg, uint id=0, bool update=true) {
	if_not_in(id, __gdt) return;
	cout<<msg<<": "<<datetime().gap(__gdt[id])<<"ms"<<endl;
	if (update) __gdt[id]=datetime();
}




#endif /* DATETIME_HPP_1324965118_2 */
