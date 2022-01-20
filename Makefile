CXX := g++
CXXFLAGS := -std=gnu++11 -Wall

SRC_DIR := ./src
OBJ_DIR := ./obj
$(shell [ -d $(OBJ_DIR) ] || mkdir -p $(OBJ_DIR))

SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)

WX_CONFIG := wx-config

WX_CXXFLAGS := $(shell $(WX_CONFIG) --cxxflags)
WX_LIBS := $(shell $(WX_CONFIG) --libs)

OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))

editor: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -O3 -o $@ $(OBJECTS) $(LDFLAGS) $(WX_LIBS) $(LIBS)
 
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) -O3 -c -o $@ $(WX_CXXFLAGS) $(CXXFLAGS) $<

.PHONY: clean
clean:
	rm -rf obj
