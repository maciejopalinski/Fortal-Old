TARGET := ./bin/fortal
TEST_TARGET := ./bin/test

SRC_DIR := src
TEST_DIR := test
INC_DIR := include
BIN_DIR := bin
OBJ_DIR := obj
DEP_DIR := deps
TMP_DIR := tmp

CXX := g++
CXXFLAGS := -O2 -std=c++17 -Wall -Wextra -pedantic -Wno-missing-field-initializers -g
LIBS := -I$(INC_DIR)
LDFLAGS :=
ARGS := -D test/fortal/*.f

CPP_SRC_FILES := $(shell find $(SRC_DIR) -type f -name '*.cpp')
CPP_OBJ_FILES := $(patsubst %, $(OBJ_DIR)/%.o, $(CPP_SRC_FILES))
CPP_HEADERS := $(shell find $(SRC_DIR) -type f -name '*.h')

GTEST_LDFLAGS := -lgtest -pthread
TEST_SRC_FILES := $(shell find $(TEST_DIR) -type f -name '*.cpp')
TEST_OBJ_FILES := $(patsubst %, $(OBJ_DIR)/%.o, $(TEST_SRC_FILES))

all: $(TARGET) $(TEST_TARGET)

run: $(TARGET)
	$(TARGET) $(ARGS)

test: $(TEST_TARGET)
	$(TEST_TARGET)

format: $(CPP_SRC_FILES) $(TEST_SRC_FILES) $(CPP_HEADERS)
	astyle --project=.astylerc $^

$(TARGET): $(CPP_OBJ_FILES)
	@echo Linking $@
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) $(LIBS) -o $@ $^ $(LDFLAGS)

$(TEST_TARGET): $(TEST_OBJ_FILES)
	@echo Linking $@
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) $(LIBS) -o $@ $^ $(LDFLAGS) $(GTEST_LDFLAGS)

$(OBJ_DIR)/%.o: % $(DEP_DIR)/%.d
	@echo Compiling $*
	@mkdir -p $(dir $@)
	@mkdir -p $(dir $(DEP_DIR)/$*.d)
	@$(CXX) -MT $@ -MMD -MP -MF $(DEP_DIR)/$*.d $(CXXFLAGS) $(LIBS) -c -o $@ $<

memcheck: $(TMP_DIR)/memcheck.out
	grep -q "no leaks are possible" $<
	rm $(TMP_DIR)/memcheck.out

$(TMP_DIR)/memcheck.out: $(TARGET)
	@mkdir -p $(TMP_DIR)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=$(TMP_DIR)/memcheck.out $(TARGET) $(ARGS)

cppcheck:
	cppcheck $(SRC_DIR) --std=c++17 --enable=all --suppress=noExplicitConstructor --suppress=useStlAlgorithm --suppress=wrongPrintfScanfArgNum --suppress=cstyleCast --force -q

cppclean:
	cppclean $(SRC_DIR)





clean:
	@echo Cleaning
	@rm -rf $(BIN_DIR) $(OBJ_DIR) $(TMP_DIR)
	@find . -type f -name '*.tmp' -exec rm -f {} \;

depclean:
	@echo Cleaning dependencies
	@rm -rf $(DEP_DIR)

clean-all: clean depclean

DEP_FILES := $(patsubst $(OBJ_DIR)/%.o, $(DEP_DIR)/%.d, $(CPP_OBJ_FILES) $(TEST_OBJ_FILES))
$(DEP_FILES):

include $(wildcard $(DEP_FILES))