CC = gcc
CXX = g++
LINKFLAGS = -ldl
CFLAGS = -Wall -g -pedantic
LIBCFLAGS = -fPIC $(CFLAGS)
LIBRARY = sandbox.so
LAUNCHER = sandbox

.PHONY: all clean

all: sandbox sandbox.so

$(LAUNCHER): main.o
	$(CXX) -o $@ $< $(LINKFLAGS)

$(LIBRARY): sandbox.o
	$(CXX) -shared -o $@ $< $(LINKFLAGS)

sandbox.o: sandbox.cpp
	$(CXX) -c $(LIBCFLAGS) -o $@ $<

main.o: main.cpp
	$(CXX) -c $(CFLAGS) -o $@ $<

clean:
	rm -rf sandbox *.so *.o