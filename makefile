CC=gcc
SERVER=text-server
CLIENT=text-client

text: $(SERVER) $(CLIENT)

text-client:
	$(CC) -pthread -o $(CLIENT) $(CLIENT).c

text-server:
	$(CC) -pthread -o $(SERVER) $(SERVER).c

clean:
	rm -rf $(CLIENT) $(SERVER) server client *.o
