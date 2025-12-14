CC      = clang
LDFLAGS = -lblackbox

SRC = main.c pt1.c pt2.c
OUT = main
BIN = $(OUT)
FILE = input.in
TEST = test.in
NEW_TEST = new_test.in

# Default: NORMAL run — no debug, no trace
LOG_LEVELS ?= -debug,-trace

all: $(BIN)

$(BIN): $(SRC)
	@$(CC) $^ $(LDFLAGS) -o $@
	@echo "When testing do "
	@echo " - make test"
	@echo " - make test-debug"
	@echo " - make test-all"
	@echo "Then when ready to go do"
	@echo " - make run"
	@echo " - make run-debug"
	@echo " - make run-all"

run: $(BIN)
	@LOG_LEVELS="$(LOG_LEVELS)" ./$(BIN) $(FILE)

# Debug mode = Add the debug flag (which is the same as removing trace)
run-debug:   ; @$(MAKE) run LOG_LEVELS=-trace

# Show everything
run-all:   ; @$(MAKE) run LOG_LEVELS=ALL

# Same only with tests
test : $(BIN)
	@LOG_LEVELS="$(LOG_LEVELS)" ./$(BIN) $(TEST)
test-debug:   ; @$(MAKE) test LOG_LEVELS=-trace
test-all:   ; @$(MAKE) test LOG_LEVELS=ALL

# Same only with tests
test2 : $(BIN)
	@LOG_LEVELS="$(LOG_LEVELS)" ./$(BIN) $(NEW_TEST)
test2-debug:   ; @$(MAKE) test LOG_LEVELS=-trace
test2-all:   ; @$(MAKE) test LOG_LEVELS=ALL
clean:
	@echo "Cleaning up/"
	@rm -f $(BIN)

.PHONY: all run run-info run-debug run-fatal clean
