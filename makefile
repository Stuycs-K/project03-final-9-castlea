compile: connections.o rps.o
	@gcc -o runner.o connections.o rps.o
run:
	@./runner.o
connections.o: connections.c connections.h
	@gcc -c connections.c connections.h
rps.o: rps.c rps.h
	@gcc -c rps.c rps.h
clean:
	@rm -rf *.o
