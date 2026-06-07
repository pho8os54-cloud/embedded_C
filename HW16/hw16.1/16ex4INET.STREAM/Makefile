BIN := client
SRC := client.c

BIN1 := server
SRC1 := server.c

CC := gcc
FLAGS := -o 

all: $(BIN) $(BIN1)

$(BIN): $(SRC)
		$(CC) $(SRC) $(FLAGS) $(BIN)
		

$(BIN1): $(SRC1)
		$(CC) $(SRC1) $(FLAGS) $(BIN1)
		

clean:
		rm -rf $(BIN) $(BIN1)