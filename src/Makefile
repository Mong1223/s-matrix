CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -I.
LDFLAGS = -lm
CHECK_FLAGS = $(shell pkg-config --libs check)

SRC_DIR = .
TEST_DIR = tests
BUILD_DIR = build

LIBRARY = s21_matrix.a
TEST_EXEC = $(BUILD_DIR)/unittest

SOURCES = $(wildcard $(SRC_DIR)/*.c)
TEST_SOURCES = $(wildcard $(TEST_DIR)/*.c)

OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
TEST_OBJECTS = $(TEST_SOURCES:$(TEST_DIR)/%.c=$(BUILD_DIR)/%.o)

all: $(LIBRARY)

$(BUILD_DIR):
	@mkdir -p $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@echo "Compiling: $<"
	$(CC) $(CFLAGS) -c $< -o $@

# Build test object files  
$(BUILD_DIR)/%.o: $(TEST_DIR)/%.c | $(BUILD_DIR)
	@echo "Compiling test: $<"
	$(CC) $(CFLAGS) -c $< -o $@

# Create static library
$(LIBRARY): $(OBJECTS)
	@echo "Creating library: $@"
	ar rcs $@ $^

# Build and run tests
test: $(TEST_EXEC)
	@echo "Running tests..."
	./$(TEST_EXEC)

# Build test executable
$(TEST_EXEC): $(LIBRARY) $(TEST_OBJECTS) | $(BUILD_DIR)
	@echo "Linking tests..."
	$(CC) $(CFLAGS) $(TEST_OBJECTS) -L. $(LIBRARY) $(CHECK_FLAGS) $(LDFLAGS) -o $@

# Coverage report
gcov_report:
	@echo "Building with coverage..."
	$(CC) $(CFLAGS) --coverage $(SOURCES) $(TEST_SOURCES) -o unittest $(CHECK_FLAGS) $(LDFLAGS)
	./unittest
	lcov -t "coverage" -b . --no-external -o coverage.info -c -d . --exclude "*/tests/*"
	genhtml -o report coverage.info
	@echo "Coverage report: report/index.html"
	@rm -f unittest coverage.info *.gcda *.gcno

# Code style check
clang_check:
	@cp ../materials/linters/.clang-format .clang-format
	@clang-format -n *.c
	@clang-format -n *.h
	@clang-format -n */*.c
	@rm .clang-format

clang:
	@cp ../materials/linters/.clang-format .clang-format
	@clang-format -i *.c
	@clang-format -i *.h
	@clang-format -i */*.c
	@rm -rf tests/*temp*
	@rm .clang-format

# Static analysis
cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem $(SRC_DIR) $(TEST_DIR)

# Memory check
memcheck: $(TEST_EXEC)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TEST_EXEC)

# Clean build artifacts
clean:
	@echo "Cleaning..."
	@rm -rf $(BUILD_DIR) $(LIBRARY) report
	@find . -name '*.gcov' -delete
	@find . -name '*.gcda' -delete  
	@find . -name '*.gcno' -delete
	@rm -f valgrind_report.log
	@echo "OK"

.PHONY: all test gcov_report clang_check clang cppcheck memcheck clean