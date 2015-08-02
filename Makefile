# executable
MAIN	:=	main

# folders
SRC 	:=	src
BIN		:=	bin

# compiler
CC		:=	gcc
STD 	:=	-std=gnu11
DEBUG	:=	-g -pg
LIBS	:=	-lm -lGLEW -lGL -lglfw -llua
LIBD	:=	libs
TPLS	:=	Newton
#NOWARN	:=	-Wno-gnu-empty-struct -Wno-disabled-macro-expansion -Wno-c++-compat -Wno-unused-parameter
CHECK	:=	-fsanitize=undefined
#PENDAN	:=	-pedantic
WALL 	:= 	-Wall
CFLAGS	:=	$(WALL) $(PENDAN) $(NOWARN) $(STD) $(DEBUG) -I$(LIBD) -I$(SRC) $(CHECK)

# files
SOURCES	:=	$(notdir $(wildcard $(SRC)/**/*.c $(SRC)/*.c))
OBJS	:=	$(patsubst %.c, $(BIN)/%.o, $(SOURCES))
DEPS	:=	$(sort $(patsubst $(notdir %), %.deps, $(OBJS)))

all: $(BIN) $(OBJS) $(MAIN)

$(MAIN): $(OBJS)
	@echo " "
	@echo "[Linking]"
	$(CC) $(CFLAGS) -L$(LIBD)/$(TPLS) -l$(TPLS) $(LIBS) -o $@ $^

$(BIN):
	@echo " "
	@echo "[Create Folders]"
	[ ! -e $@ ] && mkdir $@

-include $(DEPS)

$(DEPS):
	@true

clean:
	@echo "[Clean]"
	rm -f $(BIN)/*

$(BIN)/%.o: $(SRC)/%.c
	@echo " "
	@echo "["$@"]"
	$(CC) $(CFLAGS) -c -o $@ $< -MMD -MF $@.deps

