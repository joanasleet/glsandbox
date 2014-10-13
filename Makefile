TARGET	=	main

SRC 	=	src
CC		=	gcc
STD 	=	gnu99
CFLAGS	=	-Wall -pedantic -std=$(STD) -g -I$(SRC)
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
	$(CC) $(CFLAGS) $(LIBS) -c $? -o $@

$(BIN)/%.o: $(SRC)/%.h
	@echo " "
	@echo "["$^"]"
	$(CC) $(CFLAGS) $(LIBS) -c $? -o $@

$(TARGET): $(OBJECTS)
	@echo " "
	@echo "[Linking]"
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^

clean:
	-rm $(BIN)/*
	-rm $(TARGET)
