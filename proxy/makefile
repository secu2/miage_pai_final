proxy: src/server.c
	gcc -lm -L  lib/libcomm/ -lcommunicate -o bin/proxy src/server.c
proxyCli: src/client.c
	gcc -lm -L  lib/libcomm -lcommunicate -o bin/proxyCli src/client.c
clean:
	rm -rf *.o
