# Edit to fit needs
CC = gcc
LIBS =
DEFINES =
SANS = undefined,address,leak
WARNS = all pedantic extra
OPTIMIZE = -O3
OUTPUT =
EXEC = exec
ARGS =

# Shouldn't really be touched
HDRDIR = include
HDREXT = .h
SRCDIR = src
SRCEXT = .c
OBJDIR = objects
OBJEXT = .o
TSTDIR = test
DEPS = $(basename $(shell ls $(HDRDIR)))
INPUTS = $(basename $(shell ls $(SRCDIR)))
HEADERS = $(addprefix $(HDRDIR)/, $(addsuffix $(HDREXT), $(DEPS)))
SOURCES = $(addprefix $(SRCDIR)/, $(addsuffix $(SRCEXT), $(INPUTS)))
OBJECTS = $(addprefix $(OBJDIR)/, $(addsuffix $(OBJEXT), $(INPUTS)))
CFLAGS = $(addprefix -D, $(DEFINES)) -I$(HDRDIR) -c -o
LDFLAGS = -o

.PHONY: debug
.PHONY: release
.PHONY: build
.PHONY: run
.PHONY: clean

debug: DEFINES := DEBUG $(DEFINES)
debug: CFLAGS := $(addprefix -W, $(WARNS)) $(CFLAGS)
debug: LDFLAGS := -fsanitize=$(SANS) $(LDFLAGS)
debug: build

release: DEFINES := NDEBUG $(DEFINES)
release: CFLAGS := $(OPTIMIZE) $(CFLAGS)
release: LDFLAGS:= $(LDFLAGS)
release: build

build: | $(HDRDIR) $(SRCDIR) $(OBJDIR) $(TSTDIR) $(OUTPUT)

run: $(OUTPUT)
	$(EXEC) ./$(OUTPUT) $(ARGS)

clean:
	rm $(OBJECTS) $(OUTPUT)

$(OUTPUT): $(OBJECTS)
	$(CC) $(LDFLAGS) $@ $^ $(addprefix -l, $(LIBS))

$(OBJDIR)/%$(OBJEXT): $(SRCDIR)/%$(SRCEXT) $(HEADERS)
	$(CC) $(CFLAGS) $@ $<

$(HDRDIR) $(SRCDIR) $(OBJDIR) $(TSTDIR): % :
	mkdir $@
