
build:
	g++ ./src/server.cpp -lpthread -o server
	g++ ./src/client.cpp -lpthread -o client

run: clean build
	cp ./test/some_text.txt ./
	./server -ip 127.0.0.1 &
	./client -ip 127.0.0.2 & > temp.txt
	./client -ip 127.0.0.3 < some_text.txt
	
clean:
	rm -f client
	rm -f server
	rm -f temp.txt
	rm -f some_text.txt
