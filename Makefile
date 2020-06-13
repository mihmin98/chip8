CXX 			:= g++
CXXFLAGFS 		:= -std=c++11 -Wall -Wextra -Werror
LDFLAGS 		:= -lSDL2main -lSDL2
BUILD 			:= ./build
OBJ_DIR 		:= $(BUILD)/obj
APP_DIR 		:= $(BUILD)/app
APPNAME 		:= chip8
INCLUDE 		:= -Iinclude
SRC_DIR			:= ./src
SRC				:= $(wildcard $(SRC_DIR)/*.cpp)

OBJECTS			:= $(SRC:%.cpp=%.o)
OBJECTS			:= $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%,$(OBJECTS))

TEST 			:= ./tests
TEST_OBJ_DIR 	:= $(TEST)/obj 
TEST_BUILD_DIR 	:= $(TEST)/build
TEST_SRC 		:= $(wildcard tests/src/*.cpp)

all: build $(APP_DIR)/$(APPNAME)

echo:
	@echo $(OBJECTS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGFS) $(INCLUDE) -c $< -o $@ $(LDFLAGS)

$(APP_DIR)/$(APPNAME): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $(APP_DIR)/$(APPNAME) $^ $(LDFLAGS)

.PHONY: all build clean debug release test

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)

debug: CXXFLAGFS += -DDEBUG -g
debug: all

release: CXXFLAGFS += -O2
release: all

clean:
	rm $(OBJ_DIR)/*.o $(TEST_OBJ_DIR)/*.o $(TEST_BUILD_DIR)/* $(APP_DIR)/$(APPNAME)
