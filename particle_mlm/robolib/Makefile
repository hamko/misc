ROBOLIB_PATH = .
include $(ROBOLIB_PATH)/include.mk

SUBDIRS = $(shell find -mindepth 2 -maxdepth 2 -name Makefile | sed -e 's/^.\/\(.*\)\/Makefile$$/\1/' | sed -e '/doc/d')
# doc を除くすべてのMakefileをもつディレクトリ

.PHONY: all clean doc
.DELETE_ON_ERROR: $(ROBOLIB)

all: 
	for subdir in $(SUBDIRS); do \
	  make -C $$subdir COMPILER=$(COMPILER) || exit 1; \
	done
ifeq ($(MAKE_COPY),TRUE)
	cp $(ROBOLIB) $(COMPILER).a
endif
clean:
	for subdir in $(SUBDIRS); do \
	  make -C $$subdir clean COMPILER=$(COMPILER) || exit 1; \
	done
	make -C doc clean
	rm -f *~ $(ROBOLIB)
ifneq ($(MAKE_COPY),TRUE)
	rm -f $(shell ls $(ROBOLIB_PATH)/templete/include_mk/*.mk | sed -e 's/.*\/\([^/]*\)\.mk/\1.a/')
endif
	rm -f heads/*.h

doc:
	make -C doc
