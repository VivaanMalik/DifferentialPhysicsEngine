# --- Compiler & Flags ---
CXX      := g++
CXXFLAGS := -std=c++17 -O3 -Iinclude 
LDFLAGS  := -L. -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# --- Directories ---
SRC_DIR   := src
BUILD_DIR := build
TARGET    := $(BUILD_DIR)/diff_engine

# --- Files ---
# Automatically find all .cpp files in src/
SOURCES  := $(wildcard $(SRC_DIR)/*.cpp)
# Map those .cpp files to .o files in the build/ directory
OBJECTS  := $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# --- Rules ---

# Default rule: Build the engine
all: $(TARGET)

# Link the final binary
# The @ before commands hides the raw command from terminal for a cleaner look
$(TARGET): $(OBJECTS)
	@echo "Linking: $@"
	@$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Compile .cpp to .o
# The -MMD flag creates .d files to track header changes automatically
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@echo "Compiling: $<"
	@$(CXX) $(CXXFLAGS) -MMD -c $< -o $@

# Create the build directory if it doesn't exist
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Clean up build artifacts
clean:
	@echo "Cleaning up..."
	@rm -rf $(BUILD_DIR)

# Run the simulation
run: all
	@./$(TARGET)

# Include dependency files (tracks if you change a .hpp file)
-include $(OBJECTS:.o=.d)

.PHONY: all clean run build_dir