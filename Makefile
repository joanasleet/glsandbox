TARGET	=	main

SRC 	=	src
CC		=	clang
STD 	=	c11
LIBS	=	-lm -lGLEW -lGL -lglfw -llua
LIBD	=	libs
TPLS	=	Newton
NOWARN	=	-Wno-gnu-empty-struct -Wno-disabled-macro-expansion -Wno-c++-compat -Wno-unused-parameter
CHECK	=	-fsanitize=undefined
CFLAGS	=	-Weverything -pedantic $(NOWARN) -std=$(STD) -g -pg -I$(LIBD) -I$(SRC) $(CHECK)

BIN		=	bin
PCBIN	=	staticbin

SOURCES	=	$(notdir $(wildcard $(SRC)/**/*.c $(SRC)/*.c))
OBJECTS =	$(patsubst %.c, $(BIN)/%.o, $(SOURCES))

all: $(BIN) $(OBJECTS) $(TARGET)

$(BIN):
	@echo " "
	@echo "[Create Folders]"
	[ ! -e $@ ] && mkdir $@ && mkdir $(PCBIN)
	
$(BIN)/%.o: $(SRC)/%.c $(SRC)/%.h
	@echo " "
	@echo "["$^"]"
	$(CC) $(CFLAGS) -c $< -o $@
	@if [ $$? != 0 ]; then rm $@; fi
	
$(TARGET): $(OBJECTS)
	@echo " "
	@echo "[Linking]"
	$(CC) $(CFLAGS) -L$(LIBD)/$(TPLS) -l$(TPLS) $(LIBS) -o $@ $^ $(PCBIN)/*.o

clean:
	-rm $(BIN)/*
	-rm $(TARGET)
