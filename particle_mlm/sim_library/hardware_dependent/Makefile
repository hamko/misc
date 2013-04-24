PKG = libhdw
SUBDIRS = init null wheel linesensor

OBJ_DIR = $(shell pwd)/objs
HEAD_DIR = $(shell pwd)/heads

export OBJ_DIR HEAD_DIR

all:$(HEAD_DIR) $(OBJ_DIR) multi_make $(PKG).h $(PKG).a

$(HEAD_DIR):
	mkdir $@

$(OBJ_DIR):
	mkdir $@

$(PKG).h: $(shell ls heads/*.h)
	echo "#ifndef _LIBHDW_H_" > $@
	echo "#define _LIBHDW_H_" >> $@
	@for inc in $(shell ls heads/*.h); \
		do (echo "#include\""$$inc"\"" >> $@); \
	done
	echo "#endif //_LIBHDW_H_" >> $@

$(PKG).a: $(shell ls $(OBJ_DIR)/*.o)
	ar rcsv $@ $(shell ls $(OBJ_DIR)/*.o)

.PHONY: multi_make
multi_make:
	for subdir in $(SUBDIRS); do \
	  make -C $$subdir multi_make||exit 1;\
	done

.PHONY: clean
clean:
	rm -f $(PKG).a $(PKG).h *~ ;\
	rm -rf $(OBJ_DIR) $(HEAD_DIR)
	for subdir in $(SUBDIRS); do \
	  make -C $$subdir clean; \
	done
