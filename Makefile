CC=gcc
TARGET=1m-block
SRC=1m-block.c
LDFLAGS=-lnetfilter_queue

all: $(TARGET)

$(TARGET) : $(SRC)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET)
	rm -f *.o


