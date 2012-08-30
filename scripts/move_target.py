#coding:utf8

from common import *

if __name__=='__main__':
	if argc!=4:
		print 'parameters error'
		exit()
		
	cmake_root=argv[1]
	build_dir=argv[2]
	pos=build_dir.rfind('build')
	bin_dir=build_dir[:pos]+'bin'+build_dir[pos+5:]
	lib_dir=cmake_root+'/lib'
	target_name=argv[3]

	dfs=listdir(build_dir)
	if 'Debug' in dfs:
		build_dir=build_dir+'/Debug'
	elif 'Release' in dfs:
		build_dir=build_dir+'/Release'
	
	# print 'build_dir:', build_dir
	
	bin_files=[]
	lib_files=[]
	for fn in listdir(build_dir):
		fp=build_dir+'/'+fn
		if isfile(fp):
			if fn.startswith('lib'+target_name) or fn.endswith('.so') or fn.endswith('.a') or fn.endswith('.lib') or fn.endswith('.dll'):
				lib_files.append(fp)
			if fn==target_name or fn==target_name+'.exe':
				bin_files.append(fp)
	
	#move bin files
	# print 'bin_files:', bin_files
	if bin_files:
		if not isdir(bin_dir): mds(bin_dir)
		cmd='cp %s %s' % (' '.join(bin_files), bin_dir)
		run(cmd)
	
	#move lib files
	# print 'lib_files:', lib_files
	if lib_files:
		if not isdir(lib_dir): md(lib_dir)
		cmd='cp %s %s' % (' '.join(lib_files), lib_dir)
		run(cmd)
	
	