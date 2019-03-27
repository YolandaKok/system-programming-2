mirror_client.o: mirror_client.cpp
	g++ -c mirror_client.cpp
IOutils.o: IOutils.cpp
	g++ -c IOutils.cpp
LinkedList.o: LinkedList.cpp
	g++ -c LinkedList.cpp
ListNode.o: ListNode.cpp
	g++ -c ListNode.cpp
all: mirror_client.o IOutils.o LinkedList.o ListNode.o
	g++ -o mirror_client mirror_client.o IOutils.o LinkedList.o ListNode.o
clean:
	rm -f *.o mirror_client
