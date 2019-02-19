SUBDIRS := $(wildcard */.)

all: $(SUBDIRS) drawtool
$(SUBDIRS):
	$(MAKE) -C $@

drawtool:
	chmod +x draw-dataset

clean:
	chmod -x draw-dataset
	rm -f *.csv

.PHONY: all $(SUBDIRS)
