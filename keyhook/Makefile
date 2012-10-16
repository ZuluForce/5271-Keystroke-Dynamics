CC=g++

SRCDIR=src
INCDIR=include
OBJDIR=obj
LIBDIR=lib
BINDIR=bin

INTERCEPT_LIB=interception
CFLAGS=-O1

## How to build the shared object file such as libinterception.so
##
## gcc -c interception.c -o interception.o
## gcc --shared -o libinterception.so interception.o -lstdc++

vpath %.c $(SRCDIR)
vpath %.cpp $(SRCDIR)
vpath %.cpp $(SRCDIR)/loaders

vpath %.h $(INCDIR)
vpath %.h $(INCDIR)/loaders

vpath %.o $(OBJDIR)

_OBJS= utils.o profile_loader.o known_ploaders.o fs_loader.o
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) -c -I$(INCDIR) -o $@ $< $(CFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/loaders/*.cpp
	$(CC) -c -I$(INCDIR) -o $@ $< $(CFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c -I$(INCDIR) -o $@ $< $(CFLAGS)

x2y: intercept_example.cpp interception.h utils.o
	$(CC) -I$(INCDIR) $^ -o $(BINDIR)/intercept_example libinterception.so

ditto_cap: ditto_capture.cpp interception.h $(OBJS)
	$(CC) -I$(INCDIR) -L$(LIBDIR) $^ -o $(BINDIR)/ditto_cap $(LIBDIR)/libinterception.so

ditto: ditto.cpp ditto.h $(OBJS)
	$(CC) -I$(INCDIR) $^ -o $(BINDIR)/$@

.PHONY: clean
clean:
	rm $(OBJDIR)/*.o
	rm $(BINDIR)/*.exe
