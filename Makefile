TARGET	=	main

SRC 	=	src
CC		=	gcc
STD 	=	c99
CFLAGS	=	-Wall -Werror -pedantic -std=$(STD) -g -I$(SRC) -pg -fsanitize=undefined
LIBS	=	-lm -lGLEW -lGL -lglfw -llua

BIN		=	bin

SOURCES	=	$(notdir $(wildcard $(SRC)/**/*.c $(SRC)/*.c))
OBJECTS =	$(patsubst %.c, $(BIN)/%.o, $(SOURCES))

all: $(BIN) $(OBJECTS) $(TARGET)

$(BIN):
	@echo " "
	@echo "[Create Folders]"
	[ ! -e $@ ] && mkdir $@
	
$(BIN)/%.o: $(SRC)/%.c
	@echo " "
	@echo "["$^"]"
	$(CC) $(CFLAGS) -c $< -o $@
	@if [ $$? != 0 ]; then rm $@; fi
	
$(BIN)/%.o: $(SRC)/%.h
	@echo " "
	@echo "["$^"]"
	$(CC) $(CFLAGS) -c $? -o $@
	@if [ $$? != 0 ]; then rm $@; fi

$(TARGET): $(OBJECTS)
	@echo " "
	@echo "[Linking]"
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^

clean:
	-rm $(BIN)/*
	-rm $(TARGET)
