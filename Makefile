
CC:=gcc
RM:=del /f /q
SRCS:=genpwd.c genpwd.h
PROGNAME:=genpwd
EXT:=.exe
OBJDIR:=obj
OBJS:=$(OBJDIR)/genpwd.o
BINDIR:=bin
BIN:=$(BINDIR)/$(PROGNAME)$(EXT)
CFLAGS:=-Wextra -Wall -O2

release: $(OBJS)
	$(CC) -o $(BIN) $(OBJS) $(CFLAGS)

debug: $(OBJS)
	$(CC) -o $(BIN) $(SRCS) $(CFLAGS) -D DEBUG

$(OBJDIR)/%.o: %.c
	$(CC) -c -o $@ $<

clean:
	$(RM) $(OBJS) $(BIN)

setup:
	mkdir bin
	mkdir obj
