CXX      ?= g++
CPPFLAGS := -Isrc -Iassets
CXXFLAGS := -Wall -Wextra -std=c++17

ifeq ($(OS),Windows_NT)
HOST_PLATFORM := windows
else
HOST_PLATFORM := linux
endif

PLATFORM ?= $(HOST_PLATFORM)

ifeq ($(PLATFORM),windows)
TARGET_EXT := .exe
PLATFORM_FLAGS := -D_WIN32_WINNT=0x0601
SDL_LIBS := -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -mwindows
MKDIR = if not exist "$(subst /,\,$@)" mkdir "$(subst /,\,$@)"
RUN = $(TARGET)
else ifeq ($(PLATFORM),linux)
TARGET_EXT :=
PLATFORM_FLAGS :=
SDL_CFLAGS := $(shell pkg-config --cflags sdl2 SDL2_image SDL2_ttf SDL2_mixer)
SDL_LIBS := $(shell pkg-config --libs sdl2 SDL2_image SDL2_ttf SDL2_mixer)
MKDIR = mkdir -p "$@"
RUN = ./$(TARGET)
else
$(error Unsupported PLATFORM '$(PLATFORM)'; use PLATFORM=windows or PLATFORM=linux)
endif

BUILD_DIR := build/$(PLATFORM)
TARGET := bin/game$(TARGET_EXT)
SOURCES := $(wildcard src/*.cpp)
OBJECTS := $(patsubst src/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))

all: $(TARGET)

windows:
	$(MAKE) PLATFORM=windows all

linux:
	$(MAKE) PLATFORM=linux all

$(TARGET): $(OBJECTS) | bin
	$(CXX) $^ -o $@ $(LDFLAGS) $(SDL_LIBS)

$(BUILD_DIR)/%.o: src/%.cpp | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(PLATFORM_FLAGS) $(SDL_CFLAGS) -c $< -o $@

bin:
	$(MKDIR)

$(BUILD_DIR):
	$(MKDIR)

run: all
	$(RUN)

clean:
ifeq ($(OS),Windows_NT)
	if exist build rmdir /S /Q build
	if exist bin rmdir /S /Q bin
else
	rm -rf build bin
endif

.PHONY: all windows linux run clean
