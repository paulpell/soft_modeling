#
# Basic makefile for compilation of a C++ source
#
# all		- joo......
# compile	- just compile
# run		- run it
# clean		- tidy up
#
# Author	- Philipp Fonteyn
# Version	- 1.0.1
# Date		- 10. Feb 2012
#

CC = g++
MAIN = TestCpp.cpp
OBJECTS = World3D.o Vertex3D.o Surface3D.o Keyboard.o Texture.o Camera.o Cube3D.o Spring.o MassPoint3D.o Vector3D.o WorldObject.o Rope.o Cloth.o Cube.o ClothOnBox.o Force.o
SRC = World3D.cpp Vertex3D.cpp Surface3D.cpp Keyboard.cpp Texture.cpp Camera.cpp Cube3D.cpp Spring.cpp MassPoint3D.cpp Vector3D.cpp WorldObject.cpp Rope.cpp Cloth.cpp Cube.cpp ClothOnBox.cpp Force.cpp
LIBS = -lGL -lGLU -lglut -lrt
EXEC = Space
# automatic in $(CC)?
CFLAGS = $(LIBS)

all: compile run	

compile : objects main

main: objects
	$(CC) $(OBJECTS) $(MAIN) $(LIBS) -o $(EXEC)

objects: $(OBJECTS)
	#$(CC) $(SRC) -o $(EXEC) $(LIBS)  -g

run:
	./$(EXEC)

clean:
	rm $(EXEC)
#	rm -rf $(EXEC) *~
