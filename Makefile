SRCS = src/main.c src/opt.c src/inotify.c
BINS = main

all: $(BINS)

clean:
	rm -f $(BINS)

$(BINS): $(SRCS)
	gcc -o $@ $^

