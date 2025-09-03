# Compilatore
CXX = g++
CXXFLAGS = -Wall -Wextra -Iinclude -std=c++23

# Cartelle
SRC_DIR = src
OBJ_DIR = build
BIN_DIR = build

# Nome eseguibile
TARGET = $(BIN_DIR)/tiresia

# Trova tutti i file sorgente .cpp
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Converte in .o dentro build/
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# Regola principale
all: $(TARGET)

# Link finale
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compilazione file .cpp -> .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Crea la cartella build se non esiste
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Pulizia
clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGET)

# esegue il programma
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean

