BIN_SERVER := server
BIN_CLIENT := client

SRC_SERVER := server.c
SRC_CLIENT := client.c interface.c

CC := gcc
CFLAGS := -Wall
LIBS := -lncursesw -lpthread

all: $(BIN_SERVER) $(BIN_CLIENT)

$(BIN_SERVER): $(SRC_SERVER)
	$(CC) $(CFLAGS) $(SRC_SERVER) -o $(BIN_SERVER)

$(BIN_CLIENT): $(SRC_CLIENT)
	$(CC) $(CFLAGS) $(SRC_CLIENT) -o $(BIN_CLIENT) $(LIBS)

clean:
	rm -rf $(BIN_SERVER) $(BIN_CLIENT)