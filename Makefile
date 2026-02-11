BINARY = pixeld++
BUILD_DIR = build
COV_DIR = coverage

.PHONY: coverage dynamic static clean

# BUILD 1: Test build with GCOV instrumentation (lcov format)
coverage:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(COV_DIR)
	@cd $(BUILD_DIR) && cmake -DCMAKE_BUILD_TYPE=Coverage .. && make app_test
	@./$(BUILD_DIR)/app_test
	@lcov --capture --directory . --output-file $(COV_DIR)/lcov.info --ignore-errors mismatch
	# Filter out system and library headers to keep the report clean
	@lcov --remove $(COV_DIR)/lcov.info '/usr/*' '*/test/*' '*/crow_all.h' --output-file $(COV_DIR)/lcov.info
	@echo "LCOV report generated at: $(COV_DIR)/lcov.info"

# BUILD 2: Dynamically linked, -O0, Debug symbols
dynamic: clean
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake -DDYNAMIC_DEBUG_BUILD=ON -DCMAKE_BUILD_TYPE=Debug .. && make $(BINARY)

# BUILD 3: Statically linked, -O3, Production Ready
static: clean
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake -DSTATIC_RELEASE_BUILD=ON -DCMAKE_BUILD_TYPE=Release .. && make $(BINARY)

clean:
	rm -rf $(BUILD_DIR) $(COV_DIR)
