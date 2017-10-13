
default:	build

build:
	$(MAKE) -f src/Makefile

test:
	$(MAKE) -f tests/Makefile

clean:
	$(MAKE) -f src/Makefile clean

testclean:
	$(MAKE) -f tests/Makefile clean