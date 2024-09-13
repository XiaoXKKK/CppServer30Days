server:
	g++ -o ./bin/server server.cpp utils.cpp -std=c++11 & ./bin/server

client:
	g++ -o ./bin/client client.cpp utils.cpp -std=c++11 & ./bin/client

clean:
	rm -f ./bin/server ./bin/client