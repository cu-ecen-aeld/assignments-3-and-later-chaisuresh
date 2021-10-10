INCLUDE_DIRS = 
LIB_DIRS =
CC=$(CROSS_COMPILE)gcc

CFLAGS= -O0 -g -Wall -Werror $(INLCUDE_DIRS) $(CDEFS)
LDFLAGS= -pthread -lrt

CFILES= aesdsocket.c

SRCS= ${CFILES}
OBJS= ${CFILES:.c=.o}

all: aesdsocket

default: aesdsocket

clean:
	-rm -f *.o *.d
	-rm -f perfmon aesdsocket

aesdsocket: aesdsocket.o
	$(CC) -o $@ $@.o $(LDFLAGS) $(CFLAGS)

.c.o:
	$(CC) $(CFLAGS) -c $<

