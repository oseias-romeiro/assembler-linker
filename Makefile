CC = gcc
CFLAGS = -I./includes -Wall
LDFLAGS = -lstdc++
SRC_DIR = ./src
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(SRC_DIR)/%.o, $(SRC_FILES))
TARGET = montador

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(SRC_DIR)/*.o
