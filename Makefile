all:
	scons -Q -j 8

clean:
	scons -c -j 8
	rm -rf doc

run:
	scons -Q -j 8
	./bin/noi 2>log.log

rebuild:
	scons -c
	scons -Q -j 8

debug:
	scons -Q -j 8
	./bin/noi 2>log.log
	cat log.log | less

vlg:
	scons -Q -j 8
	valgrind ./bin/noi 2>log.log
	cat log.log | less
