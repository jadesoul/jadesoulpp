#coding:utf8

import os, sys, re, datetime
from re import compile
from sys import argv, exit
from os import system as run, getcwd as cwd, listdir, mkdir as md, makedirs as mds
from os import name as osname
from os.path import split as splitdir, splitext, join, dirname, isfile, islink, isdir, exists
from os.path import getsize as fsize, getatime as fatime, getctime as fctime
from shutil import copy, rmtree as rm
from time import sleep

argc=len(argv)
windows=(osname=='nt')
linux=not windows
now=lambda: datetime.datetime.now()

# dp, fn, name, ext=split(fullpath)
def split(fullpath):
	dirpath, filename=splitdir(fullpath)
	name, ext=splitext(filename)
	return dirpath, filename, name, ext

def fread(path, mode='r'):
	f=open(path, mode)
	s=f.read()
	f.close()
	return s
	
def fwrite(s, path, append=False, binary=False):
	mode='w'
	if append: mode='a'
	if binary: mode+='b'
	f=open(path, mode)
	f.write(s)
	f.close()
	
def fappend(s, path, binary=False):
	fwrite(s, path, True, binary)
	
if __name__=='__main__':
	pass

