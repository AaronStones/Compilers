TARGET		:= compiler

# Compiler flags. Optmised for debugging, not speed
CPPFLAGS	:= -pedantic -Wall -Wextra -Werror -g -ggdb -O0 -std=c++11
LDFLAGS		:=  #-static-libgcc -static-libstdc++ -lgcc_eh

# Required directories for building
SOURCE_DIR	:= ./sourceFiles
TESTS_DIR	:= ./testFiles
PRODUCT_DIR	:= ./
OBJECTS_DIR	:= ./buildFiles
INSTALL_DIR	:= /usr/local/bin

# Simple project, we only scan one directory (no subdirectory)
SOURCES		:= $(wildcard $(SOURCE_DIR)/*.cpp) $(wildcard $(SOURCE_DIR)/*/*.cpp)
OBJECTS		:= $(patsubst $(SOURCE_DIR)/%.cpp, $(OBJECTS_DIR)/%.o, $(SOURCES))
PALS		:= $(wildcard $(TESTS_DIR)/*/*.txt) $(wildcard $(TESTS_DIR)/*/*/*.txt)

.PHONY: clean tests
all: $(TARGET)

tests: $(PALS)
	
install: $(TARGET)
	@echo "[installing product $(TARGET)]"
	@cp $(PRODUCT_DIR)/$(TARGET) $(INSTALL_DIR)/$(TARGET)

# Build the PAL compiler
$(TARGET): $(PRODUCT_DIR) $(OBJECTS_DIR) $(OBJECTS)
	@echo "[linking product $(TARGET)]"
	@$(CXX) $(LDFLAGS) $(OBJECTS) -o $(PRODUCT_DIR)/$(TARGET)

# Build a single object file from its .cpp counterpart
$(OBJECTS_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo "[compiling $(notdir $<)]"
	@$(CXX) $(CPPFLAGS) -c $< -o $@
	
$(TESTS_DIR)/%.txt: $(TARGET) FORCE
	@echo "[testing with \`$@\`]"
	@$(PRODUCT_DIR)/$(TARGET) $@ /dev/null

# Create the build products directory
$(PRODUCT_DIR):
	@mkdir -p $(PRODUCT_DIR)

# Create the intermediate objects directory
$(OBJECTS_DIR):
	@mkdir -p $(OBJECTS_DIR)

# Delete build products and intermediate objects
clean: FORCE
	@rm -rf $(OBJECTS_DIR)
	@rm -rf $(PRODUCT_DIR)/$(TARGET)

FORCE:
