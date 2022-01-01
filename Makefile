TARGET := ./bin/fortal
TEST_TARGET := ./bin/test
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -Wno-missing-field-initializers -g
LDFLAGS := 
ARGS := -D test/fortal/*.f

SRC_DIR := src
TEST_DIR := test
BIN_DIR := bin
OBJ_DIR := obj
DEP_DIR := deps
TMP_DIR := tmp

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
	@$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(TEST_TARGET): $(TEST_OBJ_FILES)
	@echo Linking $@
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(GTEST_LDFLAGS)

$(OBJ_DIR)/%.o: % $(DEP_DIR)/%.d
	@echo Compiling $*
	@mkdir -p $(dir $@)
	@mkdir -p $(dir $(DEP_DIR)/$*.d)
	@$(CXX) -MT $@ -MMD -MP -MF $(DEP_DIR)/$*.d $(CXXFLAGS) -c -o $@ $<

memcheck: $(TMP_DIR)/memcheck.out
	grep -q "no leaks are possible" $<
	rm $(TMP_DIR)/memcheck.out

$(TMP_DIR)/memcheck.out: $(TARGET)
	@mkdir -p $(TMP_DIR)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=$(TMP_DIR)/memcheck.out $(TARGET) $(ARGS)

massif: $(TMP_DIR)/massif.out

$(TMP_DIR)/massif.out: $(TARGET)
	@mkdir -p $(TMP_DIR)
	valgrind --tool=massif --massif-out-file=$(TMP_DIR)/massif.out $(TARGET) $(ARGS)

ms_print: $(TMP_DIR)/ms_print.out

$(TMP_DIR)/ms_print.out: $(TMP_DIR)/massif.out
	ms_print $(TMP_DIR)/massif.out > $(TMP_DIR)/ms_print.out

clean:
	@echo Cleaning
	@rm -rf $(BIN_DIR) $(OBJ_DIR) $(DEP_DIR) $(TMP_DIR)
	@find . -type f -name '*.tmp' -exec rm -f {} \;

DEP_FILES := $(patsubst $(OBJ_DIR)/%.o, $(DEP_DIR)/%.d, $(CPP_OBJ_FILES) $(TEST_OBJ_FILES))
$(DEP_FILES):

include $(wildcard $(DEP_FILES))