%.1: %.sgml
	docbook-to-man $< > $@

distclean: clean

clean:
	rm -f ${man_MANS}

EXTRA_DIST = dbf.sgml

man_MANS = $(EXTRA_DIST:.sgml=.1)


