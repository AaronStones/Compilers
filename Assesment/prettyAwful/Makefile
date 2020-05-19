TARGET		:= ./compiler

# Compiler flags. Optmised for debugging, not speed
CPPFLAGS	:= -pedantic -Wall -Wextra -Werror -g -ggdb -O0 -std=c++11
LDFLAGS		:=  #-static-libgcc -static-libstdc++ -lgcc_eh

# Required directories for building
SOURCE_DIR	:= ./sourceFiles
PRODUCT_DIR	:= ./
OBJECTS_DIR	:= ./compFiles
INSTALL_DIR	:= /usr/local/bin

# Simple project, we only scan one directory (no subdirectory)
SOURCES		:= $(wildcard $(SOURCE_DIR)/*.cpp) $(wildcard $(SOURCE_DIR)/*/*.cpp)
OBJECTS		:= $(patsubst $(SOURCE_DIR)/%.cpp, $(OBJECTS_DIR)/%.o, $(SOURCES))

.PHONY: clean tests
all: $(TARGET)

	
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
