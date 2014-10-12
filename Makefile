TARGET	=	main

SRC 	=	src
CC		=	gcc
CFLAGS	=	-Wall -pedantic -std=c99 -g -I$(SRC)
LIBS	=	-lm


BIN		=	bin

SOURCES	=	$(notdir $(wildcard $(SRC)/**/*.c $(SRC)/*.c))
OBJECTS =	$(patsubst %.c, $(BIN)/%.o, $(SOURCES))

all: $(BIN) $(OBJECTS) $(TARGET)

$(BIN):
	@echo "–––––– Create Folders ––––––"
	[ ! -e $@ ] && mkdir $@
	
$(BIN)/%.o: $(SRC)/%.c
	@echo "–––––– ["$^"] ––––––"
	$(CC) $(CFLAGS) $(LIBS) -c $? -o $@

$(BIN)/%.o: $(SRC)/%.h
	@echo "–––––– ["$^"] ––––––"
	$(CC) $(CFLAGS) $(LIBS) -c $? -o $@

$(TARGET): $(OBJECTS)
	@echo "–––––– [Linking] ––––––"
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^

clean:
	rm $(BIN)/*