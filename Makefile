CC = g++
ifeq ($(shell sw_vers 2>/dev/null | grep Mac | awk '{ print $$2}'),Mac)
CFLAGS = -g -DGL_GLEXT_PROTOTYPES -DOSX -Wno-deprecated-register -Wno-deprecated-declarations -Wno-shift-op-parentheses
INCFLAGS = -I./glm-0.9.2.7 -I/usr/X11/include -I./include/
LDFLAGS = -L./osxlib/ -lm -lstdc++ -lfreeimage
else
CFLAGS = -g -DGL_GLEXT_PROTOTYPES 
INCFLAGS = -I./glm-0.9.2.7
LDFLAGS = -lfreeimage
endif

RM = /bin/rm -f 
all: main
main: main.cpp readFile.o
	$(CC) $(CFLAGS) -o main main.cpp readFile.o $(INCFLAGS) $(LDFLAGS)

readFile.o: readFile.cpp
	$(CC) $(CFLAGS) -c readFile.cpp
clean :
	$(RM) *.o main
