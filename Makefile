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
	valgrind --suppressions=./vlgsuppress.conf ./bin/noi 2>log.log
	less -R log.log

vlgf: noi
	
	valgrind --suppressions=./vlgsuppress.conf --leak-check=full ./bin/noi 2>log.log 
	less -R log.log

tests: noi
	./runTests.sh
	less -R ./testResult.log

fixterm:
	stty sane
