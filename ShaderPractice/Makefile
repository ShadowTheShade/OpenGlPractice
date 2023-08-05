# the compiler: gcc for C program, define as g++ for C++
CC = clang++
# compiler flags:
#  -g     - this flag adds debugging information to the executable file
#  -Wall  - this flag is used to turn on most compiler warnings
CFLAGS  = -lGL -lglfw
# The build target 
TARGET = src/shaderPractice
GLADPATH = glad/glad.c
all: $(TARGET)
$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) -o $(TARGET) $(GLADPATH) $(TARGET).cpp
clean:
	$(RM) $(TARGET)
