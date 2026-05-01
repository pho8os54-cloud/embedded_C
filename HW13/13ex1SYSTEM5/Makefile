BIN := server
SRC := 13ex1server.c

BIN1 := client
SRC1 := 13ex1client.c

CC := gcc
FLAGS := -o 

all: $(BIN) $(BIN1)

$(BIN): $(SRC)
		$(CC) $(SRC) $(FLAGS) $(BIN)
		

$(BIN1): $(SRC1)
		$(CC) $(SRC1) $(FLAGS) $(BIN1)
		

clean:
		rm -rf $(BIN) $(BIN1)