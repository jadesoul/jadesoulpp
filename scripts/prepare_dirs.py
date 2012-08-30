#coding:utf8

from common import *

if __name__=='__main__':
	for d in argv[1:]:
		if not exists(d):
			md(d)
			print 'created directory: %s' % d

