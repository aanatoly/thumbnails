## miniconf makefiles ## 1.1 ##

TOPDIR := ..

THS = image-imgth.thumbnailer video-ffmpeg.thumbnailer
all: $(THS)

image-imgth.thumbnailer:
	./mk-img-spec.sh > $@

CLEANLIST += image-imgth.thumbnailer

install:
	$Qinstall -d $(DESTDIR)$(DATADIR)/thumbnailers
	$Qinstall -m 644 -t $(DESTDIR)$(DATADIR)/thumbnailers $(THS)

include $(TOPDIR)/.config/rules.mk
