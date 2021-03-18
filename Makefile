SRC_DIR := ./src
OBJ_DIR := ./obj
BIN_DIR := ./bin
PROGRAM := siweb
PORT := 8080
SRC_FILES := $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.cpp))
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
LDFLAGS :=
CPPFLAGS := -DDEBUG -g
CXXFLAGS := -MMD
ARFLAGS  :=
-include $(OBJ_FILES:.o=.d)

$(PROGRAM): $(OBJ_FILES)
	g++ -std=c++17 $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	g++ -std=c++17 $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ_FILES)
	rm -f $(OBJ_FILES:.o=.d)
	rm -f $(PROGRAM)

run: $(PROGRAM)
	./$(PROGRAM) $(PORT)

library: $(PROGRAM)
	ar rvs $<.a $(OBJ_FILES)