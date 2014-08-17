PROGRAM = expr

CC = gcc
CFLAGS = -Wall

LINKER = gcc
LFLAGS = -Wall 

BINDIR = bin
SOURCESDIR = src
INCLUDEDIR = $(SOURCESDIR)/include

LIBS = -lm 
SOURCES := $(wildcard $(SOURCESDIR)/*.c)
INCLUDES := $(wildcard $(INCLUDEDIR)/*.h) 
OBJECTS := $(SOURCES:$(SOURCESDIR)/%.c=$(BINDIR)/%.o)

$(PROGRAM): $(OBJECTS)
	$(LINKER) -o $@ $(LIBS) $(LFLAGS) $(OBJECTS)

$(OBJECTS): $(BINDIR)/%.o : $(SOURCESDIR)/%.c
	@$(CC) $(CFLAGS) -I $(INCLUDEDIR) -c $< -o $@

.PHONY: clean
clean:
	@if [ -n "`ls -A $(BINDIR)`" ]; then rm $(BINDIR)/*; fi
	@if [ -f $(PROGRAM) ]; then rm $(PROGRAM); fi
