CC      = clang
LDFLAGS = -lblackbox

SRC = main.c pt1.c pt2.c
OUT = main
BIN = $(OUT)

# Default: NORMAL run — no debug, no trace
LOG_LEVELS ?= -debug,-trace

all: $(BIN)

$(BIN): $(SRC)
	@$(CC) $^ $(LDFLAGS) -o $@
	@echo " - make run"
	@echo " - make run-info"
	@echo " - make run-debug"
	@echo " - make run-fatal"

run: $(BIN)
	@LOG_LEVELS="$(LOG_LEVELS)" ./$(BIN) $(ARGS)

# Normal mode (explicit)
run-info:    ; @$(MAKE) run LOG_LEVELS=-debug,-trace

# Debug mode = ALL
run-debug:   ; @$(MAKE) run LOG_LEVELS=ALL

# Fatal/error only
run-fatal:   ; @$(MAKE) run LOG_LEVELS=+fatal,+error

clean:
	@echo "Cleaning up/"
	@rm -f $(BIN)

.PHONY: all run run-info run-debug run-fatal clean

