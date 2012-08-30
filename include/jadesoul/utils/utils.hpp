#ifndef UTILS_HPP_1324964837_75
#define UTILS_HPP_1324964837_75
/**
 * File: utils.hpp
 * Description: 
 *
 * Copyright (c) 2011 Jadesoul (Home Page: http://jadesoul.org)
 * 
 * Date: 2011-12-27 13:47:17.752000
 * Written In: Peking University, beijing, China
 */

#include "includes.hpp"
#include "datetime.hpp"
#include "meminfo.hpp"
#include "dump.hpp"
#include "range.hpp"

#include "object.hpp"
#include "num.hpp"
// #include "wchar.hpp"
// #include "unicode.hpp"
#include "str.hpp"
#include "tuple.hpp"
#include "list.hpp"
#include "set.hpp"
#include "dict.hpp"

// #include "json.hpp"
#include "filesys.hpp"
#include "network.hpp"

// template<class T>
// inline list<T> list(set<T>& s) {
	// return list<T>(s.begin(), s.end());
// }

template<class T>
inline set<T> unique(list<T>& l) {
	set<T> s;
	uint n=l.size();
	for_n(i, n) s.add(l[i]);
	return s;
}

template<class T>
inline list<T> uniqued(list<T>& l) {
	return list<T>(unique(l));
}

void sys_init(uint id=0) {
	time_init(id);
	mem_init(id);
}

void sys_gap(char* msg, uint id=0, bool update=true) {
	if_not_in(id, __gdt) return;	
	if_not_in(id, __gmt) return;
	meminfo now;
	cout<<msg<<": "<<datetime().gap(__gdt[id])<<"ms "<<now.nice_repr(now.virtu()-__gmt[id].virtu())<<endl;
	if (update) {
		__gdt[id]=datetime();
		__gmt[id]=now;
	}
}

#endif /* UTILS_HPP_1324964837_75 */
