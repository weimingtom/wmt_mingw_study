MACHINE             := -mwindows
# -mconsole

TARGET				:=	bin/PhysicsTest.exe
DEFINES				:=	-DSDL_BUILD -DWINDOWS_BUILD
INCLUDES			:=	-Isrc
 
LIBRARIES			:=  -lglu32 -lopengl32 -lwinmm -lSDL -lSDLmain

CODE_FILES			:=	game.c WindowsTime.c GLGraphics.c SDLMain.c BallData.c
CODE_FILES			:=	$(addprefix src/,$(CODE_FILES))

H_FILES				:=	game.h WindowsTime.h Graphics.h
H_FILES				:=	$(addprefix src/,$(H_FILES))

.O_FILES			:=	$(addprefix obj/,$(addsuffix .o,$(CODE_FILES)))

RM                  :=  rm -f
RMDIR               :=  $(RM) -r

$(TARGET): makedirs $(H_FILES) $(.O_FILES)
	g++  $(MACHINE) $(DEFINES) $(INCLUDES) $(.O_FILES) -o $@ $(LIBRARIES) 

run: $(TARGET)
	./$(TARGET)

clean:
	$(RM) $(.O_FILES) $(.O_FILES:.o=.o.d)
	$(RMDIR) bin obj

makedirs:
	mkdir -p $(sort $(dir $(.O_FILES))) bin

phony: run clean makedirs

obj/%.c.o: %.c
	gcc $(DEFINES) $(INCLUDES) -MMD -MF $@.d -c $< -o $@

obj/%.cpp.o: %.cpp
	g++ $(DEFINES) $(INCLUDES) -MMD -MF $@.d -c $< -o $@

-include $(.O_FILES:.o=.o.d)

