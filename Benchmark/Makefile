CC=g++-8
OPT=-O3
DEBUG=
PROFILING=
CFLAGS= -D_GLIBCXX_USE_CXX11_ABI=0 -std=c++1z -Wno-deprecated -Wall -fPIC
LCFLAGS=-lstdc++
INCLUDES= -I ../include -I ../include/BitMagic -I . 
LIBS =  -lpthread -lstdc++fs ../lib/libroaring.a
all: Benchmark

Benchmark: 
	$(CC) $(CFLAGS) $(OPT) $(DEBUG) $(INCLUDES) -c FileIO.cpp  gc.cpp LIndex.cpp 
	$(CC) $(CFLAGS) $(OPT) $(DEBUG) $(INCLUDES) Benchmark.cpp -o Benchmark FileIO.o gc.o LIndex.o $(LIBS)

clean:
	rm -f *.o Benchmark
