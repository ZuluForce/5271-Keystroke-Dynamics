CC=g++

SRCDIR=src
INCDIR=include
OBJDIR=obj
LIBDIR=lib
BINDIR=bin

INTERCEPT_LIB=interception
CFLAGS=-O1 -std=c++0x -Wall
LFLAGS=-lboost_system -lboost_thread -lboost_chrono

EXPORT_FLAG=-W1,--export-dynamic

# For exporting the boost::python libraries
PYTHON_VER=2.7
PYTHON_VER_WIN=32

PYTHON=/usr/include/python$(PYTHON_VER)
BOOST_INC=/usr/include
BOOST_LIB=/usr/lib

PYTHON_OBJ=$(OBJDIR)/python

PYTHON_OUT_LIB=lib/python
PYTHON_LIB=boost_python

SYS_LIBDIR=/usr/local/lib

CPY_CMD=cp -v
ROOT_CPY_CMD=sudo $(CPY_CMD)

## Names of individual python modules as defined in C++ under BOOST_PYTHON_MODULE
## It is importatnt that the final name be the same as defined here because python
## will look for an init method
PY_SCANCODES=DittoPyScancodes

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
vpath %.cpp $(SRCDIR)/python

vpath %.h $(INCDIR)
vpath %.h $(INCDIR)/loaders
vpath %.h $(INCDIR)/enforcers
vpath %.h $(INCDIR)/util
vpath %.h $(INCDIR)/python

vpath %.o $(OBJDIR)
vpath %.o $(PYTHON_OBJ)

vpath %.so %(LIBDIR)
vpath %.so $(PYTHON_OUT_LIB)

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
	$(CC) -I$(INCDIR) $^ -o $(BINDIR)/generators/$@ $(CFLAGS) -lboost_system


$(PYTHON_OUT_LIB)/%.so:
	$(CC) -shared $(PYTHON_OBJ)/$*.o \
	-L$(BOOST_LIB) -l$(PYTHON_LIB) -L/usr/lib/python$(PYTHON_VER)/config \
	-lpython$(PYTHON_VER) -o $@

$(PYTHON_OBJ)/$(PY_SCANCODES).o: python_scancodes.cpp profile_enforcer.h
	$(CC) -fPIC -I$(PYTHON) -I$(INCDIR) -I$(BOOST_INC) -c $< -o $@

python_scan_codes: $(PYTHON_OBJ)/$(PY_SCANCODES).o $(PYTHON_OUT_LIB)/$(PY_SCANCODES).so
	@echo "Finished building scancodes enum for python\n"

all_python: python_scan_codes
	$(ROOT_CPY_CMD) $(EXPORT_FLAG) $(PYTHON_OUT_LIB)/*.so $(SYS_LIBDIR)/
	@echo "\nFinished building all C++ code exported for python\n"

all_python_win:
	make PYTHON_VER=$(PYTHON_VER_WIN) PYTHON=C:\Python$(PYTHON_VER_WIN)\include \
	SYS_LIBDIR=C:\Python$(PYTHON_VER_WIN)\DLLs CPY_CMC=COPY ROOT_CPY_CMD=COPY \
	BOOST_INC=C:\MinGW\include BOOST_LIB=C:\MinGW\lib PYTHON_LIB=boost_python3 \
	EXPORT_FLAG= all_python

.PHONY: clean
.PHONY: clean_python
clean:
	rm -f $(OBJDIR)/*.o
	rm -f $(BINDIR)/*.exe

clean_python:
	rm -f $(PYTHON_OUT_LIB)/*.so
	rm -f $(PYTHON_OBJ)/*.o
