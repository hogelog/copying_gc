TARGET = gctest
SOURCES = gc.c main.c
OBJS = $(SOURCES:.c=.o)
CC = gcc
CFLAGS = -g -Wall
LDFLAGS =  

all: deps $(TARGET)
clean:
	rm -f $(OBJS) $(TARGET) deps
deps:
	$(CC) -MM $(SOURCES) >deps

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(LDFLAGS) -o $@

.c.o:
	$(CC) $(CFLAGS) -c $<

include deps
