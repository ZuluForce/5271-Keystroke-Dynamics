CC=g++

SRCDIR=src
INCDIR=include
OBJDIR=obj
LIBDIR=lib
BINDIR=bin

INTERCEPT_LIB=interception
CFLAGS=-O1 -std=c++0x -Wall
LFLAGS=-lboost_system -lboost_thread -lboost_chrono

## How to build the shared object file such as libinterception.so
##
## gcc -c interception.c -o interception.o
## gcc --shared -o libinterception.so interception.o -lstdc++

vpath %.c $(SRCDIR)
vpath %.cpp $(SRCDIR)
vpath %.cpp $(SRCDIR)/loaders
vpath %.cpp $(SRCDIR)/generate
vpath %.cpp $(SRCDIR)/enforcers
vpath %.cpp $(SRCDIR)/util

vpath %.h $(INCDIR)
vpath %.h $(INCDIR)/loaders
vpath %.h $(INCDIR)/enforcers
vpath %.h $(INCDIR)/util

vpath %.o $(OBJDIR)

_DITTO_OBJS=profile_loader.o profile_enforcer.o known_ploaders.o profile_enforcer.o\
			fs_loader.o basic_enforcer.o
_UTIL_OBJS=semaphore.o timer.o utils.o

DITTO_OBJS=$(patsubst %,$(OBJDIR)/%,$(_DITTO_OBJS))
UTIL_OBJS=$(patsubst %,$(OBJDIR)/%,$(_UTIL_OBJS))

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) -c -I$(INCDIR) -o $@ $< $(CFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/loaders/%.cpp
	$(CC) -c -I$(INCDIR) -o $@ $< $(CFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/util/%.cpp
	$(CC) -c -I$(INCDIR) -o $@ $< -std=c++0x -O3

$(OBJDIR)/%.o: $(SRCDIR)/enforcers/%.cpp
	$(CC) -c -I$(INCDIR) -L$(LIBDIR) -o $@ $< $(CFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c -I$(INCDIR) -o $@ $< -O3

ditto_cap: ditto_capture.cpp interception.h util.o
	$(CC) -I$(INCDIR) -L$(LIBDIR) $^ -o $(BINDIR)/ditto_cap $(LIBDIR)/libinterception.so

ditto: ditto.cpp ditto.h $(DITTO_OBJS) $(UTIL_OBJS)
	$(CC) -static -I$(INCDIR) -L$(LIBDIR) $^ -o $(BINDIR)/$@ $(CFLAGS) $(LFLAGS) $(LIBDIR)/libinterception.so

gen_fs_loader: gen_fs_loader_profile.cpp fs_loader.h
	$(CC) -I$(INCDIR) $^ -o $(BINDIR)/generators/$@

.PHONY: clean
clean:
	rm $(OBJDIR)/*.o
	rm $(BINDIR)/*.exe
