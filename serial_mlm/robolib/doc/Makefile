INSTALL_PREFIX = __INSTALL_
INSTALL_OPTION = ../core/install_option.h
PREDEF = predef.doxyfile

.PHONY: doc clean

doc:
	grep $(INSTALL_PREFIX) $(INSTALL_OPTION) | sed -e 's/^.*[ \t]\($(INSTALL_PREFIX)[^ \t]*\).*$$/PREDEFINED += \1/' > $(PREDEF)
	grep \#ifdef $(INSTALL_OPTION) | sed -e 's/^\#ifdef[ \t]*\([^ \t]*\)/PREDEFINED += \1/' >> $(PREDEF)
	doxygen

clean:
	rm -rf *~ html $(PREDEF)


