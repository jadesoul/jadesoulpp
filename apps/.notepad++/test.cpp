/**
 * File: test.cpp
 * Description: 
 *
 * Copyright (c) 2012 Jadesoul (Home Page: http://jadesoul.sinaapp.com)
 * 
 * Date: 2012-07-18 13:09:12.621000
 * Written In: Peking University, beijing, China
 */

#include <jadesoul.hpp>

int main () {
	str s=fread("test.obj");
	for_n(i, s.size()) printf("%x", (uchar)s[i]);
}

