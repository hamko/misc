PKG = libhal

OBJ_DIR = $(shell pwd)/objs
HEAD_DIR = $(shell pwd)/heads

SUBDIRS =  2008ir adc encoder led linesensor motor port pscontroller switch servo

export OBJ_DIR HEAD_DIR

all: $(OBJ_DIR) $(HEAD_DIR) $(PHEAD_DIR) multi_make $(PKG).a $(PKG).h $(PKG)_prv.h

$(OBJ_DIR):
	mkdir $@

$(HEAD_DIR):
	mkdir $@

$(PHEAD_DIR):
	mkdir $@

$(PKG).h: $(shell ls ./heads/*_prv.h)
	echo "#ifndef _LIBHAL_H_" > $@
	echo "#define _LIBHAL_H_" >> $@
	@for inc in $(patsubst %_prv.h,%.h,$(shell ls ./heads/*_prv.h)); \
		do (echo "#include\""$$inc"\"" >> $@); \
	done
	echo "#endif //_LIBHAL_H_" >> $@

$(PKG)_prv.h: $(shell ls ./heads/*_prv.h)
	echo "#ifndef _LIBHAL_PRV_H_" > $@
	echo "#define _LIBHAL_PRV_H_" >> $@
	@for inc in $(shell ls ./heads/*_prv.h); \
		do (echo "#include\""$$inc"\"" >> $@); \
	done
	echo "#endif //_LIBHAL_PRV_H_" >> $@

$(PKG).a: $(shell ls $(OBJ_DIR)/*.o)
	ar rcsv $@ $(shell ls $(OBJ_DIR)/*.o)

.PHONY : multi_make
multi_make:
	for subdir in $(SUBDIRS); do \
	  make -C $$subdir multi_make;\
	done

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) $(HEAD_DIR) $(PHEAD_DIR)
	rm -f $(PKG).a $(PKG).h $(PKG)_prv.h *~
	for subdir in $(SUBDIRS); do \
	  make -C $$subdir clean; \
	done
