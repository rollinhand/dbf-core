## Process this file with automake to produce Makefile.in

SUBDIRS = src po $(DOCDIR)

spec = $(PACKAGE).spec

EXTRA_DIST = COPYING AUTHORS INSTALL intltool-extract.in intltool-merge.in intltool-update.in $(spec) $(spec).in autogen.sh

rpm: $(distdir).tar.gz
	rpm -ta $(distdir).tar.gz

