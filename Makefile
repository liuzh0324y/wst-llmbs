
default:	build

build:
	test -d objs || mkdir -p objs
	$(MAKE) -f src/Makefile

install:
	$(MAKE) -f src/Makefile install

remove:
	$(MAKE) -f src/Makefile remove
	
deb:
	$(MAKE) -f src/Makefile deb

undeb:
	$(MAKE) -f src/Makefile undeb

clean:
	rm -rf objs