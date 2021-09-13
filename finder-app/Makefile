INCLUDE_DIRS = 
LIB_DIRS = 
CC=$(CROSS_COMPILE)gcc

CFLAGS= -O0 $(INLCUDE_DIRS) $(CDEFS)

CFILES= writer.c

SRCS= ${CFILES}
OBJS= ${CFILES:.c=.o}

all: writer

clean:
	-rm -f *.o *.d
	-rm -f perfmon writer

writer: writer.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o 

.c.o:
	$(CC) $(CFLAGS) -c $<
