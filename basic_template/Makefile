# Compiler Settings
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -fPIC
LDFLAGS = 

# Eigen3 include path (modify if installed elsewhere)
EIGEN_PATH = /usr/local/include/eigen3
CXXFLAGS += -I$(EIGEN_PATH)

# Directory Structure
INCDIR = include
SRCDIR = src
OBJDIR = obj
BINDIR = bin
LIBDIR = lib

# Source and Object Files
SRCFILES = $(SRCDIR)/mpc.cpp $(SRCDIR)/main.cpp
OBJFILES = $(OBJDIR)/mpc.o $(OBJDIR)/main.o
HEADER_FILES = $(INCDIR)/mpc.h

# Build Targets

# Executable target (default)
EXECUTABLE = $(BINDIR)/mpc

# Library targets
SHARED_LIB = $(LIBDIR)/libmpc.so
STATIC_LIB = $(LIBDIR)/libmpc.a

# Default Target
.PHONY: all
all: $(EXECUTABLE)

# Build Executable
$(EXECUTABLE): $(OBJFILES) | $(BINDIR)
	@echo "[Linking] Creating executable: $@"
	$(CXX) $(CXXFLAGS) $(OBJFILES) -o $@ $(LDFLAGS)
	@echo "[Success] Build complete: $@"

# Build Shared Library
.PHONY: shared
shared: CXXFLAGS += -shared
shared: $(SRCDIR)/mpc.cpp $(HEADER_FILES) | $(LIBDIR)
	@echo "[Building] Creating shared library: $(SHARED_LIB)"
	$(CXX) $(CXXFLAGS) -I$(INCDIR) $(SRCDIR)/mpc.cpp -o $(SHARED_LIB) $(LDFLAGS)
	@echo "[Success] Shared library created: $(SHARED_LIB)"

# Build Static Library
.PHONY: static
static: $(OBJFILES) | $(LIBDIR)
	@echo "[Archiving] Creating static library: $(STATIC_LIB)"
	ar rcs $(STATIC_LIB) $(OBJFILES)
	@echo "[Success] Static library created: $(STATIC_LIB)"

# Compile Source Files to Object Files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(HEADER_FILES) | $(OBJDIR)
	@echo "[Compiling] $<"
	$(CXX) $(CXXFLAGS) -I$(INCDIR) -c $< -o $@

# Create Directories
$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(BINDIR):
	@mkdir -p $(BINDIR)

$(LIBDIR):
	@mkdir -p $(LIBDIR)

# Run the Executable
.PHONY: run
run: $(EXECUTABLE)
	@echo "[Running] Executing MPC example..."
	@./$(EXECUTABLE)

# Clean Build Artifacts
.PHONY: clean
clean:
	@echo "[Cleaning] Removing build artifacts..."
	rm -rf $(OBJDIR) $(BINDIR) $(LIBDIR)
	@echo "[Success] Clean complete"

# Deep Clean (remove all generated files)
.PHONY: distclean
distclean: clean
	@echo "[Deep Clean] Removing all generated files..."
	find . -name "*.o" -delete
	find . -name "*.so" -delete
	find . -name "*.a" -delete
	@echo "[Success] Deep clean complete"

# Debug Info
.PHONY: help
help:
	@echo "============================================"
	@echo "   MPC Project Build System"
	@echo "============================================"
	@echo ""
	@echo "Available targets:"
	@echo "  make              - Build executable (default)"
	@echo "  make shared       - Build shared library (.so)"
	@echo "  make static       - Build static library (.a)"
	@echo "  make run          - Build and run executable"
	@echo "  make clean        - Remove build artifacts"
	@echo "  make distclean    - Remove all generated files"
	@echo "  make help         - Show this help message"
	@echo ""
	@echo "Example usage:"
	@echo "  make              # Build executable to ./bin/mpc"
	@echo "  make run          # Build and run"
	@echo "  make shared       # Build shared library to ./lib/libmpc.so"
	@echo "  make clean        # Clean up object files and binaries"
	@echo ""
	@echo "Directory structure:"
	@echo "  include/          - Header files (.h)"
	@echo "  src/              - Source files (.cpp)"
	@echo "  obj/              - Object files (.o)"
	@echo "  bin/              - Executable output"
	@echo "  lib/              - Library output (.so, .a)"
	@echo "============================================"

# Print Variables (for debugging)
.PHONY: debug
debug:
	@echo "CXX = $(CXX)"
	@echo "CXXFLAGS = $(CXXFLAGS)"
	@echo "EIGEN_PATH = $(EIGEN_PATH)"
	@echo "SRCFILES = $(SRCFILES)"
	@echo "OBJFILES = $(OBJFILES)"
	@echo "EXECUTABLE = $(EXECUTABLE)"
	@echo "SHARED_LIB = $(SHARED_LIB)"
	@echo "STATIC_LIB = $(STATIC_LIB)"

# Phony Targets
.PHONY: .PHONY
