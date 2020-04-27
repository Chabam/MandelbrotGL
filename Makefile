CC = clang

STD_LIBS = -lstdc++ -lm
GLEW_LIBS = -lGLEW -lGLU -lGL
GLFW_LIBS = -lglfw
LIBS = $(GLFW_LIBS) $(GLEW_LIBS) $(STD_LIBS)

FLAGS = -std=c++17 -g -Wall -Wextra -Werror $(LIBS) -o
BUILD_DIR = ./bin
SRC_DIR = ./src
INCLUDE = $(SRC_DIR)/main.cpp \
		  $(SRC_DIR)/buffers.cpp \
		  $(SRC_DIR)/shader.cpp \
		  $(SRC_DIR)/window.cpp
EXEC = MandelbrotGL

all:
	$(CC) $(INCLUDE) $(FLAGS) $(BUILD_DIR)/$(EXEC)

clean:
	rm $(BUILD_DIR)/*

install:
	cp $(BUILD_DIR)/$(EXEC) /bin