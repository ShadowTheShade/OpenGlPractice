# the compiler: gcc for C program, define as g++ for C++
CC = clang++
# compiler flags:
#  -g     - this flag adds debugging information to the executable file
#  -Wall  - this flag is used to turn on most compiler warnings
CFLAGS  = -Wall -g -lGL -lglfw


SHADERS = shaders
SRC = src

# The build target 

STBIMG = shaders/stb_image

GLAD = /usr/include/glad/glad.c

# Target file
TARGET = game

# Main file
MAIN = src/worldPractice

SHAD = shaders/shader

all: $(TARGET).exe

$(TARGET).exe: $(GLAD) $(MAIN).o 
	$(CC) $(CFLAGS) -o $(TARGET).exe $(GLAD) $(MAIN).o

$(MAIN).o : $(MAIN).cpp $(SHAD).h $(SHADERS)/fragmentShader.fs $(SHADERS)/vertexShader.vs
	$(CC) -c $(MAIN).cpp -o $(MAIN).o

clean:
	$(RM) $(TARGET).exe
