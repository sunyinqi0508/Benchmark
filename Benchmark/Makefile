CC=g++-8
OPT=-O3
DEBUG=
PROFILING=
CFLAGS= -D_GLIBCXX_USE_CXX11_ABI=0 -std=c++1z -Wno-deprecated -Wall -fPIC
LCFLAGS=-lstdc++
INCLUDES= -I ../include -I ../include/BitMagic -I . 
LIBS =  -lpthread -lstdc++fs -lpq ../lib/libroaring.a
all: Benchmark

Benchmark:
	$(CC) $(CFLAGS) $(OPT) $(DEBUG) $(INCLUDES) -c FileIO.cpp  gc.cpp LIndex.cpp tools.cpp DataGen.cpp
	$(CC) $(CFLAGS) $(OPT) $(DEBUG) $(INCLUDES) Benchmark.cpp -o Benchmark FileIO.o gc.o LIndex.o tools.o DataGen.o $(LIBS)

Debug:
	$(CC) $(CFLAGS) -O0 -g3 $(INCLUDES) -c FileIO.cpp  gc.cpp LIndex.cpp tools.cpp DataGen.cpp
	$(CC) $(CFLAGS) -O0 -g3 $(INCLUDES) Benchmark.cpp -o Benchmark FileIO.o gc.o LIndex.o tools.o DataGen.o $(LIBS)
	
clean:
	rm -f *.o Benchmark
