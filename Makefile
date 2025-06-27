# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11

# Files
SRC = main.cpp game.cpp  # List all your source files here
OBJ = $(SRC:.cpp=.o)  # Object files will be .o (compiled .cpp)
EXEC = game_test       # Name of the output executable

# Default target (make without arguments will use this target)
all: $(EXEC)

# Linking the object files to create the executable
$(EXEC): $(OBJ)
	$(CXX) $(OBJ) -o $(EXEC)

# Compiling the source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up the generated files
clean:
	rm -f $(OBJ) $(EXEC)