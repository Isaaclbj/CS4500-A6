cp ../src/client.cpp ./
cp ../src/server.cpp ./
g++ server.cpp -lpthread -o server
g++ client.cpp -lpthread -o client

./server -ip 127.0.0.1 &
./client -ip 127.0.0.2 &
./client -ip 127.0.0.3 &
./client -ip 127.0.0.4 &
./client -ip 127.0.0.5 &
./client -ip 127.0.0.6 &
./client -ip 127.0.0.7 &
./client -ip 127.0.0.8 &
./client -ip 127.0.0.9 &
./client -ip 127.0.0.10 &
./client -ip 127.0.0.11 &


rm client.cpp client server.cpp server