CC = gcc
CFLAGS = -Wall -g
LIBS = -lncursesw
TARGET = Mymc


SRCS = interface.c logic.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)