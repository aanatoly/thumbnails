

PKG = gdk-pixbuf-2.0 
CFLAGS = -Wall -g $(shell pkg-config --cflags $(PKG))
LDFLAGS = $(shell pkg-config --libs $(PKG))


all: imgth image-imgth.thumbnailer

%.o: %.c
	gcc $^ -o $@ -c $(CFLAGS)

imgth: imgth.o
	gcc $^ -o $@  $(LDFLAGS)

image-imgth.thumbnailer:
	./mkspec.sh > $^

clean:
	rm -f imgth *.o image-imgth.thumbnailer
