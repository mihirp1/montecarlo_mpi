CC = mpicc
CFLAGS = -g -Wall -Wstrict-prototypes
CFILE1 = monte_carlo_neutron.c
CFILE2 = monte_carlo.h
OBJECT1 = monte_carlo_neutron.o
OBJECTS = $(OBJECT1)
PROG = monte_carlo

all:$(PROG)
$(PROG):$(OBJECTS)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJECTS)
$(OBJECT1):$(CFILE2)
	$(CC) $(CFLAGS) -c $(CFILE1)
clean:
	rm -f $(OBJECTS) $(PROG)
