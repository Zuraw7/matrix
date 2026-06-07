CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++17

INC_DIR = includes

DOXYGEN   = doxygen
DOC_DIR   = docs
DOC_INDEX = $(DOC_DIR)/html/index.html

TEST_DIR  = tests
BIN_DIR   = bin
TEST_SRCS = $(shell find $(TEST_DIR) -name '*.cpp')
TEST_BINS = $(patsubst $(TEST_DIR)/%.cpp, $(BIN_DIR)/%, $(TEST_SRCS))

all: $(TEST_BINS) $(DOC_INDEX)

# Header-only library: nothing to compile into an archive.
# Documentation index is (re)generated through the docs target.
$(DOC_INDEX):
	@$(MAKE) --no-print-directory docs

# Build and run every test executable (each test file has its own main).
test: $(TEST_BINS)
	@for bin in $(TEST_BINS); do \
		echo ""; \
		echo "===== $$bin ====="; \
		./$$bin; \
	done

$(BIN_DIR)/%: $(TEST_DIR)/%.cpp
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -I $(INC_DIR) $< -o $@

clean:
	rm -rf $(BIN_DIR)

fclean: clean

re: fclean all

docs:
	@if ! command -v $(DOXYGEN) >/dev/null 2>&1; then \
		echo "doxygen not found - documentation not generated"; \
		echo "  install with: sudo dnf install doxygen graphviz"; \
	elif command -v dot >/dev/null 2>&1; then \
		$(DOXYGEN) Doxyfile && \
		echo "Documentation generated in $(DOC_INDEX)"; \
	else \
		echo "graphviz (dot) not found - generating documentation without graphs"; \
		( cat Doxyfile; echo "HAVE_DOT=NO" ) | $(DOXYGEN) - && \
		echo "Documentation generated in $(DOC_INDEX)"; \
	fi

docs-clean:
	rm -rf $(DOC_DIR)

.PHONY: all test clean fclean re docs docs-clean
