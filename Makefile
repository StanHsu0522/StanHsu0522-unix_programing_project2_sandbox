CXX = g++
LINKFLAGS = -ldl
CFLAGS = -Wall -g -pedantic -std=c++11
LIBCFLAGS = -fPIC $(CFLAGS)
INC = -I ./include
LIBRARY = sandbox.so
LAUNCHER = sandbox

.PHONY: all clean

all: $(LIBRARY) $(LAUNCHER)

$(LAUNCHER): main.o
	$(CXX) -o $@ $< $(LINKFLAGS)

$(LIBRARY): sandbox.o
	$(CXX) -shared -o $@ $< $(LINKFLAGS)

sandbox.o: sandbox.cpp
	$(CXX) -c $(LIBCFLAGS) $(INC) -o $@ $<

main.o: main.cpp
	$(CXX) -c $(CFLAGS) $(INC) -o $@ $<

clean:
	rm -rf sandbox *.so *.o
