#
# Makefile for the Crypt4GH utility
#

MAIN_EXEC = crypt4gh

CC=gcc
LD=ld
AS=gcc -c
CFLAGS=-Wall -Wstrict-prototypes -Werror -fPIC -I. -I/usr/local/include `libgcrypt-config --cflags` -O2
LIBS=-lgpg-error -lgcrypt

ifdef SYSLOG
CFLAGS += -DHAS_SYSLOG
endif

HEADERS = utils.h config.h crypt4gh.h cli.h json.h $(wildcard jsmn/*.h)
SOURCES = main.c config.c crypt4gh.c cli.c json.c $(wildcard jsmn/*.c)
OBJECTS = $(SOURCES:%.c=%.o)

.PHONY: all clean debug1 debug2 debug3
.SUFFIXES: .c .o

all: $(MAIN_EXEC)

debug1: CFLAGS += -DDEBUG=1 -g -DREPORT
debug1: $(MAIN_EXEC)

debug2: CFLAGS += -DDEBUG=2 -g -DREPORT
debug2: $(MAIN_EXEC)

debug3: CFLAGS += -DDEBUG=3 -g -DREPORT
debug3: $(MAIN_EXEC)

$(MAIN_EXEC): $(HEADERS) $(OBJECTS) 
	@echo "Creating $@"
	@$(CC) -o $@ $(OBJECTS) $(LIBS)

%.o: %.c $(HEADERS)
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) -c -o $@ $<

clean:
	-rm -f $(MAIN_EXEC) $(OBJECTS)
