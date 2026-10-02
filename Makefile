# Compiler and Flags
CXX      := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -Isrc -Iassets -D_WIN32_WINNT=0x0601
LDFLAGS  := -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -mwindows

# Directories
SRC_DIR   := src
BUILD_DIR := build
BIN_DIR   := bin
ASSETS_DIR:= assets

# Target Output Executable
TARGET    := $(BIN_DIR)/game.exe

# Source and Object Files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# OS detection for directory and shell operations
ifeq ($(OS),Windows_NT)
    MKDIR = if not exist "$(subst /,\,$(1))" mkdir "$(subst /,\,$(1))"
    RMDIR = if exist "$(subst /,\,$(1))" rmdir /S /Q "$(subst /,\,$(1))"
else
    MKDIR = mkdir -p $(1)
    RMDIR = rm -rf $(1)
endif

# Default Target
all: $(TARGET)

# Link Binary
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# Compile C++ Source Files into Object Files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Ensure Directories Exist
$(BUILD_DIR):
	@$(call MKDIR,$(BUILD_DIR))

$(BIN_DIR):
	@$(call MKDIR,$(BIN_DIR))

# Clean Build Artifacts
clean:
	@$(call RMDIR,$(BUILD_DIR))
	@$(call RMDIR,$(BIN_DIR))
	@echo Cleaned build and bin folders.

# Run the compiled executable
run: all
	$(TARGET)

.PHONY: all clean run
