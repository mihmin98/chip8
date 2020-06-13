CXX 			:= g++
CXXFLAGFS 		:= -std=c++11 -Wall -Wextra -Werror
LDFLAGS 		:= -lSDL2main -lSDL2
BUILD 			:= ./build
OBJ_DIR 		:= $(BUILD)/obj
APP_DIR 		:= $(BUILD)/app
APPNAME 		:= chip8
INCLUDE 		:= -Iinclude
SRC				:= $(wildcard src/*.cpp)

OBJECTS			:= $(SRC:%.cpp=$(OBJ_DIR)/%.o)

TEST 			:= ./tests
TEST_OBJ_DIR 	:= $(TEST)/obj 
TEST_BUILD_DIR 	:= $(TEST)/build
TEST_SRC 		:= $(wildcard tests/src/*.cpp)

all: build $(APP_DIR)/$(APPNAME)

$(OBJ_DIR)./%.o: %.cpp
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
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(APP_DIR)/*
	-@rm -rvf $(TEST_OBJ_DIR)/*
	-@rm -rvf $(TEST_BUILD_DIR)/*

