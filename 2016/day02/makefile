CC      = clang -g
LDFLAGS = -lblackbox

SRC = main.c pt1.c pt2.c
OUT = main
BIN = $(OUT)
FILE = input.in
TEST = test.in

# Default: NORMAL run — no debug, no trace
LOG_LEVELS ?= -debug,-trace

all: $(BIN)

$(BIN): $(SRC)
	@$(CC) $^ $(LDFLAGS) -o $@

run: $(BIN)
	@LOG_LEVELS="$(LOG_LEVELS)" ./$(BIN) $(FILE)

# Same only with tests
test : $(BIN)
	@LOG_LEVELS="$(LOG_LEVELS)" ./$(BIN) $(TEST)

clean:
	@echo "Cleaning up/"
	@rm -f $(BIN)
	@rm -rf $(OUT).dSYM*

.PHONY: all run test clean
