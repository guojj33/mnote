CC = gcc
HEADER_DIR = ./include
SRC_DIR = ./src
BIN_DIR = ./bin

INCLUDE = -I $(HEADER_DIR)

OUTS = mnote

all: bin $(addprefix $(BIN_DIR)/, $(OUTS))

install: all
	@cp $(BIN_DIR)/$(OUTS) /usr/bin/$(OUTS)

uninstall:
	@rm -r ~/.mnote
	@rm /usr/bin/$(OUTS)

bin :
	@mkdir -p $(BIN_DIR)

$(BIN_DIR)/%: $(SRC_DIR)/%.c $(HEADER_DIR)/%.h
	@$(CC) $^ -o $@ $(INCLUDE)

clean :
	@rm -rf $(BIN_DIR)