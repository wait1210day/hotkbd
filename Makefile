CC = @gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -Isrc
LDFLAGS =

SRC = src/frameworks.c src/kbindList.c src/kmaps.c src/preprocessor.c src/hotkbd.c src/continuousTri.c src/renderer.c
OBJ = $(SRC:.c=.o)
EXEC = hotkbd

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ) $(LBLIBS)
	@echo " LD      $@"

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $^ -o $@
	@echo " CC      $@"

clean:
	@rm -rf $(OBJ) $(EXEC)
	@echo " CLEAN   $(OBJ)"
	@echo " CLEAN   $(EXEC)"