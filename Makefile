CC := g++
FLAGS := -std=c++11 -Wall
INC_DIR := include
SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin
INCLUDE := -I ./$(INC_DIR)

TARGET := Agenda

OBJECTS := $(BUILD_DIR)/Agenda.o $(BUILD_DIR)/Date.o $(BUILD_DIR)/User.o \
			$(BUILD_DIR)/Meeting.o $(BUILD_DIR)/Storage.o \
			$(BUILD_DIR)/Logger.o $(BUILD_DIR)/AgendaService.o \
			$(BUILD_DIR)/AgendaUI.o


$(BIN_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	@$(CC) $(FLAGS) $(INCLUDE) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	@$(CC) $(FLAGS) $(INCLUDE) -c $^ -o $@  

clean:
	@echo "Cleaning..."
	@rm -rf $(BUILD_DIR) $(BIN_DIR)

run: $(BIN_DIR)/$(TARGET)
	@$(BIN_DIR)/$(TARGET)