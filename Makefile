# Compiler
CXX = g++
CXXFLAGS = -std=c++17 -O2

# Raylib paths (Homebrew)
RAYLIB_PATH = $(shell brew --prefix raylib)
INCLUDES = -I$(RAYLIB_PATH)/include
LIBS = -L$(RAYLIB_PATH)/lib -lraylib \
       -framework OpenGL -framework Cocoa \
       -framework IOKit -framework CoreVideo

# Output executable
TARGET = game

# Source files
SRCS = snakeGame.cpp \
       gameMechs.cpp \
       player.cpp \
       objPos.cpp \
       objPosArrayList.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default rule
all: $(TARGET)

# Link
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

# Compile
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean
clean:
	rm -f $(OBJS) $(TARGET)
