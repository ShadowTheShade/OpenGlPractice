# the compiler: gcc for C program, define as g++ for C++
CC = clang++
# compiler flags:
#  -g     - this flag adds debugging information to the executable file
#  -Wall  - this flag is used to turn on most compiler warnings
CFLAGS  = -lGL -lglfw
# The build target 
SRC = src/
TARGET = partOne
GLAD = /usr/include/glad/glad.c
all: $(TARGET).exe
$(TARGET).exe: $(SRC)$(TARGET).cpp $(GLAD)
	$(CC) $(CFLAGS) -o $(TARGET).exe $(GLAD) $(SRC)$(TARGET).cpp
clean:
	$(RM) $(TARGET).exe
