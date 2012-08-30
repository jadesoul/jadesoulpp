/**
 * File: file_copy.cpp
 * Description: 
 *
 * Copyright (c) 2012 Jadesoul (Home Page: http://jadesoul.sinaapp.com)
 * 
 * Date: 2012-04-21 12:16:53.357000
 * Written In: Peking University, beijing, China
 */

#include <jadesoul.hpp>

int main (int argc, char** argv) {
	if (argc!=3)
	{
		fprintf(stderr,"Usage:%s fromfile tofile\n\a",argv[0]);
		exit(1);
	}
	cout<<argv[1]<<endl;
	cout<<argv[2]<<endl;
	cout<<"success="<<cp(argv[1], argv[2])<<endl;
}

