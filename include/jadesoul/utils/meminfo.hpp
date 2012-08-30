#ifndef MEMINFO_HPP_1324965202_37
#define MEMINFO_HPP_1324965202_37
/**
 * File: meminfo.hpp
 * Description: 
 *
 * Copyright (c) 2011 Jadesoul (Home Page: http://jadesoul.org)
 * 
 * Date: 2011-12-27 13:53:22.367000
 * Written In: Peking University, beijing, China
 */

#include "includes.hpp"

/**
 * memory information
 */

class meminfo {
private:
#ifdef ENV_WIN32_MSVC
	PROCESS_MEMORY_COUNTERS pmc;
#else
#endif

public:
	meminfo() {
#ifdef ENV_WIN32_MSVC
		HANDLE handle=GetCurrentProcess();
		GetProcessMemoryInfo(handle, &pmc, sizeof(pmc));
#else
#endif
	}
	
	friend ostream& operator <<(ostream& o, meminfo& mi) {
#ifdef ENV_WIN32_MSVC
		return o<<"Mem Usage:"
				<<endl
				
				<<"WorkingSetSize="
				<<mi.nice_repr(mi.pmc.WorkingSetSize)
				<<endl
				
				<<"PeakWorkingSetSize="
				<<mi.nice_repr(mi.pmc.PeakWorkingSetSize)
				<<endl
				
				<<"PagefileUsage="
				<<mi.nice_repr(mi.pmc.PagefileUsage)
				<<endl
				
				<<"PeakPagefileUsage"
				<<mi.nice_repr(mi.pmc.PeakPagefileUsage)
				<<endl;
#else
#endif
		return o;
	}
	
	//return physical memory size
	size_t phys() {
#ifdef ENV_WIN32_MSVC
		return pmc.WorkingSetSize;
#else
		return 0;
#endif
	}
	
	//return virtual memory size
	size_t virtu() {
#ifdef ENV_WIN32_MSVC
		return pmc.PagefileUsage;
#else
		return 0;
#endif
	}
	
	uchar get_unit(ulong bytes) {
		ulong& b=bytes;
		const uchar B=1, K=2, M=3, G=4, T=5, P=6;
		if (b<(1<<10)) return B;
		if (b<(1<<20)) return K;
		if (b<(1<<30)) return M;
		b/=1<<30;
		if (b<(1<<10)) return G;
		if (b<(1<<20)) return T;
		if (b<(1<<30)) return P;
		return B;
	}
	
	string nice_repr(ulong bytes) {
		ulong& b=bytes;
		uchar u=get_unit(labs(b));
		const uchar B=1, K=2, M=3, G=4, T=5, P=6;
		char buf[50];
		if (u==B) sprintf(buf, fmtstr_ulong, b);
		else if (u==K) sprintf(buf, "%.1fK", (float)b/(1<<10));
		else if (u==M) sprintf(buf, "%.1fM", (float)b/(1<<20));
		else if (u==G) sprintf(buf, "%.1fG", (float)b/(1<<30));
		else if (u==T) sprintf(buf, "%.1fT", (float)b/(1<<30)/(1<<10));
		else if (u==P) sprintf(buf, "%.1fP", (float)b/(1<<30)/(1<<20));
		else sprintf(buf, fmtstr_ulong, b);
		return buf;
	}
};


map<uint, meminfo> __gmt;	//global meminfo table

void mem_init(uint id=0) {
	__gmt[id]=meminfo();
}

void mem_gap(char* msg="", uint id=0, bool update=true) {
	if_not_in(id, __gmt) return;
	meminfo now;
	cout<<msg<<": "<<now.nice_repr(now.virtu()-__gmt[id].virtu())<<endl;
	if (update) __gmt[id]=now;
}

#endif /* MEMINFO_HPP_1324965202_37 */
