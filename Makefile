SRC_DIR := ./src
OBJ_DIR := ./obj
BIN_DIR := ./bin
PROGRAM := siweb
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
LDFLAGS :=
CPPFLAGS :=
CXXFLAGS := -MMD
-include $(OBJ_FILES:.o=.d)

$(PROGRAM): $(OBJ_FILES)
	g++ $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	g++ $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ_FILES)
	rm -f $(OBJ_FILES:.o=.d)
	rm -f $(PROGRAM)