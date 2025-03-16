# Compiler and flags
CXX       = g++
CXXFLAGS  = -Wall -O2 -std=c++23 -Iinclude -Iinclude/SDL2 
LDFLAGS := -Llib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

# Directories
SRCDIR    = src
# Source files and corresponding object files in the same folder (src)
SOURCES   := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS   := $(patsubst $(SRCDIR)/%.cpp, $(SRCDIR)/%.o, $(SOURCES))
TARGET    = MiniGolf.exe

# Default target: Build the executable
all: $(TARGET)

# Link object files to create the executable at the root folder
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Compile each source file to an object file in the src folder
$(SRCDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target to remove object files and the executable
clean:
	del /q $(SRCDIR)\*.o
	del $(TARGET)

.PHONY: all clean
