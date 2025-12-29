CC      = clang
LDFLAGS = -lblackbox -Wno-deprecated -Wall -Wextra

SRC = main.c pt1.c pt2.c
OUT = main
BIN = $(OUT)

# Default: NORMAL run — no debug, no trace
LOG_LEVELS ?= -debug,-trace

all: $(BIN)

$(BIN): $(SRC)
	@$(CC) $^ $(LDFLAGS) -o $@
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

clean:
	@echo "Cleaning up/"
	@rm -f $(BIN)

.PHONY: all run run-info run-debug run-fatal clean
