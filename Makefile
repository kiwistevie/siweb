# Modules
MODULES := middlewares routing parsing encoding utils

# Directories
SRC_DIR := ./src
OBJ_DIR := ./obj
BIN_DIR := ./bin

# Source and Object files
SRC_FILES := $(shell find $(SRC_DIR) -name *.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
OBJ_DIRS  := $(patsubst %, $(OBJ_DIR)/%, $(MODULES))

# Compiler and Linker configuration
COMPILER := g++
LINKER   := g++
LDFLAGS  :=
CPPFLAGS := -DDEBUG -g
CXXFLAGS := -MMD
ARFLAGS  :=
-include $(OBJ_FILES:.o=.d)

# Executable
PROGRAM := siweb
PORT := 8080

# Rules
build: $(OBJ_DIRS) $(PROGRAM)
	echo $^

$(OBJ_DIRS):
	mkdir -p $@

$(PROGRAM): $(OBJ_FILES)
	$(LINKER) -std=c++17 $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(COMPILER) -std=c++17 $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ_FILES)
	rm -f $(OBJ_FILES:.o=.d)
	rm -f $(PROGRAM)
	rm -r obj

run: build
	./$(PROGRAM) $(PORT)

library: $(PROGRAM)
	ar rvs $<.a $(OBJ_FILES)