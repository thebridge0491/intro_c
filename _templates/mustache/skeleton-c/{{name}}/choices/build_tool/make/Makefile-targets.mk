# Targets Makefile script.
#----------------------------------------
# Common automatic variables legend (GNU make: make (Linux) gmake (FreeBSD)):
# $* - basename (cur target)  $^ - name(s) (all depns)  $< - name (1st depn)
# $@ - name (cur target)      $% - archive member name  $? - changed depns

FMTS ?= tar.gz
distdir = $(proj).$(version)

#LINTOPTS = --enable=information --report-progress --quiet --force --suppress=missingIncludeSystem --std=posix --std=c99 -Iinclude -I../src
LINTOPTS = --enable=all --report-progress --quiet --force --std=c99 --std=posix -Iinclude -I../src

lib/%.so : 
	-$(LINK.c) -fPIC -shared -Wl,-soname,$*.so.$(ver_major) $^ -o $@.$(version) $(LDLIBS)
	-cd lib ; ln -sf $*.so.$(version) $*.so.$(ver_major) \
		&& ln -sf $*.so.$(version) $*.so
lib/%.dylib : 
	-$(LINK.c) -fPIC -dynamiclib -undefined suppress -flat_namespace -Wl,-install_name,@rpath/$*.$(ver_major).dylib,-current_version,$(version),-compatibility_version,$(version) $^ -o lib/$*.$(version).dylib $(LDLIBS)
	-cd lib ; ln -sf $*.$(version).dylib $*.$(ver_major).dylib \
		&& ln -sf $*.$(version).dylib $*.dylib

.PHONY: help clean check uninstall install dist doc lint report
help: ## help
	@echo "##### subproject: $(proj) #####"
	@echo "Usage: $(MAKE) [target] -- some valid targets:"
#	-@for fileX in $(MAKEFILE_LIST) `if [ -z "$(MAKEFILE_LIST)" ] ; then echo Makefile ../Makefile-targets.mk ; fi` ; do \
#		grep -ve '^[A-Z]' $$fileX | awk '/^[^.%][-A-Za-z0-9_]+[ ]*:.*$$/ { print "...", substr($$1, 1, length($$1)) }' | sort ; \
#	done
	-@for fileX in $(MAKEFILE_LIST) `if [ -z "$(MAKEFILE_LIST)" ] ; then echo Makefile ../Makefile-targets.mk ; fi` ; do \
		grep -E '^[ a-zA-Z_-]+:.*?## .*$$' $$fileX | \
		awk 'BEGIN {FS = ":.*?## "}; {printf "%-25s%s\n", $$1, $$2}' ; \
	done
clean: ## clean build artifacts
	-rm -rf ../build/* ../build/.??*
check: testCompile ## run test [TOPTS=""]
#	export [DY]LD_LIBRARY_PATH=. # ([da|ba|z]sh Linux)
#	setenv [DY]LD_LIBRARY_PATH . # (tcsh FreeBSD)
	-LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):lib tests/ts_main $(TOPTS)
uninstall install: ## [un]install artifacts
	-@if [ "uninstall" = "$@" ] ; then \
		rm -i $(PREFIX)/lib/*$(proj)*.* $(PREFIX)/lib/pkgconfig/$(proj).pc ; \
		rm -ir $(PREFIX)/share/doc/$(proj) ; rm -i $(PREFIX)/bin/*$(proj)* ; \
	else cp -fR bin include lib share $(PREFIX)/ ; fi
dist: ## [FMTS="tar.gz"] archive source code
	-@mkdir -p $(distdir) ; cp -f ../exclude.lst .
#	#-(cd .. ; zip -9 -q --exclude @exclude.lst -r - . | unzip -od build/$(distdir) -)
	-tar --format=posix --dereference --exclude-from=exclude.lst -C .. -cf - . | tar -xpf - -C $(distdir)
	
	-@for fmt in `echo $(FMTS) | tr ',' ' '` ; do \
		case $$fmt in \
			zip) echo "### $(distdir).zip ###" ; \
				rm -f $(distdir).zip ; \
				zip -9 -q -r $(distdir).zip $(distdir) ;; \
			*) tarext=`echo $$fmt | grep -e '^tar$$' -e '^tar.xz$$' -e '^tar.bz2$$' || echo tar.gz` ; \
				echo "### $(distdir).$$tarext ###" ; \
				rm -f $(distdir).$$tarext ; \
				tar --posix -L -caf $(distdir).$$tarext $(distdir) ;; \
		esac \
	done
	-@rm -r $(distdir)
doc: ## generate documentation
	-doxygen ../Doxyfile_*.txt ; rm -fr share/doc/$(proj)/html
	-mv -f html share/doc/$(proj)/html
lint: ## lint check
	-cppcheck $(LINTOPTS) ../src
report: ## report code coverage
#	# read coverage data w/ [llvm-cov] gcov -f -b -n *.gcda
#	find . -type f -name '*.gcda' -exec llvm-cov gcov -f -b --no-output {} \;
#	# read coverage data w/ lcov -c -d . -o .coverage ... *.gcda
	-lcov --capture -d . -o .coverage --gcov-tool ../llvm-gcov.sh
	-genhtml -o cov .coverage
