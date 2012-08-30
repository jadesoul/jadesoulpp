all:
	@svn up
	python scripts/gen_cmake_recursively.py tests ../include
	python scripts/prepare_dirs.py build bin lib
	cd build ; cmake .. ; make -sj; cd -
	
wchar:
	@svn up
	cd build; make -sj wchar; cd -
	./bin/tests/jadesoul_dir/utils_dir/wchar
	
clean:
	python scripts/clean_dirs.py build bin lib
	
