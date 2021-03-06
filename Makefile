CC=gcc
CFLAGS=-Wall -DRELEASE=\"$(RELEASE)\"
#PREFIX=src
#TARGETS=$(addprefix $(PREFIX)/, main.c interpreter.c command.c graphics.c)
OBJDIR=obj
TARGETS=$(addprefix $(OBJDIR)/, main.o interpreter.o command.o graphics.o \
                                color.o convert.o calculate.o parser.o \
                                shaderutils.o)
LIBFLAGS=-lreadline -lcairo -lGLEW -lGL lib/libglfw3.a -lglfw -lm -lpthread \
         -lX11 -lXxf86vm -lXi -lXrandr
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
