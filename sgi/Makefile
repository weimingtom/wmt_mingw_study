CC := gcc
CPP := g++
LD := ld

CFLAGS := -g

TARGETS := test001 test002 test003 test004 test005 test006 test007

all: $(TARGETS)

test001: test001.cpp
	$(CPP) -Wno-deprecated -o $@ $<

test002: test002.cpp
	$(CPP) -o $@ $<

test003: test003.cpp
	$(CPP) -o $@ $<

test004: test004.cpp
	$(CPP) -o $@ $<
	
test005: test005.cpp
	$(CPP) -o $@ $<
	
test006: test006.cpp
	$(CPP) -o $@ $<

test007: test007.cpp
	$(CPP) -o $@ $<
	
clean:
	$(RM) *.exe *.o
