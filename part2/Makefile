  
build:
	g++ ./src/main.cpp -lpthread -o server

run: clean build
	./main
    ./test/testsuite.cpp -lpthread -o testsuite
	
clean:
	rm -f main