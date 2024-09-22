# Compiler
CXX = g++
CXXFLAGS = -std=c++11 -Wall -I /usr/local/include/eigen3

# Directories
INCDIR = include
SRCDIR = src
OBJDIR = obj

# Files
SRCFILES = $(SRCDIR)/mpc.cpp $(SRCDIR)/main.cpp
OBJFILES = $(OBJDIR)/mpc.o $(OBJDIR)/main.o
TARGET = mpc.so

# Ensure the object directory exists and compile target
$(TARGET): $(OBJFILES)
	$(CXX) $(OBJFILES) -o $(TARGET)

# Create object directory if not exists and compile source files into objects
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -I$(INCDIR) -c $< -o $@

# Create object directory
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Clean up
clean:
	rm -rf $(OBJDIR) $(TARGET)
