#coding:utf8

from common import *

if __name__=='__main__':
	for d in argv[1:]:
		if isdir(d):
			rm(d)
			print 'deleted directory: %s' % d

