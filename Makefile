CFLAGS=-O2 -Wall -Wextra -pthread $(OPTFLAGS)
LIBS=-ldl $(OPTLIBS)

SOURCES=$(wildcard hardware/**/*.c data_structure/**/*.c software_resource/**/*.c *.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))

all: $(OBJECTS)
	$(CC) -o simulator -pthread $(OBJECTS)

clean:
	rm -rf $(OBJECTS)
