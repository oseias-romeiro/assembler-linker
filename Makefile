CC = gcc
CFLAGS = -I./includes -Wall
LDFLAGS = -lstdc++
SRC_DIR = ./src
ASSEMBLER_DIR = $(SRC_DIR)/assembler
LINKER_DIR = $(SRC_DIR)/linker
ASSEMBLER_SRC_FILES = $(wildcard $(ASSEMBLER_DIR)/*.cpp)
LINKER_SRC_FILES = $(wildcard $(LINKER_DIR)/*.cpp)
ASSEMBLER_OBJ_FILES = $(patsubst $(ASSEMBLER_DIR)/%.cpp, $(ASSEMBLER_DIR)/%.o, $(ASSEMBLER_SRC_FILES))
LINKER_OBJ_FILES = $(patsubst $(LINKER_DIR)/%.cpp, $(LINKER_DIR)/%.o, $(LINKER_SRC_FILES))
ASSEMBLER_TARGET = montador
LINKER_TARGET = linker

all: $(ASSEMBLER_TARGET) $(LINKER_TARGET)

$(ASSEMBLER_TARGET): $(ASSEMBLER_OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(LINKER_TARGET): $(LINKER_OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(ASSEMBLER_DIR)/%.o: $(ASSEMBLER_DIR)/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

$(LINKER_DIR)/%.o: $(LINKER_DIR)/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(ASSEMBLER_DIR)/*.o $(LINKER_DIR)/*.o
