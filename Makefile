SRCS = src/main.c src/opt.c src/inotify.c
BINS = inot

all:
	make $(BINS)

debug:
	make $(BINS) "CFLAGS=-DDEBUG"

clean:
	rm -f $(BINS)

$(BINS): $(SRCS)
	gcc $(CFLAGS) -o $@ $^

