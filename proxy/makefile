proxy: src/server.c
	gcc -L  lib/libcomm/ -lcommunicate -o bin/proxy src/server.c
proxy: src/client.c
	gcc -L  lib/libcomm -lcommunicate -o bin/proxyCli src/client.c
clean:
	rm -rf *.o
