EXECUTABLES = cg1_ex1
CC=g++
CFLAGS= -O3 
DEPS = Node.hpp Scenegraph.hpp Robot.hpp Context.hpp
OBJ = Node.o Scenegraph.o Robot.o Context.o Main.o
LIBS = -lGLU -lGL -lglut -lm

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXECUTABLES): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o *~ $(EXECUTABLES)