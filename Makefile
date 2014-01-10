CC=gcc
CFLAGS=-g -Wall -DRELEASE=\"$(RELEASE)\"
#PREFIX=src
#TARGETS=$(addprefix $(PREFIX)/, main.c interpreter.c command.c graphics.c)
OBJDIR=obj
TARGETS=$(addprefix $(OBJDIR)/, main.o interpreter.o command.o graphics.o \
                                color.o convert.o calculate.o parser.o)
LIBFLAGS=-lreadline -lGL -lglut -pthread
RELEASE=$(shell git describe --abbrev=0)
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
