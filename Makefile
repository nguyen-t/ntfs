# Edit to fit needs
HDREXT=.h
SRCEXT=.c
OBJEXT=.o
CC=gcc
LIBS=
SANS=undefined,address,leak
WARNS=all pedantic extra
OUTPUT=
EXEC=exec
ARGS=

# Shouldn't really be touched
HDRDIR=include
SRCDIR=src
OBJDIR=objects
TSTDIR=test
DEPS=$(basename $(shell ls $(HDRDIR)))
INPUTS=$(basename $(shell ls $(SRCDIR)))
HEADERS=$(addprefix $(HDRDIR)/, $(addsuffix $(HDREXT), $(DEPS)))
SOURCES=$(addprefix $(SRCDIR)/, $(addsuffix $(SRCEXT), $(INPUTS)))
OBJECTS=$(addprefix $(OBJDIR)/, $(addsuffix $(OBJEXT), $(INPUTS)))
CFLAGS=$(addprefix -W, $(WARNS)) $(addprefix -l, $(LIBS)) -I$(HDRDIR) -c -o
LDFLAGS=-fsanitize=$(SANS) -o

.PHONY: all
.PHONY: run
.PHONY: clean

all: | $(HDRDIR) $(SRCDIR) $(OBJDIR) $(TSTDIR) $(OUTPUT)

run: $(OUTPUT)
	$(EXEC) ./$(OUTPUT) $(ARGS)

clean:
	rm $(OBJECTS) $(OUTPUT)

$(OUTPUT): $(OBJECTS)
	$(CC) $(LDFLAGS) $@ $^

$(OBJDIR)/%$(OBJEXT): $(SRCDIR)/%$(SRCEXT) $(HEADERS)
	$(CC) $(CFLAGS) $@ $<

$(HDRDIR) $(SRCDIR) $(OBJDIR) $(TSTDIR): % :
	mkdir $@
