### 
## Makefile
## 
### 

## Variables

# Tools & flags
CC=gcc
CFLAGS=--std=c99 --pedantic -Wall -W -Wmissing-prototypes 
LD=gcc
LDFLAGS= -lm

# Files
EXEC=test
OBJECTS=main.o matriceCreuse.o utile.o


## Rules

all: $(EXEC)

test: $(OBJECTS)
	$(LD) -o $(EXEC) $(OBJECTS) $(LDFLAGS) 

main.o: main.c
	$(CC) -c main.c -o main.o $(CFLAGS)

matriceCreuse.o: matriceCreuse.c
	$(CC) -c matriceCreuse.c -o matriceCreuse.o $(CFLAGS)
utile.o: utile.c
	$(CC) -c utile.c -o utile.o $(CFLAGS)



clean:
	rm -f *.o $(EXEC) *~ test.*

