# Compiler
CXX = g++
CXXFLAGS = -Wall -Wextra -Iinclude -std=c++23

# Directory
SRC_DIR = src
TEST_DIR = tests
OBJ_DIR = build
BIN_DIR = build

# Executable name
TARGET = $(BIN_DIR)/tiresia
TEST_TARGET = $(BIN_DIR)/tests

# All source files in src/
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Remove main.cpp from SRCS
SRCS_NO_MAIN = $(filter-out $(SRC_DIR)/main.cpp,$(SRCS))

# All test sources
TEST_SRCS = $(wildcard $(TEST_DIR)/*.cpp)

# Object files
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))
OBJS_NO_MAIN = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS_NO_MAIN))
TEST_OBJS = $(patsubst $(TEST_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(TEST_SRCS))

# Default target
all: $(TARGET)

# Main program
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Test program
$(TEST_TARGET): $(OBJS_NO_MAIN) $(TEST_OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile .cpp -> .o (src/)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile .cpp -> .o (tests/)
$(OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create directories if not exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Debug mode
debug: CXXFLAGS += -g -O0 -DDEBUG
debug: clean all

# Clean all
clean:
	rm -rf $(OBJ_DIR) $(TARGET) $(TEST_TARGET)

# Run the main program
run: $(TARGET)
	./$(TARGET)

# Run the tests program
run-tests: $(TEST_TARGET)
	./$(TEST_TARGET)

.PHONY: all clean run run-tests debug
