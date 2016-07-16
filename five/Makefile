CPP := g++
CC := gcc
LD := ld
RM := rm -rf

CFLAGS   :=
CPPFLAGS := 

OBJS := main.o chess.o game.o

all: five

five : $(OBJS)
	$(CPP) $(CFLAGS) -o $@ $(OBJS)

%.o : %.cpp
	$(CPP) $(CPPFLAGS) -o $@ -c $<
	
%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $<
	
clean :
	$(RM) *.exe *.o five
