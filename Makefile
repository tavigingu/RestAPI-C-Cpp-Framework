# Define compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -pthread

# Include paths
INCLUDES = -Iinclude -IDB  # Adaugă și directorul db la căile de includere

# Source files and target
SOURCES = $(wildcard src/*.cpp) DB/MyDB.cpp  # Adaugă MyDB.cpp la lista de surse
TARGET = myserver

# Define object files based on source files
OBJS = $(SOURCES:.cpp=.o)

# Default rule to build the target
all: $(TARGET)

# Rule to link object files and create the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(TARGET) $(OBJS)

# Rule to compile each source file into an object file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean up object files and executable
clean:
	rm -f $(OBJS) $(TARGET)

# Optional: 'run' rule to build and run the server
run: all
	./$(TARGET)
