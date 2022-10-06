CFLAGS=-std=c11 -g -static
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
HDRS=$(SRCS:.c=.h)

0cc: $(OBJS)
	$(CC) -o 0cc $(OBJS) $(CFLAGS)

$(OBJS): $(HDRS)

test: 0cc
	./test.sh

clean:
	rm -f 0cc *.o *~ tmp*

.PHONY: test clean