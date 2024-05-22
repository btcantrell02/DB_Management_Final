CC = g++

CFLAGS = -g -Wall -std=c++0x

TARGET = project

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) $(TARGET).cpp

clean:
	$(RM) a.out
	