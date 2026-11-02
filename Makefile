CXX ?= g++
CPPFLAGS := -Isrc -Iassets
CXXFLAGS := -Wall -Wextra -std=c++17

# Detect Host OS
ifeq ($(OS),Windows_NT)
    HOST_PLATFORM := windows
else
    HOST_PLATFORM := linux
endif

PLATFORM ?= $(HOST_PLATFORM)
BUILD_DIR := build/$(PLATFORM)

# Setup Host OS Commands for Directory Management
ifeq ($(HOST_PLATFORM),windows)
    MKDIR = if not exist "$(subst /,\,$(1))" mkdir "$(subst /,\,$(1))"
    RMDIR = if exist "$(subst /,\,$(1))" rmdir /S /Q "$(subst /,\,$(1))"
    CP    = copy /Y "$(subst /,\,$(1))" "$(subst /,\,$(2))"
    GRADLE_CMD := cd android && gradlew.bat assembleDebug
else
    MKDIR = mkdir -p "$(1)"
    RMDIR = rm -rf "$(1)"
    CP    = cp "$(1)" "$(2)"
    GRADLE_CMD := cd android && ./gradlew assembleDebug
endif

# Target-Platform Specific Configurations
ifeq ($(PLATFORM),windows)
    TARGET_NAME := game.exe
    PLATFORM_FLAGS := -D_WIN32_WINNT=0x0601
    SDL_CFLAGS := 
    SDL_LIBS := -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -mwindows
    RUN = $(TARGET)
else ifeq ($(PLATFORM),linux)
    TARGET_NAME := game_linux
    PLATFORM_FLAGS := 
    SDL_CFLAGS := $(shell pkg-config --cflags sdl2 SDL2_image SDL2_ttf SDL2_mixer)
    SDL_LIBS := $(shell pkg-config --libs sdl2 SDL2_image SDL2_ttf SDL2_mixer)
    RUN = ./$(TARGET)
else ifeq ($(PLATFORM),android)
    TARGET_NAME := game.apk
    RUN = @echo "Cannot run Android APK directly from terminal. Use an emulator or ADB."
else
    $(error Unsupported PLATFORM '$(PLATFORM)'; use PLATFORM=windows, PLATFORM=linux, or PLATFORM=android)
endif

TARGET  := bin/$(TARGET_NAME)
SOURCES := $(wildcard src/*.cpp)
OBJECTS := $(patsubst src/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))
DEPS    := $(OBJECTS:.o=.d)

# Path to the standard Gradle debug output APK
APK_SRC := android/app/build/outputs/apk/debug/app-debug.apk
APK_DST := bin/game.apk

# Logic to default the "all" rule based on Host OS
ifeq ($(HOST_PLATFORM),windows)
    # On Windows host, you can switch between windows or android
    ifeq ($(PLATFORM),android)
all: android
    else
all: $(TARGET)
    endif
else
    # On Linux host, build the linux binary
all: $(TARGET)
endif

windows:
ifeq ($(HOST_PLATFORM),windows)
	$(MAKE) PLATFORM=windows all
else
	$(error Cannot compile Windows binaries natively on a Linux host without a cross-compiler)
endif

linux:
ifeq ($(HOST_PLATFORM),linux)
	$(MAKE) PLATFORM=linux all
else
	$(error Cannot compile Linux binaries natively on a Windows host. Please use WSL or run this inside a Linux terminal)
endif

# Builds APK via Gradle, ensures 'bin' exists, then copies the APK
android: | bin
ifeq ($(HOST_PLATFORM),windows)
	$(GRADLE_CMD)
	@$(call CP,$(APK_SRC),$(APK_DST))
	@echo "APK successfully copied to $(APK_DST)"
else
	$(error Android orchestration via this Makefile is configured for Windows hosts)
endif

# Link step
$(TARGET): $(OBJECTS) | bin
	$(CXX) $^ -o $@ $(LDFLAGS) $(SDL_LIBS)

# Compile step
$(BUILD_DIR)/%.o: src/%.cpp | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP $(PLATFORM_FLAGS) $(SDL_CFLAGS) -c $< -o $@

bin:
	@$(call MKDIR,$@)

$(BUILD_DIR):
	@$(call MKDIR,$@)

run: all
	$(RUN)

clean:
	@$(call RMDIR,build)
	@$(call RMDIR,bin)

-include $(DEPS)

.PHONY: all windows linux android run clean
