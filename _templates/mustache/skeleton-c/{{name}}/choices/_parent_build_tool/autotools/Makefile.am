# Multi-package project automake Makefile.am script.
ACLOCAL_AMFLAGS = -I m4
SUBDIRS = common app

.PHONY : help doc run debug valgrind
.DEFAULT_GOAL = help

help: $(SUBDIRS)
	-@for dirX in $(SUBDIRS) ; do mkdir -p $$dirX/.libs ; done
	-for dirX in $(SUBDIRS) ; do $(MAKE) -C $$dirX $@ ; done
	@echo "       cd build ; sh ../bootstrap.sh configure [OPTS=??]"
	@echo "Usage: $(MAKE) [SUBDIRS="$(SUBDIRS)"] [target]"
all-local: $(SUBDIRS)
	-for dirX in $(SUBDIRS) ; do $(MAKE) -C $$dirX $@ ; done
doc: html
run debug valgrind:
	-$(MAKE) -C app $@


GZIP_ENV = --best --force
TAROPTS = --format=posix --dereference --exclude-from=exclude.lst
ZIPOPTS = -9 -q --exclude @exclude.lst -r
am__tar = tar $(TAROPTS) -chf - "$$tardir"
##EXTRA_DIST = exclude.lst bootstrap.sh configure.ac Makefile.am

dist-hook:
	cp -f $(srcdir)/exclude.lst .
#	(cd $(srcdir) ; zip $(ZIPOPTS) - .) | unzip -od $(distdir) -
	tar $(TAROPTS) -C $(srcdir) -cf - . | tar -xpf - -C $(distdir)
dist-zip : distdir
	-rm -f $(distdir).zip
	zip $(ZIPOPTS) $(distdir).zip $(distdir)
	$(am__post_remove_distdir)
