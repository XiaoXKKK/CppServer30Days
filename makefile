server:
	g++ -o ./bin/server server.cpp -std=c++11 & ./bin/server

client:
	g++ -o ./bin/client client.cpp -std=c++11 & ./bin/client