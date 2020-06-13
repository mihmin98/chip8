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
TEST_SRC_DIR	:= $(TEST)/src
TEST_OBJ_DIR 	:= $(TEST)/obj
TEST_BUILD_DIR 	:= $(TEST)/build
TEST_MAIN		:= $(TEST_SRC_DIR)/tests-main.cpp
TEST_SRC 		:= $(wildcard $(TEST_SRC_DIR)/*.cpp)
TEST_SRC		:= $(filter-out $(TEST_MAIN), $(TEST_SRC))

TEST_OBJECTS	:= $(TEST_MAIN:%.cpp=%.o)
TEST_OBJECTS	:= $(patsubst $(TEST_SRC_DIR)/%,$(TEST_OBJ_DIR)/%,$(TEST_OBJECTS))

TEST_EXECS 		:= $(TEST_SRC:%.cpp=%.out)
TEST_EXECS		:= $(patsubst $(TEST_SRC_DIR)/%,$(TEST_BUILD_DIR)/%,$(TEST_EXECS))

all: build $(APP_DIR)/$(APPNAME)

echo:
	@echo $(TEST_EXECS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGFS) $(INCLUDE) -c $< -o $@ $(LDFLAGS)

$(APP_DIR)/$(APPNAME): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $(APP_DIR)/$(APPNAME) $^ $(LDFLAGS)

$(TEST_OBJECTS): $(TEST_MAIN)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGFS) $(INCLUDE) -c $< -o $@ $(LDFLAGS)

$(TEST_EXECS): $(TEST_SRC) $(OBJECTS) $(TEST_OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGFS) $(INCLUDE) $< $(TEST_OBJECTS) $(OBJECTS) -o $@  $(LDFLAGS)

.PHONY: all build clean debug release tests

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)

debug: CXXFLAGFS += -DDEBUG -g
debug: all

release: CXXFLAGFS += -O2
release: all

tests: $(TEST_EXECS)
	$(TEST_EXECS)

clean:
	rm $(OBJ_DIR)/*.o $(TEST_OBJ_DIR)/*.o $(TEST_BUILD_DIR)/* $(APP_DIR)/$(APPNAME)
