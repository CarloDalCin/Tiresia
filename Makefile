# Compiler
CXX = g++
CXXFLAGS = -Wall -Wextra -Iinclude -std=c++23

# Directory
SRC_DIR = src
TEST_DIR = tests
OBJ_DIR = build
BIN_DIR = build

# executable name
TARGET = $(BIN_DIR)/tiresia
TEST_TARGET = $(BIN_DIR)/tests

# Find all .cpp files in src/ and tests/
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
TEST_SRCS = $(wildcard $(TEST_DIR)/*.cpp)

# Convert to .o into build/
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))
TEST_OBJS = $(patsubst $(TEST_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(TEST_SRCS))

# Default target
all: $(TARGET)

# Final link
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compilating file .cpp -> .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compilating tests -> .o
$(OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build tests
tests: $(OBJS) $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $^

# Debug mode
debug: CXXFLAGS += -g -O0 -DDEBUG
debug: clean all

# Make build directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean all
clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGET) $(TEST_TARGET)

# Run the main program
run: $(TARGET)
	./$(TARGET)

# Run the tests program
run-tests: tests
	./$(TEST_TARGET)

.PHONY: all clean run tests run-tests debug
