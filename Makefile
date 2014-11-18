CFLAGS=-I.
LDFLAGS=
LIBS=

all:
	g++ test.cpp -o test $(CFLAGS) $(LDFLAGS) $(LIBS)

clean:
	rm test