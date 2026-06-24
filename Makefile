# Shrodyne build wrapper
# Thin convenience layer over the real build system (CMake + FetchContent).
# Adjust BIN_NAME / GENERATOR to match your CMakeLists.txt target name.

BUILD_DIR   := build
BUILD_TYPE  ?= Debug
GENERATOR   ?= Ninja
JOBS        ?= $(shell nproc)
BIN_NAME    := shrodyne

.PHONY: all configure build run clean rebuild debug release \
        compile_commands format help

all: build

## Configure CMake into $(BUILD_DIR) (only re-runs if cache is stale)
configure: $(BUILD_DIR)/CMakeCache.txt

$(BUILD_DIR)/CMakeCache.txt: CMakeLists.txt
	cmake -S . -B $(BUILD_DIR) \
		-G "$(GENERATOR)" \
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE) \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=ON

## Build the project
build: configure
	cmake --build $(BUILD_DIR) -j$(JOBS)

## Build then run the binary
run: build
	./$(BUILD_DIR)/$(BIN_NAME)

## Force a Debug build
debug:
	$(MAKE) BUILD_TYPE=Debug build

## Force a Release build
release:
	$(MAKE) BUILD_TYPE=Release build

## Wipe the build directory entirely
clean:
	rm -rf $(BUILD_DIR)

## Clean + full rebuild
rebuild: clean build

## Symlink compile_commands.json to project root for clangd/editor tooling
compile_commands: configure
	ln -sf $(BUILD_DIR)/compile_commands.json compile_commands.json

## Format all C++ sources with clang-format (expects .clang-format in root)
format:
	find src include -regex '.*\.\(cpp\|hpp\|h\)$$' -exec clang-format -i {} +

help:
	@echo "Targets: build run debug release clean rebuild compile_commands format"
