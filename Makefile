HDRDIR=include
SRCDIR=src
OBJDIR=objects
TSTDIR=test
DEPS=$(basename $(shell ls $(HDRDIR)))
INPUTS=$(basename $(shell ls $(SRCDIR)))
HEADERS=$(addprefix $(HDRDIR)/, $(addsuffix .h, $(DEPS)))
SOURCES=$(addprefix $(SRCDIR)/, $(addsuffix .c, $(INPUTS)))
OBJECTS=$(addprefix $(OBJDIR)/, $(addsuffix .o, $(INPUTS)))
CC=gcc
CFLAGS=-c -Wall -Wextra -O2 -I$(HDRDIR) -o
LDFLAGS=-I$(HDRDIR) -o
OUTPUT=
EXEC=exec
ARGS=

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

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) $@ $<

$(HDRDIR) $(SRCDIR) $(OBJDIR) $(TSTDIR): % :
	mkdir $@
