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
SRC = TestCpp.cpp World3D.cpp Vertex3D.cpp Surface3D.cpp Keyboard.cpp Texture.cpp Camera.cpp Cube3D.cpp Spring.cpp MassPoint3D.cpp Vector3D.cpp
LIBS = -lGL -lGLU -lglut
EXEC = Space

all: compile run	

compile : 
	$(CC) $(SRC) -o $(EXEC) -lglut -lGL -lGLU

run:
	./$(EXEC)

clean:
	rm $(EXEC)
#	rm -rf $(EXEC) *~
