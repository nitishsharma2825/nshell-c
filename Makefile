CC = gcc
CFLAGS = -Wall -g
OBJ = edsh.o commands.o

all: edsh

edsh: $(OBJ)
	$(CC) $(CFLAGS) -o edsh $(OBJ)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f edsh $(OBJ)