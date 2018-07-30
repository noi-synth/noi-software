all:
	scons -Q

clean:
	scons -c

run:
	scons -Q
	./bin/noi
