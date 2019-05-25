all: noi
	

noi:
	scons -Q -j 4

clean:
	scons -c -j 4
	rm -rf doc

run: noi
	./bin/noi 2>log.log

rebuild:
	scons -c
	scons -Q -j 4

debug: noi
	./bin/noi 2>log.log
	less -R log.log
	stty sane

doc:
	doxygen Doxyfile

vlg: noi
	valgrind --suppressions=./vlgsuppress.conf ./bin/noi 2>log.log
	less -R log.log
	stty sane

vlgf: noi
	
	valgrind --suppressions=./vlgsuppress.conf --leak-check=full ./bin/noi 2>log.log 
	less -R log.log

tests: noi
	./runTests.sh
	less -R ./testResult.log

fixterm:
	stty sane
