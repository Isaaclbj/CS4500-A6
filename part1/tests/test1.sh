cp ../src/client.cpp ./
cp ../src/server.cpp ./
g++ server.cpp -lpthread -o server
g++ client.cpp -lpthread -o client

echo "compiled"

rm client.cpp client server.cpp server