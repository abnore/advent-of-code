CC      = clang
LDFLAGS = -lblackbox

SRC = main.c pt1.c pt2.c
OUT = main
BIN = $(OUT)

LOG_LEVELS ?= ALL

all: $(BIN)

$(BIN): $(SRC)
	@$(CC) $^ $(LDFLAGS) -o $@
	@echo " - make run "
	@echo " - make run-info "
	@echo " - make run-debug "
	@echo " - make run-fatal "
run: $(BIN)
	@LOG_LEVELS="$(LOG_LEVELS)" ./$(BIN) $(ARGS)

run-info:  ;  @$(MAKE) run LOG_LEVELS=+info,+warn
run-debug: ;  @$(MAKE) run LOG_LEVELS=+debug,-trace
run-fatal: ;  @$(MAKE) run LOG_LEVELS=+fatal

clean:
	@echo "Cleaning up/"
	@rm -f $(BIN)

.PHONY: all run run-info run-debug run-fatal clean

