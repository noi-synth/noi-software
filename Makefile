all:
	scons -Q

clean:
	scons -c
	rm -rf doc

run:
	scons -Q
	./bin/noi

rebuild:
	scons -c
	scons -Q
