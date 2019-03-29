mirror_client.o: mirror_client.cpp
	g++ -c mirror_client.cpp
IOutils.o: IOutils.cpp
	g++ -c IOutils.cpp
LinkedList.o: LinkedList.cpp
	g++ -c LinkedList.cpp
ListNode.o: ListNode.cpp
	g++ -c ListNode.cpp
dfs_file.o: dfs_file.cpp
	g++ -c dfs_file.cpp
readProcess.o: readProcess.cpp
	g++ -c readProcess.cpp
writeProcess.o: writeProcess.cpp
	g++ -c writeProcess.cpp
all: mirror_client.o IOutils.o LinkedList.o ListNode.o dfs_file.o readProcess.o writeProcess.o
	g++ -o mirror_client mirror_client.o IOutils.o LinkedList.o ListNode.o dfs_file.o readProcess.o writeProcess.o
clean:
	rm -f *.o mirror_client
