# Compiler and Flags
CXX = g++
CXXFLAGS = -Wall -std=c++17 -g -I.

# Target executable name
TARGET = heuristica.exe

# List your C++ source files here
SRCS = candidato.cpp ADMAG.cpp main.cpp
HDRS = candidato.hpp ADMAG.hpp

# Automatically generate object file names from source files
OBJS = $(SRCS:.cpp=.o)

.PHONY: all clean

# Default rule (the first rule is run by default)
all: $(TARGET)

# Rule to link the object files into the final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile .cpp files into .o files
%.o: %.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule adapted for Windows Command Prompt (cmd)
.PHONY: clean
clean:
	@del /Q *.o *.exe 2>nul || exit 0