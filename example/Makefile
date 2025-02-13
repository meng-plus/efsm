CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic -I../core

SRCS = ../core/efsm.c example.c
OBJS = $(SRCS:.c=.o)
EXEC = example.out

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run:
	./$(EXEC)

clean:
	rm -f $(OBJS) $(EXEC)
