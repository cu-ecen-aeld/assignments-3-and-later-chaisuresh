INCLUDE_DIRS = -Wall -Werror
LIB_DIRS =
CC=$(CROSS_COMPILE)gcc

CFLAGS= -O0 $(INLCUDE_DIRS) $(CDEFS)

CFILES= aesdsocket.c

SRCS= ${CFILES}
OBJS= ${CFILES:.c=.o}

all: aesdsocket

clean:
	-rm -f *.o *.d
	-rm -f perfmon aesdsocket

aesdsocket: aesdsocket.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o

.c.o:
	$(CC) $(CFLAGS) -c $<

