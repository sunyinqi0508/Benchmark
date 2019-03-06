CC=g++-8
OPT=-O -g3
DEBUG=
PROFILING=
CFLAGS= -D_GLIBCXX_USE_CXX11_ABI=0 -std=c++17 -Wno-deprecated -Wall -fPIC
LCFLAGS=-lstdc++
INCLUDES= -I ../include -I ../include/BitMagic -I . 
LIBS =  -lpthread -lstdc++fs -lpq ../lib/libroaring.a
SOURCE_FILES = $(wildcard */*.cpp)  $(wildcard */*.h) $(wildcard */*.hpp)
all: Benchmark

Benchmark: $(SOURCE_FILES)
	$(CC) $(CFLAGS) $(OPT) $(DEBUG) $(INCLUDES) -c FileIO.cpp  gc.cpp LIndex.cpp tools.cpp DataGen.cpp
	$(CC) $(CFLAGS) $(OPT) $(DEBUG) $(INCLUDES) Benchmark.cpp -o Benchmark FileIO.o gc.o LIndex.o tools.o DataGen.o $(LIBS)

Debug: $(SOURCE_FILES)
	$(CC) $(CFLAGS) -O0 -g3 $(INCLUDES) -c FileIO.cpp  gc.cpp LIndex.cpp tools.cpp DataGen.cpp
	$(CC) $(CFLAGS) -O0 -g3 $(INCLUDES) Benchmark.cpp -o Benchmark FileIO.o gc.o LIndex.o tools.o DataGen.o $(LIBS)
	
clean:
	rm -f *.o Benchmark
