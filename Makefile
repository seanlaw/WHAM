include ./makefile.defs

all: libwham.$(LIBEXT) programs

libwham.$(LIBEXT):
	$(UNAME)
	$(MAKE) -C lib 

programs:
	$(MAKE) -C src

clean:
	$(MAKE) -C lib clean
	$(MAKE) -C src clean
	rm -rf *~
	rm -rf *stackdump
