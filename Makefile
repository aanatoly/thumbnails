## miniconf makefiles ## 1.1 ##

TOPDIR := .

SUBDIRS := imgth \
    specs

install:
	$Qinstall -D -m 755 -T thumbnailer $(DESTDIR)$(BINDIR)/thumbnailer


include $(TOPDIR)/.config/rules.mk
