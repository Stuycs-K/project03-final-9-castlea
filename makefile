default: connections.o server.o player.o rps.o
	@echo all compiled
server: server.o
	@./server.o
player: player.o
	@./player.o
connections.o: connections.c connections.h
	@gcc -c connections.c connections.h
rps.o: rps.c rps.h
	@gcc -c rps.c rps.h
server.o: server.c connections.h
	@gcc -c server.c connections.h
player.o: player.c connections.h
	@gcc -c player.c connections.h
clean:
	@rm -rf *.o
	@rm -rf *.gch
