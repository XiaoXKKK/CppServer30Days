server:
	g++ -o ./bin/server server.cpp utils.cpp -std=c++11

client:
	g++ -o ./bin/client client.cpp utils.cpp -std=c++11

clean:
	rm -f ./bin/server ./bin/client