
default:	build

build:
	test -d objs || mkdir -p objs
	$(MAKE) -f wst/Makefile

install:
	$(MAKE) -f wst/Makefile install

remove:
	$(MAKE) -f wst/Makefile remove
	
deb:
	$(MAKE) -f wst/Makefile deb

undeb:
	$(MAKE) -f wst/Makefile undeb

clean:
	rm -rf objs