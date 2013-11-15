CC=gcc
CFLAGS=-Wall
#PREFIX=src
#TARGETS=$(addprefix $(PREFIX)/, main.c interpreter.c command.c graphics.c)
OBJDIR=obj
TARGETS=$(addprefix $(OBJDIR)/, main.o interpreter.o command.o graphics.o \
                                color.o convert.o)
LIBFLAGS=-lreadline -lGL -lglut
OUTPUT=roloc

VPATH= %.c src

$(OUTPUT): $(TARGETS)
	$(CC) $(CFLAGS) $(TARGETS) $(LIBFLAGS) -o $(OUTPUT)

$(OBJDIR)/%.o: %.c
	mkdir -p $(OBJDIR)
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm roloc
	rm obj/*.o
