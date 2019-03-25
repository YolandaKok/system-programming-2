mirror_client.o: mirror_client.cpp
	g++ -c mirror_client.cpp
IOutils.o: IOutils.cpp
	g++ -c IOutils.cpp
all: mirror_client.o IOutils.o
	g++ -o mirror_client mirror_client.o IOutils.o
clean:
	rm -f *.o mirror_client
