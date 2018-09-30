all: noi
	

noi:
	scons -Q -j 8

clean:
	scons -c -j 8
	rm -rf doc

run: noi
	./bin/noi 2>log.log

rebuild:
	scons -c
	scons -Q -j 8

debug: noi
	./bin/noi 2>log.log
	less -R log.log

vlg: noi
	valgrind ./bin/noi 2>log.log
	less -R log.log

fixterm:
	stty sane
