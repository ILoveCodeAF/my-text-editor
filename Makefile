CC=gcc
CXX=g++

RM=rm -f
CP=cp -f
DIR=$(HOME)/.local/bin

LDLIBS= -lX11 -lXft -lpthread
CFLAGS= `pkg-config --cflags --libs freetype2`

SRCS=te.c x.c queue.c io.c line.c
OBJS=$(SRCS:.c=.o)



all: te

te: $(OBJS)
	$(CC) -o te $(OBJS) $(LDLIBS) $(CFLAGS)

.c.o:
	$(CC) -o $@ -c $< $(LDLIBS) $(CFLAGS)

x.o: config.h x.h queue.h te.h
te.o: x.h queue.h te.h io.h
io.o: x.h te.h io.h line.h
line.o: line.h


$(OBJS): config.h

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) te

install: te
	$(CP) te $(DIR)

uninstall:
	$(RM) $(DIR)/te

.PHONY: all clean distclean install uninstall
