BIN := server
SRC := server.c

BIN1 := client
SRC1 := client.c

CC := gcc
FLAGS := -o
FLAGS1 := -lrt

all: $(BIN) $(BIN1)

$(BIN): $(SRC)
		$(CC) $(SRC) $(FLAGS) $(BIN) $(FLAGS1)
		

$(BIN1): $(SRC1)
		$(CC) $(SRC1) $(FLAGS) $(BIN1) $(FLAGS1)
		

clean:
		rm -rf $(BIN) $(BIN1)